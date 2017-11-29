// Traffic_MonitoringDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Traffic_Monitoring.h"
#include "Traffic_MonitoringDlg.h"
#include <opencv2/core/core.hpp> 
#include <opencv2/highgui/highgui.hpp> 

//#include "CLegend.h"  
#include <vector>
#include <fstream> 
#include <iostream>
#include <sstream>

//#include "Windows.h"
#include <afxwin.h>
#include  <direct.h> 
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// 全局变量
HWND myMainWnd;          // 本窗口的句柄
UINT interfaceNum = 0;   // 网卡号
//NOTIFYICONDATA nid;      // 托盘结构体
UINT second=0;
UINT minute=0;
UINT hour=0; 
long count_sec=0;
bool start_flag=false;
std::vector<double> vec_dl,vec_ul;
std::vector<double>::iterator it;

double DL_totaltraffic=0; 
double UL_totaltraffic=0; 
double totaltraffic=0; // 流量
float DL_max_speed=0;
float UL_max_speed=0;
float DL_AVG=0;
float UL_AVG=0;
float dl_temp_speed=0;
float ul_temp_speed=0;
int count_no=0;//统计周期次数
float count_total_dl=0;//统计的速度相加来求平均
float count_total_ul=0;


CString selet_name;
CString connection_type;
CString network_type;
CString signal_strength;
CString transmission_type;
CString file_name;
CString savefilename;
CString img_path;
CString filename;//for img
CString mode;//操作模式

bool select_benchmark;
int cycle=0;
bool save_csv=false;
bool save_img=false;
bool auto_do=false;
bool app_first_run=true;
bool app_first_run_img=true;


std::ofstream oFile; 




void WriteLog(CString logStr)
{

	std::ofstream outfile;
	outfile.open("D:\\log.txt", std::ios::app);
	if (outfile.is_open())
	{
		outfile << logStr << std::endl;;
		outfile.close();
	}
//	fclose(fp);
}

CTraffic_MonitoringDlg::CTraffic_MonitoringDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTraffic_MonitoringDlg::IDD, pParent)
	//, m_Chart(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDB_BITMAP_PLAY);
}


void CTraffic_MonitoringDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTraffic_MonitoringDlg)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_STATIC1, m_static1);
	DDX_Control(pDX, IDC_BUTTON1, m_Btn);
	DDX_Control(pDX, IDC_BUTTON2, my_But_Reset);
	DDX_Control(pDX, IDC_COMBO2, mycb);

}

BEGIN_MESSAGE_MAP(CTraffic_MonitoringDlg, CDialog)
	//{{AFX_MSG_MAP(CTraffic_MonitoringDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDOWN()
	//ON_MESSAGE(WM_TRAY, OnTrayNotify)  // 托盘消息的消息映射  
	ON_WM_CLOSE()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CTraffic_MonitoringDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CTraffic_MonitoringDlg::OnBnClickedButton2)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CTraffic_MonitoringDlg::OnCbnSelchangeCombo2)
	ON_BN_CLICKED(IDC_RADIO_DUT, &CTraffic_MonitoringDlg::OnBnClickedRadioDut)
	ON_BN_CLICKED(IDC_RADIO_BEN, &CTraffic_MonitoringDlg::OnBnClickedRadioBen)
	ON_BN_CLICKED(IDC_RADIO_LAN, &CTraffic_MonitoringDlg::OnBnClickedRadioLan)
	ON_BN_CLICKED(IDC_RADIO_WIFI, &CTraffic_MonitoringDlg::OnBnClickedRadioWifi)
	ON_BN_CLICKED(IDC_RADIO_LTE, &CTraffic_MonitoringDlg::OnBnClickedRadioLte)
	ON_BN_CLICKED(IDC_RADIO_3G, &CTraffic_MonitoringDlg::OnBnClickedRadio3g)
	ON_BN_CLICKED(IDC_RADIO_Good, &CTraffic_MonitoringDlg::OnBnClickedRadioGood)
	ON_BN_CLICKED(IDC_RADIO_Weak, &CTraffic_MonitoringDlg::OnBnClickedRadioWeak)
	ON_BN_CLICKED(IDC_RADIO_UL, &CTraffic_MonitoringDlg::OnBnClickedRadioUl)
	ON_BN_CLICKED(IDC_RADIO_DL, &CTraffic_MonitoringDlg::OnBnClickedRadioDl)
	ON_BN_CLICKED(IDC_RADIO_ULDL, &CTraffic_MonitoringDlg::OnBnClickedRadioUldl)
	ON_BN_CLICKED(IDC_RADIO_AUTO, &CTraffic_MonitoringDlg::OnBnClickedRadioAuto)
	ON_BN_CLICKED(IDC_RADIO_MAN, &CTraffic_MonitoringDlg::OnBnClickedRadioMan)
