; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CPopDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "Traffic_Monitoring.h"

ClassCount=3
Class1=CTraffic_MonitoringApp
Class2=CTraffic_MonitoringDlg

ResourceCount=4
Resource2=IDD_TRAFFIC_MONITORING_DIALOG
Resource1=IDR_MAINFRAME
Class3=CPopDlg
Resource3=IDD_DIALOG_POP
Resource4=IDR_MENU1

[CLS:CTraffic_MonitoringApp]
Type=0
HeaderFile=Traffic_Monitoring.h
ImplementationFile=Traffic_Monitoring.cpp
Filter=N

[CLS:CTraffic_MonitoringDlg]
Type=0
HeaderFile=Traffic_MonitoringDlg.h
ImplementationFile=Traffic_MonitoringDlg.cpp
Filter=W
BaseClass=CDialog
VirtualFilter=dWC
LastObject=CTraffic_MonitoringDlg



[DLG:IDD_TRAFFIC_MONITORING_DIALOG]
Type=1
Class=CTraffic_MonitoringDlg
ControlCount=10
Control1=IDC_STATIC_CONTENT1,static,1342308352
Control2=IDC_SEPARATE,static,1073741828
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC_CONTENT3,static,1342308352
Control7=IDC_STATIC_CONTENT5,static,1342308352
Control8=IDC_STATIC_CONTENT2,static,1342308352
Control9=IDC_STATIC_CONTENT4,static,1342308352
Control10=IDC_STATIC_CONTENT6,static,1342308352

[DLG:IDD_DIALOG_POP]
Type=1
Class=CPopDlg
ControlCount=5
Control1=IDC_BTN_EXIT,button,1342242816
Control2=IDC_SLIDER_TRANSPARENCY,msctls_trackbar32,1342242840
Control3=IDC_BTN_TYPE,button,1342242816
Control4=IDC_BTN_DELTRAY,button,1342242816
Control5=IDC_STATIC,static,1342308352

[CLS:CPopDlg]
Type=0
HeaderFile=PopDlg.h
ImplementationFile=PopDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CPopDlg
VirtualFilter=dWC

[MNU:IDR_MENU1]
Type=1
Class=?
CommandCount=0

