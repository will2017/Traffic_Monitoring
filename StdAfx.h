// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__BB938EBE_6E6E_4AAE_A397_64B816C1C3EC__INCLUDED_)
#define AFX_STDAFX_H__BB938EBE_6E6E_4AAE_A397_64B816C1C3EC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

// 全局变量
extern HWND myMainWnd; 
extern UINT interfaceNum;  // 网卡号
extern NOTIFYICONDATA nid;  // 托盘结构体

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__BB938EBE_6E6E_4AAE_A397_64B816C1C3EC__INCLUDED_)