END_MESSAGE_MAP()



BOOL CTraffic_MonitoringDlg::OnInitDialog()
{
	
	//WriteLog("OnInitDialog()");
	//RegisterActiveX();
	CDialog::OnInitDialog();
	SetWindowPos(&wndTopMost, GetSystemMetrics(SM_CXSCREEN)-500, GetSystemMetrics(SM_CYSCREEN)-600, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE);
	//SetIcon(m_hIcon, TRUE);			// Set big icon
	//SetIcon(m_hIcon, TRUE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	// 保存本窗口的窗口句柄
	myMainWnd = m_hWnd;
	
	// 设置窗体透明
	SetWindowTransparency(500);
	//设置带宽及网卡名称到标题处
	DWORD bandwidth;
	CString band,name;
	bandwidth=netTraffic_download.GetInterfaceBandwidth((int)interfaceNum);
	
	//设置秒表Static字体  
    m_Font.CreatePointFont(220, "Arial", NULL);  
    m_static1.SetFont(&m_Font,true);  
   // m_static1.SetWindowText("20");  

	//设置按钮图片 
	HBITMAP hBmp=::LoadBitmap(AfxGetInstanceHandle(), 
	MAKEINTRESOURCE(IDB_BITMAP_PLAY));   
	m_Btn.SetBitmap(hBmp);

	HBITMAP hBmp1=::LoadBitmap(AfxGetInstanceHandle(), 
	MAKEINTRESOURCE(IDB_BITMAP_RESET));   
	my_But_Reset.SetBitmap(hBmp1);

	
	int netCount=0;
	MFNetTraffic netTraffic;
	netCount = netTraffic.GetNetworkInterfacesCount();

	CButton* radio=(CButton*)GetDlgItem(IDC_RADIO_MAN);
	radio->SetCheck(1);
	mode="Manually";
	GetDlgItem(IDC_EDIT_CYCLE)->EnableWindow(FALSE);//默认置灰周期
	

	// 根据网卡名添加菜单
	for (UINT i=0; i<=netCount; i++)
	{
		CString title;
		netTraffic.GetNetworkInterfaceName(&title, i);
		//mycb.AddString(title);
		mycb.InsertString(i,title);
	}
	mycb.SetCurSel(0);


	netTraffic_download.GetNetworkInterfaceName(&name,(int)interfaceNum);
	band.Format(_T( "%u "),bandwidth);
	band="Adapter："+name;//"带宽："+

	SetWindowText(band);

	SetTimer(4, 1000, NULL);


	// 总体、下载和上传的对象
	netTraffic_total.SetTrafficType(MFNetTraffic::AllTraffic);
	netTraffic_download.SetTrafficType(MFNetTraffic::IncomingTraffic);
	netTraffic_upload.SetTrafficType(MFNetTraffic::OutGoingTraffic);


	// 以IDC_SEPARATE分割对话框
	bExpand = false;//FALSE;
	ExtendDlg(IDC_SEPARATE,bExpand);


	return TRUE;  // return TRUE  unless you set the focus to a control
}


