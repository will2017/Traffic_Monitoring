// Traffic_Monitoring.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Traffic_Monitoring.h"
#include "Traffic_MonitoringDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTraffic_MonitoringApp

BEGIN_MESSAGE_MAP(CTraffic_MonitoringApp, CWinApp)
	//{{AFX_MSG_MAP(CTraffic_MonitoringApp)
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTraffic_MonitoringApp construction

CTraffic_MonitoringApp::CTraffic_MonitoringApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CTraffic_MonitoringApp object

CTraffic_MonitoringApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CTraffic_MonitoringApp initialization

BOOL CTraffic_MonitoringApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CTraffic_MonitoringDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
	}
	else if (nResponse == IDCANCEL)
	{
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
