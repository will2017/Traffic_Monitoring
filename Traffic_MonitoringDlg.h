// Traffic_MonitoringDlg.h : header file
//

#if !defined(AFX_TRAFFIC_MONITORINGDLG_H__2F6A7262_1023_4263_B563_0E4BDD1DA939__INCLUDED_)
#define AFX_TRAFFIC_MONITORINGDLG_H__2F6A7262_1023_4263_B563_0E4BDD1DA939__INCLUDED_

#include "MFNetTraffic.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define WM_TRAY WM_USER + 1  // 托盘自定义消息  

#include "afxwin.h"



/////////////////////////////////////////////////////////////////////////////
// CTraffic_MonitoringDlg dialog

class CTraffic_MonitoringDlg : public CDialog
{
// Construction
public:
	DWORD nowExplorerID;                                 // 当前资源管理器的进程ID
	BOOL bExpand;                                        // 标志是否展开扩展窗口
	void ExtendDlg(int nResourceID, BOOL bExpand);       // 扩展与收缩窗口函数
	BOOL SetWindowTransparency(int Byte);                // 设置窗口透明度
	void InitTray();                 // 托盘初始化函数
	CString m_totalFlow;             // 总流量（上传流量+下载流量）
	CString m_totalSpeed;            // 总网速（上传网速+下载网速）
	CString m_uploadFlow;            // 上传流量
	CString m_downloadFlow;          // 下载流量
	CString m_uploadSpeed;           // 上传网速
	CString m_downloadSpeed;         // 下载网速
	void GetTrafficInfo();           // 获取网速和流量的相关信息
	
	////////////////////////////////
	CFont m_Font; 
	CString DL_max_speed_str;  //最大下载速度         
	CString UL_max_speed_str;         
	CString DL_AVG_str;           // 下载平均网速
	CString UL_AVG_str;         //
	void RegisterActiveX();
//	static void setWndTitle(CString title);
	
	// 定义3个MFNetTraffic对象，分别由于获取总的、下载的和上传的网速即流量信息
	MFNetTraffic netTraffic_total;   
	MFNetTraffic netTraffic_download;
	MFNetTraffic netTraffic_upload;

	

	// 弹出菜单窗口的对象
	//CPopDlg *popDlg;

	CTraffic_MonitoringDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CTraffic_MonitoringDlg)
	enum { IDD = IDD_TRAFFIC_MONITORING_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTraffic_MonitoringDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;


	// Generated message map functions
	//{{AFX_MSG(CTraffic_MonitoringDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTimer(UINT nIDEvent);
	//afx_msg LRESULT OnTrayNotify(WPARAM wParam, LPARAM lParam); // 托盘消息响应函数的声明 
	//afx_msg void OnClose();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	CStatic m_static1;
	CButton m_Btn;
	CButton my_But_Reset;
	CComboBox mycb;
	afx_msg void OnCbnSelchangeCombo2();

	bool initUI();

	//double m_TeeChartArray[200];


	//CChartLineSerie* m_pLineSerie;
	afx_msg void OnBnClickedRadioDut();
	afx_msg void OnBnClickedRadioBen();
	afx_msg void OnBnClickedRadioLan();
	afx_msg void OnBnClickedRadioWifi();
	afx_msg void OnBnClickedRadioLte();
	afx_msg void OnBnClickedRadio3g();
	afx_msg void OnBnClickedRadioGood();
	afx_msg void OnBnClickedRadioWeak();
	afx_msg void OnBnClickedRadioUl();
	afx_msg void OnBnClickedRadioDl();
	afx_msg void OnBnClickedRadioUldl();
	afx_msg void OnBnClickedButton3();

	afx_msg void OnBnClickedButtonRetPic();
	afx_msg void OnBnClickedRadioAuto();
	afx_msg void OnBnClickedRadioMan();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRAFFIC_MONITORINGDLG_H__2F6A7262_1023_4263_B563_0E4BDD1DA939__INCLUDED_)
