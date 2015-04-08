// spviewDlg.cpp : implementation file
//

#include "stdafx.h"
#include "spview.h"
#include "spviewDlg.h"
#include "DlgFind.h"
#include <string.h>
//#include <winsock.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSpviewDlg dialog

CSpviewDlg::CSpviewDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSpviewDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSpviewDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_dCenter=1e9;
	m_dSpan=1e8;
	m_iPoint=1024;
	m_iCTPoint=512;
	m_iDiv=10;
	m_hThread=0;
	m_pCWinThread = (CWinThread *)0;
	m_bSpectra=FALSE;

// 	for (int i=0;i<4;i++)
// 	{
// 		m_Edit[i]=0;
// 	}
	m_ButtonApply=NULL;

}
CSpviewDlg::~CSpviewDlg(void)
 {
//	CloseHandle(m_hThread);
	for(int i=0;i<5;i++)
	{
	     delete m_Edit[i];
 	}
	 	delete m_Graph;
	 	delete m_ComboxSpan;
	 	delete m_ComboxCenter;
		delete m_CT;
	delete m_ButtonApply;
 }
void CSpviewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSpviewDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSpviewDlg, CDialog)
	//{{AFX_MSG_MAP(CSpviewDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_WM_KEYDOWN()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_DATARECV,OnRcvData)
	ON_MESSAGE(WM_THREADEND,OnThreadEnd)
	ON_BN_CLICKED(IDD_BUTTONAPPLY,OnButtonApply)
	ON_BN_CLICKED(IDC_BTSPECTRA,OnButtonSpectra)
	ON_CBN_SELCHANGE(IDC_COMBOXCEN,OnSelectchange)
	ON_CBN_SELCHANGE(IDC_COMBOXSPAN,OnSelectchange)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSpviewDlg message handlers

BOOL CSpviewDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	int cx = GetSystemMetrics(SM_CXSCREEN);
	int cy = GetSystemMetrics(SM_CYSCREEN);
	SetWindowPos(&wndTop,0,0, 800,600, SWP_NOZORDER);