void CTraffic_MonitoringDlg::OnPaint() 
{
	//WriteLog("OnPaint() ");
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting
		

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		//dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

HCURSOR CTraffic_MonitoringDlg::OnQueryDragIcon()
{
	m_hIcon = AfxGetApp()->LoadIcon(IDB_BITMAP_PLAY);
	return  m_hIcon;
}


//screen capture
void hwnd2mat(std::string filename) {//

	//WriteLog("hwnd2mat() begin...\n");
	cv::Mat screenImg;
	HWND hwnd;
	hwnd = GetDesktopWindow();
	HDC hwindowDC, hwindowCompatibleDC;

	int height, width;
	HBITMAP hbwindow;
	//Mat src;
	BITMAPINFOHEADER  bi;

	hwindowDC = GetDC(hwnd);
	hwindowCompatibleDC = CreateCompatibleDC(hwindowDC);
	SetStretchBltMode(hwindowCompatibleDC, COLORONCOLOR);

	RECT windowsize;    // get the height and width of the screen
	GetClientRect(hwnd, &windowsize);

	height = windowsize.bottom;  //change this to whatever size you want to resize to
	width = windowsize.right;
	screenImg.create(height, width, CV_8UC4);

	// create a bitmap
	hbwindow = CreateCompatibleBitmap(hwindowDC, width, height);
	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = width;
	bi.biHeight = -height;  //this is the line that makes it draw upside down or not
	bi.biPlanes = 1;
	bi.biBitCount = 32;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrUsed = 0;
	bi.biClrImportant = 0;

	// use the previously created device context with the bitmap
	SelectObject(hwindowCompatibleDC, hbwindow);
	// copy from the window device context to the bitmap device context
	StretchBlt(hwindowCompatibleDC, 0, 0, width, height, hwindowDC, 0, 0, width, height, SRCCOPY); //change SRCCOPY to NOTSRCCOPY for wacky colors !
	GetDIBits(hwindowCompatibleDC, hbwindow, 0, height, screenImg.data, (BITMAPINFO *)&bi, DIB_RGB_COLORS);  //copy from hwindowCompatibleDC to hbwindow

																											 // avoid memory leak
	DeleteObject(hbwindow); DeleteDC(hwindowCompatibleDC); ReleaseDC(hwnd, hwindowDC);

	//screenImg = src(Rect(0, 0, sWidth, sHeight));
	//cvtColor(screenImg, g_grayImage, CV_BGR2GRAY);
	//WriteLog("hwnd2mat() end...\n");
	imwrite(filename,screenImg);
}



CString getTime(bool with_date)
{
	//WriteLog("getTime(bool with_date)\n");
	SYSTEMTIME sys;
	GetLocalTime(&sys);
	CString current_time;
	char localTime[64] = { NULL };
	if(with_date)
		sprintf_s(localTime, "%4d_%02d_%02d_%02d_%02d_%02d", sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond);
	else
		sprintf_s(localTime, "%02d:%02d:%02d",sys.wHour, sys.wMinute, sys.wSecond);
	current_time = localTime;
	return current_time;
}

CString getTime()
{
	SYSTEMTIME sys;
	GetLocalTime(&sys);
	CString current_time;
	char localTime[64] = { NULL };
	sprintf_s(localTime, "%02d_%02d_%02d",sys.wHour, sys.wMinute, sys.wSecond);
	current_time = localTime;
	return current_time;
}


/************************************************************************/
/* 设置窗体透明                                                         
/************************************************************************/
BOOL CTraffic_MonitoringDlg::SetWindowTransparency(int Byte)
{
	// 设置窗体透明
	//WriteLog("SetWindowTransparency(int Byte)\n");
	COLORREF maskColor = GetSysColor(COLOR_3DFACE); // 获取窗体颜色
	SetWindowLong(GetSafeHwnd(),GWL_EXSTYLE,GetWindowLong(GetSafeHwnd(),GWL_EXSTYLE)|0x00080000);  
	HINSTANCE hInst = LoadLibrary(_T("User32.dll"));    
	if (hInst)      
	{         
		typedef BOOL (WINAPI *MyFun)(HWND,COLORREF,BYTE,DWORD);  	        
		MyFun myfun = NULL;  	      
		myfun = (MyFun)GetProcAddress(hInst, "SetLayeredWindowAttributes");  	       
		if (myfun) 
		{
			myfun(GetSafeHwnd(),maskColor,Byte,2);  	// 100是透明度（范围0-255）
		}
		else
		{
			FreeLibrary(hInst);  
			return FALSE;
		}
		FreeLibrary(hInst);  
    }  

	return TRUE;
}


void CTraffic_MonitoringDlg::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	bExpand=!bExpand;
	// 以IDC_SEPARATE分割对话框
	ExtendDlg(IDC_SEPARATE,bExpand);

	CDialog::OnLButtonDblClk(nFlags, point);
}


