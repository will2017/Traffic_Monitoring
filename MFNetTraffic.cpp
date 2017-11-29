#include "stdafx.h"
#include "MFNetTraffic.h"
#include "float.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
#include "winperf.h"
MFNetTraffic::MFNetTraffic()
{
	lasttraffic = 0.0;
	CurrentInterface = -1;
	CurrentTrafficType = AllTraffic;
	GetInterfaces();
}
MFNetTraffic::~MFNetTraffic()
{
	
}
PERF_OBJECT_TYPE *FirstObject(PERF_DATA_BLOCK *dataBlock)
{
  return (PERF_OBJECT_TYPE *) ((BYTE *)dataBlock + dataBlock->HeaderLength);
}
PERF_OBJECT_TYPE *NextObject(PERF_OBJECT_TYPE *act)
{
  return (PERF_OBJECT_TYPE *) ((BYTE *)act + act->TotalByteLength);
}
PERF_COUNTER_DEFINITION *FirstCounter(PERF_OBJECT_TYPE *perfObject)
{
  return (PERF_COUNTER_DEFINITION *) ((BYTE *) perfObject + perfObject->HeaderLength);
}
PERF_COUNTER_DEFINITION *NextCounter(PERF_COUNTER_DEFINITION *perfCounter)
{
  return (PERF_COUNTER_DEFINITION *) ((BYTE *) perfCounter + perfCounter->ByteLength);
}
PERF_COUNTER_BLOCK *GetCounterBlock(PERF_INSTANCE_DEFINITION *pInstance)
{
  return (PERF_COUNTER_BLOCK *) ((BYTE *)pInstance + pInstance->ByteLength);
}
PERF_INSTANCE_DEFINITION *FirstInstance (PERF_OBJECT_TYPE *pObject)
{
  return (PERF_INSTANCE_DEFINITION *)  ((BYTE *) pObject + pObject->DefinitionLength);
}
PERF_INSTANCE_DEFINITION *NextInstance (PERF_INSTANCE_DEFINITION *pInstance)
{
  PERF_COUNTER_BLOCK  *pCtrBlk = GetCounterBlock(pInstance);
  return (PERF_INSTANCE_DEFINITION *) ((BYTE *)pInstance + pInstance->ByteLength + pCtrBlk->ByteLength);
}
char *WideToMulti(wchar_t *source, char *dest, int size)
{
  WideCharToMultiByte(CP_ACP, 0, source, -1, dest, size, 0, 0);
  return dest;
}



