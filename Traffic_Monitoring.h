// Traffic_Monitoring.h : main header file for the TRAFFIC_MONITORING application
//

#if !defined(AFX_TRAFFIC_MONITORING_H__FD393845_94D4_4F20_8609_E332A2747A8F__INCLUDED_)
#define AFX_TRAFFIC_MONITORING_H__FD393845_94D4_4F20_8609_E332A2747A8F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CTraffic_MonitoringApp:
// See Traffic_Monitoring.cpp for the implementation of this class
//

class CTraffic_MonitoringApp : public CWinApp
{
public:
	CTraffic_MonitoringApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTraffic_MonitoringApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CTraffic_MonitoringApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRAFFIC_MONITORING_H__FD393845_94D4_4F20_8609_E332A2747A8F__INCLUDED_)