/************************************************************************/
/* 展开、收缩对话框                                                     */
/************************************************************************/
void CTraffic_MonitoringDlg::ExtendDlg(int nResourceID, BOOL bExpand)
{
	// nResourceID为分割控件的ID（此处为picture控件）
	// 如果bExpand==TRUE，则扩展对话框为最大；
	// 否则缩小对话框，新的对话框的底部为资源nResourceID控件分割
	static CRect rcLarge;
	static CRect rcSmall;
	
	// 在开始时，保存对话框的最大和最小尺寸
	if(rcLarge.IsRectNull())
	{
		CRect rcLandmark;
		// 得到分割条的指针
		CWnd* pWndLandmark=GetDlgItem(nResourceID);
		ASSERT(pWndLandmark);
		
		// 得到对话框的尺寸
		GetWindowRect(rcLarge);
		// 得到分割条在对话框中的位置
		pWndLandmark->GetWindowRect(rcLandmark);
		
		rcSmall=rcLarge;
		rcSmall.bottom=rcLandmark.top;
	}
	
	if(bExpand)
	{
		// 扩展对话框：重新设置对话框到其原始（最大）尺寸（rcLarge）
		SetWindowPos(NULL,0,0,rcLarge.Width(),rcLarge.Height(),
			SWP_NOMOVE|SWP_NOZORDER);
	}
	else
	{
		// 缩小对话框：重新设置对话框到其最小尺寸（rcSmall）
		SetWindowPos(NULL,0,0,rcSmall.Width(),rcSmall.Height(),
			SWP_NOMOVE|SWP_NOZORDER);
	}
}




/*get data traffic from network adapter*/
void CTraffic_MonitoringDlg::GetTrafficInfo()
{
	//WriteLog("GetTrafficInfo()\n");
	DWORD db_traffic=0;  // 网速
	double total_flow=0;
	double dl_flow=0;
	double ul_flow=0;
	double temp=0;
	
	CString fmtSpeed = _T("%.1f Mbps"); // 网速格式化字符串
	CString fmtFlow = _T("%.2f GB");      // 流量格式化字符串

	CString fmt_dl_avg = _T("%.1f Mbps");
	CString fmt_ul_avg = _T("%.1f Mbps");
	CString fmt_dl_max = _T("%.1f Mbps");
	CString fmt_ul_max = _T("%.1f Mbps");


    // 下载
	db_traffic = netTraffic_download.GetTraffic(interfaceNum);// 获得1s的流量
	totaltraffic=totaltraffic+db_traffic;
	DL_totaltraffic=DL_totaltraffic+db_traffic;//单次流量累积
	temp=db_traffic*8/1000.0/1000.0;//字节->Mbit/s,得当前速度
	if(temp<1)//当前速度过小，显示小数点后3位
	{
		fmtSpeed = _T("%.3f Mbps");
		fmt_dl_avg = _T("%.3f Mbps");
		fmt_ul_avg = _T("%.3f Mbps");
		fmt_dl_max = _T("%.3f Mbps");
		fmt_ul_max = _T("%.3f Mbps");
	}

	if(vec_dl.size()<120)//120开全局
		vec_dl.push_back(temp);//保存当前速度到容器中，绘图需要
	else
	{
		it = vec_dl.begin();
		vec_dl.erase(it);
		vec_dl.push_back(temp);
	}

	if(count_sec>=1)
	{
		dl_temp_speed=dl_temp_speed+temp;//每秒的速度相加，速度累加
		DL_AVG=dl_temp_speed/count_sec;//总速度除时间，得平均速度
	}		
	else
		DL_AVG=temp;//第1秒时的速度，此时count_sec=0;

	DL_AVG_str.Format(fmt_dl_avg,DL_AVG);//格式化平均速度，以便在UI上更新
	m_downloadSpeed.Format(fmtSpeed,temp); // 格式化并保存网速	
	dl_flow=DL_totaltraffic/1000.0/1000.0/1000.0;
	m_downloadFlow.Format(fmtFlow,dl_flow );      
	if(temp>DL_max_speed)
		DL_max_speed=temp;
	DL_max_speed_str.Format(fmt_dl_max,DL_max_speed);
	

    // 上传
	db_traffic = netTraffic_upload.GetTraffic(interfaceNum);                    // 获得网速
	totaltraffic=totaltraffic+db_traffic;
	UL_totaltraffic=UL_totaltraffic+db_traffic;
	temp=db_traffic*8/1000.0/1000.0;   // current speed
	if(temp<1)
	{
		fmtSpeed = _T("%.3f Mbps");
	}
	if(vec_ul.size()<120)//120开全局
		vec_ul.push_back(temp);
	else
	{
		it = vec_ul.begin();
		vec_ul.erase(it);
		vec_ul.push_back(temp);
	}

	m_uploadSpeed.Format(fmtSpeed,temp);                // current speed
	ul_flow=UL_totaltraffic/1000.0/1000.0/1000.0;
	m_uploadFlow.Format(fmtFlow, ul_flow);        // 格式化并保存流量
	
	if(count_sec>=1)
	{
		ul_temp_speed=ul_temp_speed+temp;//每秒的速度相加，速度累加
		UL_AVG=ul_temp_speed/count_sec;//总速度除时间，得平均速度
	}		
	else
		UL_AVG=temp;//第1秒时的速度，此时count_sec=0;
	UL_AVG_str.Format(fmt_ul_avg,UL_AVG);//格式化平均速度，以便在UI上更新

	if(temp>UL_max_speed)
		UL_max_speed=temp;
	UL_max_speed_str.Format(fmt_ul_max,UL_max_speed);//max speed
                 
	//totaltraffic = netTraffic_total.GetInterfaceTotalTraffic(interfaceNum);     // 获得流量
	// 总的流量
	//db_traffic = netTraffic_total.GetTraffic(interfaceNum);      
	total_flow=totaltraffic/1000.0/1000.0/1000.0;
	m_totalFlow.Format(fmtFlow, total_flow);         // 格式化并保存流量
}

