// Traffic_MonitoringDlg.h : header file
//

#if !defined(AFX_TRAFFIC_MONITORINGDLG_H__2F6A7262_1023_4263_B563_0E4BDD1DA939__INCLUDED_)
#define AFX_TRAFFIC_MONITORINGDLG_H__2F6A7262_1023_4263_B563_0E4BDD1DA939__INCLUDED_

#include "MFNetTraffic.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define WM_TRAY WM_USER + 1  // �����Զ�����Ϣ  

#include "afxwin.h"



/////////////////////////////////////////////////////////////////////////////
// CTraffic_MonitoringDlg dialog

class CTraffic_MonitoringDlg : public CDialog
{
// Construction
public:
	DWORD nowExplorerID;                                 // ��ǰ��Դ�������Ľ���ID
	BOOL bExpand;                                        // ��־�Ƿ�չ����չ����
	void ExtendDlg(int nResourceID, BOOL bExpand);       // ��չ���������ں���
	BOOL SetWindowTransparency(int Byte);                // ���ô���͸����
	void InitTray();                 // ���̳�ʼ������
	CString m_totalFlow;             // ���������ϴ�����+����������
	CString m_totalSpeed;            // �����٣��ϴ�����+�������٣�
	CString m_uploadFlow;            // �ϴ�����
	CString m_downloadFlow;          // ��������
	CString m_uploadSpeed;           // �ϴ�����
	CString m_downloadSpeed;         // ��������
	void GetTrafficInfo();           // ��ȡ���ٺ������������Ϣ
	
	////////////////////////////////
	CFont m_Font; 
	CString DL_max_speed_str;  //��������ٶ�         
	CString UL_max_speed_str;         
	CString DL_AVG_str;           // ����ƽ������
	CString UL_AVG_str;         //
	void RegisterActiveX();
//	static void setWndTitle(CString title);
	
	// ����3��MFNetTraffic���󣬷ֱ����ڻ�ȡ�ܵġ����صĺ��ϴ������ټ�������Ϣ
	MFNetTraffic netTraffic_total;   
	MFNetTraffic netTraffic_download;
	MFNetTraffic netTraffic_upload;

	

	// �����˵����ڵĶ���
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
	//afx_msg LRESULT OnTrayNotify(WPARAM wParam, LPARAM lParam); // ������Ϣ��Ӧ���������� 
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