// 
// 	CDlgFind dlg;
// 	dlg.DoModal();
	//Graph
	m_Spectra.Create("瀑布显示", 
	SS_BLACKFRAME | SS_BLACKRECT | WS_VISIBLE | WS_DLGFRAME,
		CRect(0,320,360,570),
		this,
		10001);	
    m_Spectra.ShowWindow(SW_HIDE);

	m_CT.Create("星座显示",
		SS_BLACKFRAME | SS_BLACKRECT | WS_VISIBLE | WS_DLGFRAME,
		CRect(360,320,680,570),
		this,
		10002);
	
     m_CT.ShowWindow(SW_HIDE);
	RECT rcGraph = {0, 20, 680, 570};
	m_Graph.Create("频谱显示", 
		SS_BLACKFRAME | SS_BLACKRECT | WS_VISIBLE | WS_DLGFRAME,
		rcGraph,
		this,
		10000);	

	
    //
	//edit
	RECT rcEdit[5]={
		{700,180,740,200},
		{700,220,740,240},
		{700,260,790,280},
		{700,300,790,320},
		{700,140,790,160}
	};
	for (int i=0;i<5;i++)
	{
		m_Edit[i]=new CEdit;
		m_Edit[i]->CreateEx(WS_EX_CLIENTEDGE,_T("EDIT"),NULL,
    WS_CHILD | WS_VISIBLE |WS_BORDER | WS_TABSTOP,rcEdit[i],this, 129+i,NULL);
	}
	//combox
	m_ComboxCenter=new CComboBox;
	m_ComboxCenter->Create(WS_CHILD | WS_VISIBLE |WS_BORDER | WS_TABSTOP|CBS_AUTOHSCROLL|CBS_DROPDOWNLIST,
		CRect(743,180,795,280),this,IDC_COMBOXCEN);
	m_ComboxSpan=new CComboBox;
    m_ComboxSpan->Create(WS_CHILD | WS_VISIBLE |WS_BORDER | WS_TABSTOP|CBS_AUTOHSCROLL|CBS_DROPDOWNLIST,
		CRect(743,220,795,320),this,IDC_COMBOXSPAN);
	CString strUnit[4]={"HZ","KHZ","MHZ","GHZ"};
	for (i=0;i<4;i++)
	{
		m_ComboxCenter->InsertString(i,strUnit[i]);
		m_ComboxSpan->InsertString(i,strUnit[i]);
	}
    m_ComboxCenter->SetCurSel(2);
	m_ComboxSpan->SetCurSel(2);
    //button
	m_ButtonApply=new CButton;
	m_ButtonApply->Create("OK",WS_CHILD|WS_VISIBLE|WM_COMMAND |BS_DEFPUSHBUTTON|BS_CENTER,CRect(700,340,780,360),this,IDD_BUTTONAPPLY);
	m_ButtonApply->SetWindowText("应用");
	m_ButtonSpectra=new CButton;
	m_ButtonSpectra->Create("Waterfall",WS_CHILD|WS_VISIBLE|WM_COMMAND |BS_PUSHBUTTON|BS_CENTER,CRect(700,400,780,420),this,IDC_BTSPECTRA);
    m_ButtonSpectra->SetWindowText("NO_EP");
	//static
	CString strStatic[5]={"CP","CT","SP","DIV","PT"};
	for (i=0;i<5;i++)
	{
		m_Static[i]=new CStatic;
		m_Static[i]->Create(strStatic[i],WS_CHILD|WS_VISIBLE|SS_LEFT,CRect(680,140+40*i,700,160+40*i),this);
	}
	//IP
	char szFileName[MAX_PATH]={0};
	GetCurrentDirectory(255, szFileName);
	strcat(szFileName,"\\INI.ini");
    //set CT INI
	m_CT.SetParam(GetPrivateProfileInt("CANSTELATIONLINE","HAVELINE",0,szFileName),GetPrivateProfileInt("CANSTELATIONLINE","RCT",2,szFileName));
    //
	((CStatic*)GetDlgItem(IDC_STATIC_IP))->SetWindowPos(&wndTop, 1,0,70,19,0);

	((CIPAddressCtrl*)GetDlgItem(IDC_IPADDRESS1))->SetAddress(GetPrivateProfileInt("IP","IP1",192,szFileName),
		GetPrivateProfileInt("IP","IP2",168,szFileName),GetPrivateProfileInt("IP","IP3",1,szFileName),GetPrivateProfileInt("IP","IP4",124,szFileName));
	((CIPAddressCtrl*)GetDlgItem(IDC_IPADDRESS1))->SetWindowPos(&wndTop, 70,0,300,19,0);
	//MODE
	((CEdit*)GetDlgItem(IDC_MODE))->SetWindowPos(&wndTop, 480,0,30,19,0);
	((CEdit*)GetDlgItem(IDC_MODE))->SetWindowText("0");

	char cTemp[255]={0};
	sprintf(cTemp,"%.2f",m_dCenter/(1E6));
	m_Edit[0]->SetWindowText(cTemp);
	sprintf(cTemp,"%.2f",m_dSpan/(1E6));
	m_Edit[1]->SetWindowText(cTemp);
	sprintf(cTemp,"%d",m_iDiv);
	m_Edit[2]->SetWindowText(cTemp);
	sprintf(cTemp,"%d",m_iPoint);
	m_Edit[3]->SetWindowText(cTemp);
	sprintf(cTemp,"%d",m_iCTPoint);
	m_Edit[4]->SetWindowText(cTemp);
	//
	// TODO: Add extra initialization here
	m_hMutex = CreateMutex(NULL,FALSE,NULL);


	SetWindowText(_T("BEACON"));
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSpviewDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSpviewDlg::OnPaint() 
{
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
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSpviewDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}
void CSpviewDlg::OnThreadEnd(WPARAM wParam,LPARAM lParam)
{
	m_hThread = 0;
}
void CSpviewDlg::OnRcvData(WPARAM wParam,LPARAM lParam)
{
	int i,recvlen=m_iPoint; 
	double *x,*y;
	double DataConstellation[2048]={0};
	x=new double[recvlen];
	y=new double[recvlen];
	double *pRecvdata=(double*)lParam;
	double df=m_dSpan/recvlen;
	WaitForSingleObject(m_hMutex,INFINITE);
	for (i=0;i<recvlen;i++)
	{
		x[i]=  m_dCenter - m_dSpan/2 + i*m_dSpan/(recvlen-1);
		y[i] = pRecvdata[i];
	}
	int j=0;
	if (m_bSpectra)
	{
		for (i=recvlen;i<recvlen+2*m_iCTPoint;++i)
			{
			DataConstellation[j]=pRecvdata[i];
			++j;
			}
	}

   ReleaseMutex(m_hMutex);
   
    m_Graph.SelectTrace(0);
	m_Graph.SetTraceMode(m_Graph.ClearWrite);
	m_Graph.SetTraceColor(0,m_Graph.ClearWrite);
    m_Graph.SetGraphParam(0,0,0,0,m_iDiv,m_dCenter,m_dSpan,100,100,100,recvlen,0);
 
    m_Graph.SetTraceData(x,y,recvlen);
	m_Graph.Invalidate();
    m_Graph.UpdateWindow();
     if (m_bSpectra)
    {	
	m_Spectra.SetGraphParam(0,0,0,0,m_iDiv,m_dCenter,m_dSpan,100,100);
	m_Spectra.SetTraceData(x,y,recvlen);
    m_Spectra.Invalidate();
	m_Spectra.UpdateWindow();
    

	m_CT.SetTraceData(DataConstellation,m_iCTPoint);
	m_CT.Invalidate();
	m_CT.UpdateWindow();
   }

	delete []x;delete []y;

 return;
}