void CTraffic_MonitoringDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	
	CString str;
	
	CString fmtFlow = _T("%.2f GB"); 

	// 获取流量及网速信息，显示之
	if (0 == nIDEvent)
	{
		//WriteLog("OnTimer(0)\n");
		second++;
		count_sec++;
		if(second==60)
		{
			second=0;
			minute++;		
		}
		if(minute==60)
		{
			minute=0;
			hour++;
		}
		//时钟显示
		
		str.Format(_T("%02i:%02i:%02i"),hour,minute,second);
		GetDlgItem(IDC_STATIC1)->SetWindowText(str);
		
		//GetTrafficInfo();


		CRect rect;  
	    GetClientRect(&rect);//获得客户区大小
		CString contentTemp;
		contentTemp = m_downloadSpeed + _T("↓");
		GetDlgItem(IDC_STATIC_CONTENT1)->SetWindowText(contentTemp);
		contentTemp = m_uploadSpeed + _T("↑");
		GetDlgItem(IDC_STATIC_CONTENT2)->SetWindowText(contentTemp);

		contentTemp = m_downloadFlow + _T("↓");
		GetDlgItem(IDC_STATIC_CONTENT3)->SetWindowText(contentTemp);
		contentTemp = m_uploadFlow + _T("↑");
		GetDlgItem(IDC_STATIC_CONTENT4)->SetWindowText(contentTemp);

		contentTemp = DL_AVG_str + _T("↓");
		GetDlgItem(IDC_STATIC_DL_AVG)->SetWindowText(contentTemp);
		contentTemp = UL_AVG_str + _T("↑");
		GetDlgItem(IDC_STATIC_UL_AVG)->SetWindowText(contentTemp);

		contentTemp = DL_max_speed_str + _T("↓");
		GetDlgItem(IDC_STATIC_DL_MAX)->SetWindowText(contentTemp);
		contentTemp = UL_max_speed_str + _T("↑");
		GetDlgItem(IDC_STATIC_UL_MAX)->SetWindowText(contentTemp);

		if(count_sec==cycle&&auto_do==true)
		{
			if(save_csv)
			{
				char buf[100];
				sprintf(buf,"%.3f",UL_AVG);
				sscanf(buf,"%f",&UL_AVG);
				sprintf(buf,"%.3f",DL_AVG);
				sscanf(buf,"%f",&DL_AVG);
				oFile.open(savefilename, std::ios::app );
				oFile<<getTime(false)<<","<<mode<<","<<count_sec<<","<<selet_name<<","<<connection_type<<","<<network_type<<","<<signal_strength<<","<<transmission_type<<","<<UL_AVG<<","<<DL_AVG<<std::endl;
				//oFile<<"时间"模式+持续时间<<"项目"<<","<<"连接类型"<<","<<"网络类型"<<","<<"信号强度"<<","<<"传输类型"<<endl;
				oFile.close();
			}

			if(save_img)
			{
				filename=img_path+"\\"+getTime()+"_"+selet_name+"_"+connection_type+"_"+network_type+"_"+signal_strength+"_"+transmission_type+".jpg";
				hwnd2mat(filename.GetBuffer());
			}
			count_no++;
			count_total_dl=count_total_dl+DL_AVG;//统计的速度相加来求平均
			count_total_ul=count_total_ul+UL_AVG;
			OnBnClickedButton2();//重置秒表
		}
		

	}


	if (4 == nIDEvent)
	{
		CString contentTemp;
		GetTrafficInfo();
		contentTemp = m_totalFlow;
		GetDlgItem(IDC_STATIC_TOL_FLOW)->SetWindowText(contentTemp);
	}



	CDialog::OnTimer(nIDEvent);
}


