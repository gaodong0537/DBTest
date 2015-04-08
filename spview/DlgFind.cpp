// DlgFind.cpp : implementation file
//

#include "stdafx.h"
#include "spview.h"
#include "DlgFind.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgFind dialog


CDlgFind::CDlgFind(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgFind::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgFind)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgFind::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgFind)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgFind, CDialog)
	//{{AFX_MSG_MAP(CDlgFind)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgFind message handlers