UINT  CSpviewDlg::RecvProc(LPVOID lpParameter)
{
    HWND hwnd = ((RECVPARAM*)lpParameter)->hwnd;
    HANDLE hMutex =  ((RECVPARAM*)lpParameter)->hMutex;
    DWORD  dwIPT=((RECVPARAM*)lpParameter)->ip;
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD(1,1);
	
	err = WSAStartup(wVersionRequested,&wsaData);
	if ( err != 0)
	{
		return FALSE;
	}
	
	if ( LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1)
	{
		WSACleanup();
		return FALSE;
	}
	
	SOCKET temsock  = socket(AF_INET,SOCK_STREAM,0);
	
	SOCKADDR_IN m_addrSrv;
	m_addrSrv.sin_addr.S_un.S_addr =htonl(dwIPT);
	m_addrSrv.sin_family= AF_INET;
	m_addrSrv.sin_port = htons(6001);
	
//	bind(temsock ,(SOCKADDR*)&m_addrSrv,sizeof(SOCKADDR));
//	listen(temsock ,5);
//	int len=sizeof(SOCKADDR);
//	SOCKADDR_IN addrFrom;
	double *tempbuf=0;	double datay[4096]={0};
//     SOCKET sockConn;
	int i=0;
	while(TRUE)
	{


	while(TRUE)
	{
		//     sockConn= accept(temsock ,(SOCKADDR*)&addrFrom,&len);
		if(SOCKET_ERROR==connect(temsock,(SOCKADDR*)&m_addrSrv,sizeof(SOCKADDR)))
		{
			/*::MessageBox(hwnd,"连接超时，服务器可能未启动,请重试","警告",MB_OK);*/
			closesocket(temsock);
			tempbuf=0;
			Sleep(1000);
			++i;
			break;
		}
		
		char check[255]="0";
		char point[20]="point:";
		char center[20]="center:";
		char span[20]="span:";
		char mode[20]="mode:";
		
		char temp[255]={0};
		while(TRUE)
		{
			//temhwnd = ((RECVPARAM*)lpParameter)->hwnd;
			memset(check,0,255);
			
// 	        double cetem=CPublic::m_dPublicCenter;
// 			double sptem=CPublic::m_dPublicSpan;
// 			int    potem=CPublic::m_iPublicPoint;
// 			int    mdtem=CPublic::m_iPublicMode;
            WaitForSingleObject(hMutex,INFINITE);
			sprintf(temp,"%05d",CPublic::m_iPublicPoint);
			strcpy(check,point);
			strcat(check,temp);
			sprintf(temp,"%018f",CPublic::m_dPublicCenter);
			strcat(check,center);
			strcat(check,temp);
			sprintf(temp,"%018f",CPublic::m_dPublicSpan);
			strcat(check,span);
        	strcat(check,temp);
			sprintf(temp,"%02d",CPublic::m_iPublicMode);
			strcat(check,mode);
			strcat(check,temp);
            	ReleaseMutex(hMutex);
            int erro=send(temsock,check,255,0);

           Sleep(200);
			char recvbuf[4096*8];
			char recvCTbuf[2048*8];
			memset(recvbuf,0,4096*8);
			 int  recvlen = recv(temsock,recvbuf,4096*8,0);
/*			 int  recvCTlen = recv(temsock,recvCTbuf,2048*8,0);*/
			if(recvlen <=0 )
			{
				
				break;
			}
			tempbuf = (double *)recvbuf;
/*			int n=(recvlen+recvCTlen)/8-1;*/
			int n1=recvlen/8-1;
			WaitForSingleObject(hMutex,INFINITE);
			
			memcpy(datay,tempbuf,n1*sizeof(double));
			tempbuf = (double *)recvCTbuf;
/*            memcpy(datay+n1,recvCTbuf,(n-n1)*sizeof(double));*/
			datay[n1]='\0';
			ReleaseMutex(hMutex);
			::PostMessage(hwnd,WM_DATARECV,0,(LPARAM)datay);
		//	Sleep(20);
			
		}
	
	}
    if (i==1)	{
			break;}	
	}
	::PostMessage(hwnd,WM_THREADEND,0,0);
   AfxEndThread(1,TRUE);

   return 1;
}