//start Button 
void CTraffic_MonitoringDlg::OnBnClickedButton1()
{
	//开始按钮显示为播放
	if(start_flag==false)
	{
		//点开始后，进程开始，计算1秒流量，获得是第2秒的数据
		if(initUI())//判断周期是否合规，否则停下来
		{
			//GetTrafficInfo();
			GetDlgItem(IDC_STATIC_NUM)->SetWindowText("Detecting...");
			second=0;
			minute=0;
			hour=0;
			count_sec=0;
			DL_totaltraffic=0; 
			UL_totaltraffic=0; 
			DL_max_speed=0;
			UL_max_speed=0;
			DL_AVG=0;
			UL_AVG=0;
			dl_temp_speed=0;
			ul_temp_speed=0;
			//vec_dl.clear();
			//vec_ul.clear();

			SetTimer(0, 1000, NULL);
			
			HBITMAP hBmp=::LoadBitmap(AfxGetInstanceHandle(), 
			MAKEINTRESOURCE(IDB_BITMAP_STOP));   
			m_Btn.SetBitmap(hBmp);
			start_flag=true;
		}
		//m_pLineSerie->ClearSerie();
		//m_pLineSerie->AddPoints(DL_AVG,UL_AVG,5);
		
		
	}
	else//开始按钮显示为停止
	{
		initUI();
		if(save_csv&&auto_do==false)//防止自动模式退出后，仍添加数据
		{
			char buf[100];
			sprintf(buf,"%.3f",UL_AVG);
			sscanf(buf,"%f",&UL_AVG);
			sprintf(buf,"%.3f",DL_AVG);
			sscanf(buf,"%f",&DL_AVG);
			oFile.open(savefilename, std::ios::app );
			oFile<<getTime(false)<<","<<mode<<","<<count_sec<<","<<selet_name<<","<<connection_type<<","<<network_type<<","<<signal_strength<<","<<transmission_type<<","<<UL_AVG<<","<<DL_AVG<<std::endl;
			//oFile<<"时间"<<"+++++项目"<<","<<"连接类型"<<","<<"网络类型"<<","<<"信号强度"<<","<<"传输类型"<<endl;
			oFile.close();
		}

		if(save_img&&auto_do==false)
		{
			filename=img_path+"\\"+getTime()+"_"+selet_name+"_"+connection_type+"_"+network_type+"_"+signal_strength+"_"+transmission_type+".jpg";
			hwnd2mat(filename.GetBuffer());
		}
		count_no++;
		count_total_dl=count_total_dl+DL_AVG;//统计的速度相加来求平均
		count_total_ul=count_total_ul+UL_AVG;

		HBITMAP hBmp=::LoadBitmap(AfxGetInstanceHandle(), 
		MAKEINTRESOURCE(IDB_BITMAP_PLAY));   
		m_Btn.SetBitmap(hBmp);
		start_flag=false;
		

		KillTimer(0);	
		count_total_dl=0;//统计的速度相加来求平均
		count_total_ul=0;//置0，可以重新计算
		
		OnBnClickedButton2();//重置秒表
		count_no=0;
		second=0;
		minute=0;
		hour=0;
		count_sec=0;
		DL_totaltraffic=0; 
		UL_totaltraffic=0; 
		DL_max_speed=0;
		UL_max_speed=0;
		DL_AVG=0;
		UL_AVG=0;
		dl_temp_speed=0;
		ul_temp_speed=0;
	}
}

