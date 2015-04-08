; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CCanstelation
LastTemplate=CStatic
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "spview.h"

ClassCount=5
Class1=CSpviewApp
Class2=CSpviewDlg
Class3=CAboutDlg

ResourceCount=4
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_SPVIEW_DIALOG
Class4=CDlgFind
Class5=CCanstelation
Resource4=IDD_DLGFIND

[CLS:CSpviewApp]
Type=0
HeaderFile=spview.h
ImplementationFile=spview.cpp
Filter=N

[CLS:CSpviewDlg]
Type=0
HeaderFile=spviewDlg.h
ImplementationFile=spviewDlg.cpp
Filter=D
LastObject=CSpviewDlg
BaseClass=CDialog
VirtualFilter=dWC

[CLS:CAboutDlg]
Type=0
HeaderFile=spviewDlg.h
ImplementationFile=spviewDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_SPVIEW_DIALOG]
Type=1
Class=CSpviewDlg
ControlCount=4
Control1=IDC_IPADDRESS1,SysIPAddress32,1342242816
Control2=IDC_BUTTON1,button,1073742593
Control3=IDC_STATIC_IP,static,1342308353
Control4=IDC_MODE,edit,1350631552

[DLG:IDD_DLGFIND]
Type=1
Class=CDlgFind
ControlCount=4
Control1=IDC_STATIC,button,1342177287
Control2=IDC_BTOK,button,1342242816
Control3=IDC_BTCEL,button,1342242816
Control4=IDC_TABSEL,SysTabControl32,1342177280

[CLS:CDlgFind]
Type=0
HeaderFile=DlgFind.h
ImplementationFile=DlgFind.cpp
BaseClass=CDialog
Filter=D
LastObject=CDlgFind

[CLS:CCanstelation]
Type=0
HeaderFile=Canstelation.h
ImplementationFile=Canstelation.cpp
BaseClass=CStatic
Filter=W
VirtualFilter=WC