void CSpviewDlg::OnButtonApply()
{  
	int iTemp;
	iTemp=GetDlgItemInt(IDD_EDITPOINT);
    if (iTemp<=0)
    {
		MessageBox("请输入正确的整数采样点！","警告");
		return;
    }
	m_iPoint=iTemp;
	iTemp=GetDlgItemInt(IDD_EDITCTPOINT);
    if (iTemp<=0)
    {
		MessageBox("请输入正确的整数采样点！","警告");
		return;
    }
	m_iCTPoint=iTemp;
	iTemp=GetDlgItemInt(IDD_EDITDIV);
	if (iTemp<=0)
	{
		MessageBox("请输入正确的DIV","警告");
		return;
	}
	m_iDiv=iTemp;
	iTemp=GetDlgItemInt(IDC_MODE);
	if (iTemp<0 )
	{
		MessageBox("请输入正确的MODE","警告");
		return;
	}
	m_iMode=iTemp;

    CString strCenter,strSpan;
    if(	!GetDlgItemText(IDD_EDITCENTER,strCenter)|| !GetDlgItemText(IDD_EDITSPAN,strSpan)) MessageBox("请输入正确的Center与Span！");
	
	double dCenter=strtodouble(strCenter);
	double dSpan=strtodouble(strSpan);
    int nIndex=m_ComboxCenter->GetCurSel();
	switch (nIndex)
	{
	case 0:
		m_dCenter=dCenter;
		break;
    case 1:
		m_dCenter=dCenter*1E3;
		break;
	case  2:
		m_dCenter=dCenter*1E6;
		break;
	case 3:
		m_dCenter=dCenter*1E9;
		break;
	default:
		break;

	}
	nIndex=m_ComboxSpan->GetCurSel();
	switch(nIndex)
	{
	case 0:
		m_dSpan=dSpan;
			break;
	case 1:
		m_dSpan=dSpan*1E3;
		break;
	case 2:
		m_dSpan=dSpan*1E6;
		break;
	case 3:
		m_dSpan=dSpan*1E9;
		break;
	default:
		break;

	}

   WaitForSingleObject(m_hMutex,INFINITE);
	CPublic::m_dPublicCenter=m_dCenter;
	CPublic::m_dPublicSpan=m_dSpan;
	CPublic::m_iPublicPoint=m_iPoint;
	CPublic::m_iPublicMode=m_iMode;
     ReleaseMutex(m_hMutex);
     DWORD dwIP;
	 ((CIPAddressCtrl*)GetDlgItem(IDC_IPADDRESS1))->GetAddress(dwIP);
	if (m_hThread==0)
	{ 
		RECVPARAM *pRecvParam = new RECVPARAM;
		pRecvParam->hwnd =m_hWnd;
		pRecvParam->hMutex=m_hMutex;
		pRecvParam->ip=dwIP;
		m_pCWinThread = AfxBeginThread(RecvProc,(LPVOID)pRecvParam);
		m_hThread = m_pCWinThread->m_hThread;
	}
 



   

	return;
//	CString 


}