//Reset Button
void CTraffic_MonitoringDlg::OnBnClickedButton2()
{
	//KillTimer(0);
	GetDlgItem(IDC_STATIC1)->SetWindowText(_T("00:00:00"));
	CString temp;
	CString contentTemp;

	if(auto_do==false)
	{
		if(start_flag==false)
		{
			if(DL_AVG<1)
				temp = _T("%0.3fMbps↓,%0.3fMbps↑");
			else
				temp = _T("%0.1fMbps↓,%0.1fMbps↑");
			temp.Format(temp,DL_AVG,UL_AVG);
			contentTemp = temp;
			GetDlgItem(IDC_STATIC_NUM)->SetWindowText(contentTemp);
		}
	}
	else//自动统计类型情况
	{
		if(count_no==0)
			GetDlgItem(IDC_STATIC_NUM)->SetWindowText("No. 0");

		else
		{
			if(DL_AVG<1)
				temp.Format(_T("%0.3fMbps %0.3fMbps,No.%d"),count_total_dl/count_no,count_total_ul/count_no,count_no);
			else
				temp.Format(_T("%0.1fMbps %0.2fMbps,No.%d"),count_total_dl/count_no,count_total_ul/count_no,count_no);
			contentTemp = temp;
			GetDlgItem(IDC_STATIC_NUM)->SetWindowText(contentTemp);
		}

	}
	contentTemp = _T("0.0 Mbps↓  ");
	GetDlgItem(IDC_STATIC_CONTENT1)->SetWindowText(contentTemp);
	contentTemp = + _T("0.0 Mbps↑");
	GetDlgItem(IDC_STATIC_CONTENT2)->SetWindowText(contentTemp);

	contentTemp = _T("0.0 Mbps↓  ");
	GetDlgItem(IDC_STATIC_UL_MAX)->SetWindowText(contentTemp);
	contentTemp = + _T("0.0 Mbps↑");
	GetDlgItem(IDC_STATIC_DL_MAX)->SetWindowText(contentTemp);

	contentTemp = _T("0.0 Mbps↓  ");
	GetDlgItem(IDC_STATIC_UL_AVG)->SetWindowText(contentTemp);
	contentTemp = + _T("0.0 Mbps↑");
	GetDlgItem(IDC_STATIC_DL_AVG)->SetWindowText(contentTemp);

	contentTemp = _T("0.00 GB↓  ");
	GetDlgItem(IDC_STATIC_CONTENT3)->SetWindowText(contentTemp);
	contentTemp = _T("0.00 GB↑");
	GetDlgItem(IDC_STATIC_CONTENT4)->SetWindowText(contentTemp);
	second=0;
	minute=0;
	hour=0;
	count_sec=0;
	DL_totaltraffic=0; 
	UL_totaltraffic=0; 
	DL_max_speed=0;
	UL_max_speed=0;
	DL_AVG=0;
	UL_AVG=0;
	dl_temp_speed=0;
	ul_temp_speed=0;
}




//network adapter selection
void CTraffic_MonitoringDlg::OnCbnSelchangeCombo2()
{
	DWORD bandwidth;
	CString band,name;
	int netCount=0;

	interfaceNum = mycb.GetCurSel();
	bandwidth=netTraffic_download.GetInterfaceBandwidth((int)interfaceNum);
	netTraffic_total.GetNetworkInterfaceName(&name,(int)interfaceNum);
	band.Format(_T( "%u "),bandwidth);
	//band.Format(_T( "%u "),interfaceNum);
	band="Adapter："+name;//"带宽："+band+"M 网卡："+name;
	SetWindowText(band);
	band="";
}