//�õ��ӿں�ΪinterfaceNumber�Ľӿ�����
double MFNetTraffic::GetTraffic(int interfaceNumber)
{
	try
	{
#define DEFAULT_BUFFER_SIZE 40960L
		POSITION pos;
		CString InterfaceName;
		pos = Interfaces.FindIndex(interfaceNumber);
		if(pos==NULL)
			return 0.0;
		//�õ���ǰ�Ľӿ�����
		InterfaceName = Interfaces.GetAt(pos);
		//Ϊ�������ݻ���
		unsigned char *data = new unsigned char [DEFAULT_BUFFER_SIZE];
		// ��RegQueryValueEx���ص�ֵ:�����к��Ըı���
		DWORD type;
		// ����ĳߴ�
		DWORD size = DEFAULT_BUFFER_SIZE;
		// RegQueryValueEx���ص�ֵ
		DWORD ret;
		// ���������(������510)��ѯ��������
		while((ret = RegQueryValueEx(
			HKEY_PERFORMANCE_DATA, "510", NULL, &type, data, &size)) != ERROR_SUCCESS) 
		{
			if(ret == ERROR_MORE_DATA) 
			{
				// ����ߴ�̫С�������ڴ����
				size += DEFAULT_BUFFER_SIZE;		
				delete [] data;
				data = new unsigned char [size];
			} 
			else 
			{
				// δ����Ĵ���
				return 1;
			}
		}
		//�������ݿ�
		PERF_DATA_BLOCK *dataBlockPtr = (PERF_DATA_BLOCK *)data;
		//ö�������е�һ������
		PERF_OBJECT_TYPE *objectPtr = FirstObject(dataBlockPtr);
		//�������� 
		for(int a=0 ; a<(int)dataBlockPtr->NumObjectTypes ; a++) 
		{
			char nameBuffer[255];
			// �ж��Ƿ������������������510
			if(objectPtr->ObjectNameTitleIndex == 510) 
			{
				// ƫ�Ʊ���
				DWORD processIdOffset = ULONG_MAX;
				// �ҵ���һ��������
				PERF_COUNTER_DEFINITION *counterPtr = FirstCounter(objectPtr);
				// ��������
				for(int b=0 ; b<(int)objectPtr->NumCounters ; b++) 
				{
					// �жϽ��յ����������Ƿ���������Ҫ��
					if((int)counterPtr->CounterNameTitleIndex == CurrentTrafficType)
						processIdOffset = counterPtr->CounterOffset;
					// ��һ��������
					counterPtr = NextCounter(counterPtr);
				}
				//�������Ͳ���������Ҫ��
				if(processIdOffset == ULONG_MAX) {
					delete [] data;
					return 1;
				}
				// �ҵ���һ��ʵ��(instance)
				PERF_INSTANCE_DEFINITION *instancePtr = FirstInstance(objectPtr);
				DWORD fullTraffic;
				DWORD traffic;
				//��������ʵ��
				for(int b=0 ; b<objectPtr->NumInstances ; b++) 
				{
					wchar_t *namePtr = (wchar_t *) ((BYTE *)instancePtr + instancePtr->NameOffset);		
					// �õ����ʵ�е�PERF_COUNTER_BLOCK
					PERF_COUNTER_BLOCK *counterBlockPtr = GetCounterBlock(instancePtr);
					// �������ǵõ��˽ӿڵ�����
					char *pName = WideToMulti(namePtr, nameBuffer, sizeof(nameBuffer));
					CString iName;
					iName.Format("%s",pName);
					POSITION pos = TotalTraffics.FindIndex(b);
					if(pos!=NULL)
					{
						fullTraffic = *((DWORD *) ((BYTE *)counterBlockPtr + processIdOffset));
						TotalTraffics.SetAt(pos,fullTraffic);
					}
					// �����ǰ�Ľӿھ�������ѡ��Ľӿ�
					if(InterfaceName == iName)
					{
						traffic = *((DWORD *) ((BYTE *)counterBlockPtr + processIdOffset));
						double acttraffic = (double)traffic;
						double trafficdelta;
						// �жϴ���Ľӿ��Ƿ����µ�
						if(CurrentInterface != interfaceNumber)
						{
							lasttraffic = acttraffic;
							trafficdelta = 0.0;
							CurrentInterface = interfaceNumber;
						}
						else
						{
							trafficdelta = acttraffic - lasttraffic;
							lasttraffic = acttraffic;
						}
						delete [] data;
						return(trafficdelta);
					}
					//��һ��ʵ��
					instancePtr = NextInstance(instancePtr);
				}
			}
			// ��һ������
			objectPtr = NextObject(objectPtr);
		}
		delete [] data;
		return 0;
	}
	catch(...)
	{
		return 0;
	}
}