double CSpviewDlg::strtodouble(CString str)
{
	char cStr[256]={0};
	memcpy(cStr,(LPTSTR)(LPCTSTR)str,256);
	char *p;
	double dstr=strtod(cStr,&p);
	return dstr;
}

void CSpviewDlg::OnSelectchange(WPARAM wParam,LPARAM lParam)
{
// 	int i;
// 	if (( i=lParam)==61748)
// 	{
// 		MessageBox("你赢了");
// 	}

	return;
}

BOOL CSpviewDlg::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (wParam==VK_RETURN)
	{
		OnButtonApply();
		
	}
	
	
		switch(LOWORD(wParam))
		{	
		case IDC_COMBOXSPAN:
		if (HIWORD(wParam)==1)	
//////////////////////////////
		break;
		case IDC_COMBOXCEN:
			if (HIWORD(wParam)==1)	
//////////////////
			break;
        
		default:
			break;
		}	
	
	return CDialog::OnCommand(wParam, lParam);
}

void CSpviewDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	if (m_hThread)
	{
		CloseHandle(m_hThread);
	}
	
// 	for(int i=0;i<4;i++)
// 	{
//     delete m_Edit[i];
// 	}
// 	delete m_Graph;
// 	delete m_ComboxSpan;
// 	delete m_ComboxCenter;
// 	delete m_ButtonApply;
 	CDialog::OnClose();
}

void CSpviewDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
// 	int i = nChar;
// 	if (nChar==VK_RETURN)
// 	{
// 		OnButtonApply();
// 		return ;
// 	}
	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CSpviewDlg::OnButton1() 
{
	// TODO: Add your control notification handler code here
	OnButtonApply();
}
void CSpviewDlg::OnButtonSpectra()
{

	if (m_bSpectra==FALSE)
	{
		::MoveWindow(m_Graph.GetSafeHwnd(),0,20,680,300,TRUE);
// 		m_Spectra.Create("频谱显示", 
// 			SS_BLACKFRAME | SS_BLACKRECT | WS_VISIBLE | WS_DLGFRAME,
// 			CRect(0,310,680,580),
// 			this,
// 			10001);	
		m_Graph.Invalidate(TRUE);
//		m_Graph.UpdateWindow();
		m_Spectra.SetGraphParam(0+1,0,0,0,m_iDiv,m_dCenter,m_dSpan,100,100);
		m_Spectra.SetGraphParam(0,0,0,0,m_iDiv,m_dCenter,m_dSpan,100,100);
		m_Spectra.ShowWindow(SW_SHOW);
		m_CT.ShowWindow(SW_SHOW);
		m_bSpectra=TRUE;
		((CButton *)GetDlgItem(IDC_BTSPECTRA))->SetWindowText(_T("YES_EP"));

	}
	else
	{
	//	m_Spectra.DestroyWindow();
    //    m_Spectra.ShowWindow(SW_HIDE);
		m_Spectra.ShowWindow(SW_HIDE);
		m_CT.ShowWindow(SW_HIDE);
        ::MoveWindow(m_Graph.GetSafeHwnd(),0,20,680,570,TRUE);
		m_Graph.Invalidate(TRUE);
		m_bSpectra=FALSE;
		((CButton *)GetDlgItem(IDC_BTSPECTRA))->SetWindowText(_T("NO_EP"));
	}
}

//DEL void CSpviewDlg::OnDestroy() 
//DEL {
//DEL 	CDialog::OnDestroy();
//DEL 	
//DEL 	// TODO: Add your message handler code here
//DEL 	
//DEL }