//单选按钮监听
void CTraffic_MonitoringDlg::OnBnClickedRadioDut()
{
	GetDlgItem(IDC_EDIT_DUT)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_BEN)->EnableWindow(FALSE);
	select_benchmark = false;
}
void CTraffic_MonitoringDlg::OnBnClickedRadioBen()
{
	GetDlgItem(IDC_EDIT_DUT)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_BEN)->EnableWindow(TRUE);
	select_benchmark = true;
}
void CTraffic_MonitoringDlg::OnBnClickedRadioLan()
{
	connection_type="LAN";
}
void CTraffic_MonitoringDlg::OnBnClickedRadioWifi()
{
	connection_type="WIFI";
}
void CTraffic_MonitoringDlg::OnBnClickedRadioLte()
{
	network_type="LTE";
}
void CTraffic_MonitoringDlg::OnBnClickedRadio3g()
{
	network_type="3G";
}
void CTraffic_MonitoringDlg::OnBnClickedRadioGood()
{
	signal_strength="Good";
}
void CTraffic_MonitoringDlg::OnBnClickedRadioWeak()
{
	signal_strength="Weak";
}
void CTraffic_MonitoringDlg::OnBnClickedRadioUl()
{
	transmission_type="UL";
}
void CTraffic_MonitoringDlg::OnBnClickedRadioDl()
{
	transmission_type="DL";
}
void CTraffic_MonitoringDlg::OnBnClickedRadioUldl()
{
	transmission_type="ULDL";
}
//UI 
bool CTraffic_MonitoringDlg::initUI()
{
	//WriteLog("initUI()\n");
	CString no;
	GetDlgItem(IDC_EDIT_CYCLE)->GetWindowText(no);
	if(select_benchmark==false)
		GetDlgItem(IDC_EDIT_DUT)->GetWindowText(selet_name);
	else
		GetDlgItem(IDC_EDIT_BEN)->GetWindowText(selet_name);
	//MessageBox(selet_name);
	cycle = atoi(no);
	if(auto_do==true)
	{
		if (cycle > 3599 || cycle <= 0)
		{
			MessageBox("pls input No. between 1~3599!");
			return false;
		}
	}
	if (start_flag==false)
	{
		GetDlgItem(IDC_RADIO_DUT)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_BEN)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_WIFI)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_LAN)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_LTE)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_3G)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_UL)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_DL)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_ULDL)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_Good)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_Weak)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_CYCLE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_DUT)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_BEN)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO2)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_CSV)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_IMG)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_AUTO)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_MAN)->EnableWindow(FALSE);


				
		file_name = "..\\"+selet_name+ "_" + connection_type + "_" + network_type + "_" + signal_strength + "_" + transmission_type + "_";

			
		//MessageBox(file_name);
		if (BST_CHECKED == IsDlgButtonChecked(IDC_CHECK_CSV))
		{
			save_csv=true;
			if(app_first_run==true)
			{
				char   buffer[MAX_PATH];   
				getcwd(buffer, MAX_PATH); 
				CString temp="\\";
				//savefilename=buffer+temp+getTime(true)+".csv";
				savefilename=getTime(true)+".csv";
				//MessageBox(savefilename);
				oFile.open(savefilename, std::ios::app );
				//oFile<<getTime()<<selet_name<<","<<connection_type<<","<<network_type<<","<<signal_strength<<","<<transmission_type<<endl;
				oFile<<"Time"<<","<<"Mode"<<","<<"Lasted(s)"<<","<<"Project"<<","<<"Connection Type"<<","<<"Network Type"<<","<<"Signal"<<","<<"Transmission_type"<<","<<"UL Mbps"<<","<<"DL Mbps"<<std::endl;
				oFile.close();
				app_first_run=false;//只能修改一次
			}
					
		}
		if (BST_CHECKED == IsDlgButtonChecked(IDC_CHECK_IMG))
		{
			save_img=true;
			if(app_first_run_img==true)
			{
				img_path="img_"+getTime(true);
				mkdir(img_path); 
				app_first_run_img=false;
			}
		}
		else
			save_img=false;
	}
	else
	{
		GetDlgItem(IDC_RADIO_DUT)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_BEN)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_WIFI)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_LAN)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_LTE)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_3G)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_UL)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_DL)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_ULDL)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_Good)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_Weak)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_CYCLE)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_DUT)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_BEN)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO2)->EnableWindow(TRUE);
		if(app_first_run==true)
			GetDlgItem(IDC_CHECK_CSV)->EnableWindow(TRUE);//开始后，不能再修改，保证名字唯一
		GetDlgItem(IDC_CHECK_IMG)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_AUTO)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_MAN)->EnableWindow(TRUE);
	
	}
		
	return true;
}


void CTraffic_MonitoringDlg::OnBnClickedRadioAuto()
{
	GetDlgItem(IDC_EDIT_CYCLE)->EnableWindow(TRUE);//默认置灰周期
	auto_do=true;
	mode="Auto";
}

void CTraffic_MonitoringDlg::OnBnClickedRadioMan()
{
	GetDlgItem(IDC_EDIT_CYCLE)->EnableWindow(FALSE);//默认置灰周期
	auto_do=false;
	mode="Manually";
}