//ö�ٰ�װ�Ľӿڣ������
BOOL MFNetTraffic::GetInterfaces()
{
	try
	{
#define DEFAULT_BUFFER_SIZE 40960L
		
		Interfaces.RemoveAll();
		unsigned char *data = (unsigned char*)malloc(DEFAULT_BUFFER_SIZE);
		DWORD type;
		DWORD size = DEFAULT_BUFFER_SIZE;
		DWORD ret;
		char s_key[4096];
		sprintf( s_key , "%d" , 510 );
		//RegQueryValueEx�Ĺ̶����ø�ʽ		
		while((ret = RegQueryValueEx(HKEY_PERFORMANCE_DATA, s_key, 0, &type, data, &size)) != ERROR_SUCCESS) {
			while(ret == ERROR_MORE_DATA) 
			{
				size += DEFAULT_BUFFER_SIZE;
				data = (unsigned char*) realloc(data, size);
			} 
			if(ret != ERROR_SUCCESS)
			{
				return FALSE;
			}
		}
		//�õ����ݿ�		
		PERF_DATA_BLOCK	 *dataBlockPtr = (PERF_DATA_BLOCK *)data;
		//�õ���һ������
		PERF_OBJECT_TYPE *objectPtr = FirstObject(dataBlockPtr);
		for(int a=0 ; a<(int)dataBlockPtr->NumObjectTypes ; a++) 
		{
			char nameBuffer[255];
			if(objectPtr->ObjectNameTitleIndex == 510) 
			{
				DWORD processIdOffset = ULONG_MAX;
				PERF_COUNTER_DEFINITION *counterPtr = FirstCounter(objectPtr);
				for(int b=0 ; b<(int)objectPtr->NumCounters ; b++) 
				{
					if(counterPtr->CounterNameTitleIndex == 520)
						processIdOffset = counterPtr->CounterOffset;
					counterPtr = NextCounter(counterPtr);
				}
				if(processIdOffset == ULONG_MAX) {
					free(data);
					return 1;
				}
				PERF_INSTANCE_DEFINITION *instancePtr = FirstInstance(objectPtr);
				for(int b=0 ; b<objectPtr->NumInstances ; b++) 
				{
					wchar_t *namePtr = (wchar_t *) ((BYTE *)instancePtr + instancePtr->NameOffset);
					PERF_COUNTER_BLOCK *counterBlockPtr = GetCounterBlock(instancePtr);
					char *pName = WideToMulti(namePtr, nameBuffer, sizeof(nameBuffer));
					DWORD bandwith = *((DWORD *) ((BYTE *)counterBlockPtr + processIdOffset));				
					DWORD tottraff = 0;
					Interfaces.AddTail(CString(pName));//���õ����������Ʒŵ��ַ����б���
					Bandwidths.AddTail(bandwith);
					TotalTraffics.AddTail(tottraff); 
					instancePtr = NextInstance(instancePtr);
				}
			}
			objectPtr = NextObject(objectPtr);
		}
		free(data);
		return TRUE;
	}
	catch(...)
	{
		return FALSE;
	}
}


//���ذ�װ�Ľӿ���
int MFNetTraffic::GetNetworkInterfacesCount()
{
	return Interfaces.GetCount()-1;
}


//���ؽӿ�����
BOOL MFNetTraffic::GetNetworkInterfaceName(CString *InterfaceName, int index)
{
	POSITION pos = Interfaces.FindIndex(index);
	if(pos==NULL)
		return FALSE;
	InterfaceName->Format("%s",Interfaces.GetAt(pos));
	return TRUE;
}


//���ؽӿڴ���
DWORD MFNetTraffic::GetInterfaceBandwidth(int index)
{
	POSITION pos = Bandwidths.FindIndex(index);
	if(pos==NULL)
		return 0;
	else 
		return Bandwidths.GetAt(pos) / 1000/1000;
	//Win32_NetworkAdapter wna;
}


//����ӿڽ��պͷ��͵�������
DWORD MFNetTraffic::GetInterfaceTotalTraffic(int index)
{
	DWORD	totaltraffic = 0;
	POSITION	pos;
	pos= TotalTraffics.FindIndex(index);
	if(pos!=NULL)
	{
		totaltraffic = TotalTraffics.GetAt(pos); 
		if(totaltraffic == 0.0)
		{
			GetTraffic(index);
			pos= TotalTraffics.FindIndex(index);
			if(pos!=NULL)
			{
				totaltraffic = TotalTraffics.GetAt(pos); 
			}
		}
	}
	return(totaltraffic);
}


void MFNetTraffic::SetTrafficType(int trafficType)
{
	CurrentTrafficType = trafficType;
}