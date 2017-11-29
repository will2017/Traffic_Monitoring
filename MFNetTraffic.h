#if !defined(AFX_MFNETTRAFFIC_H__9CA9C41F_F929_4F26_BD1F_2B5827090494__INCLUDED_)
#define AFX_MFNETTRAFFIC_H__9CA9C41F_F929_4F26_BD1F_2B5827090494__INCLUDED_
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <afxtempl.h>
class MFNetTraffic  
{
public:
	enum TrafficType //��������
	{
		AllTraffic		= 388,//�ܵ�����
		IncomingTraffic	= 264,//��������
		OutGoingTraffic	= 506 //�������
	};
	void	SetTrafficType(int trafficType);		//������������
	DWORD	GetInterfaceTotalTraffic(int index);	//�õ�index�����ӿڵ�������
	BOOL	GetNetworkInterfaceName(CString *InterfaceName, int index);//�õ�����ӿ�����
	int		GetNetworkInterfacesCount();			//�õ��ӿڵ���Ŀ
	double	GetTraffic(int interfaceNumber);		//�õ�����
	DWORD	GetInterfaceBandwidth(int index);		//�õ�index�����ӿڵĴ���
	MFNetTraffic();
	virtual ~MFNetTraffic();

private:
	BOOL		GetInterfaces();
	double		lasttraffic;
	CStringList Interfaces;		
	CList < DWORD, DWORD &>		Bandwidths;	//����
	CList < DWORD, DWORD &>		TotalTraffics;//�ܵ�����
	int CurrentInterface;
	int CurrentTrafficType;
};
#endif // !defined(AFX_MFNETTRAFFIC_H__9CA9C41F_F929_4F26_BD1F_2B5827090494__INCLUDED_)