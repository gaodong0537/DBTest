// spviewDlg.h : header file
//

#if !defined(AFX_SPVIEWDLG_H__58110AEB_83E5_4538_A21A_AE73930C6520__INCLUDED_)
#define AFX_SPVIEWDLG_H__58110AEB_83E5_4538_A21A_AE73930C6520__INCLUDED_

#define  WM_DATARECV WM_USER+1
#define  WM_THREADEND WM_USER+2
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <winsock.h>
#include "GraphStatic.h"
#include "SpectraView.h"
#include "Canstelation.h"
#include "Public.h"
#pragma comment(lib,"ws2_32.lib")
/////////////////////////////////////////////////////////////////////////////
// CSpviewDlg dialog
typedef struct _RECVPARAM
{
	HWND hwnd;
	HANDLE hMutex;
	DWORD ip;
}RECVPARAM;

class CSpviewDlg : public CDialog
{
// Construction
public:
	CSpviewDlg(CWnd* pParent = NULL);	// standard constructor
	 virtual ~CSpviewDlg();
    static UINT  RecvProc(LPVOID lpParameter);
    
	HANDLE m_hMutex;
	double m_dCenter;
	double m_dSpan;
	int m_iDiv;
	int m_iPoint;
	int m_iCTPoint;
	int m_iMode;
	CGraphStatic m_Graph;
	CCanstelation m_CT;
	CEdit *m_Edit[5];
	CStatic *m_Static[5];
	CButton *m_ButtonApply;
    CSpectraView m_Spectra;
	CButton *m_ButtonSpectra;
	CComboBox *m_ComboxCenter;
	CComboBox *m_ComboxSpan;
	HANDLE m_hThread;
	CWinThread *m_pCWinThread;
	BOOL m_bSpectra;
// Dialog Data
	//{{AFX_DATA(CSpviewDlg)
	enum { IDD = IDD_SPVIEW_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSpviewDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
    double strtodouble(CString str);
	// Generated message map functions
	//{{AFX_MSG(CSpviewDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnButton1();
	//}}AFX_MSG
	afx_msg void OnRcvData(WPARAM wParam,LPARAM lParam);
	afx_msg void OnThreadEnd(WPARAM wParam,LPARAM lParam);
	afx_msg void OnButtonApply();
	afx_msg void OnButtonSpectra();
	afx_msg void OnSelectchange(WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPVIEWDLG_H__58110AEB_83E5_4538_A21A_AE73930C6520__INCLUDED_)
