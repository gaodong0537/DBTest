#if !defined(AFX_CANSTELATION_H__BB568589_2B5D_4E5E_ADDB_C5A15CFAB95C__INCLUDED_)
#define AFX_CANSTELATION_H__BB568589_2B5D_4E5E_ADDB_C5A15CFAB95C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Canstelation.h : header file
//
#define  MAXDATALEN 2048
/////////////////////////////////////////////////////////////////////////////
// CCanstelation window

class CCanstelation : public CStatic
{
// Construction
public:
	CCanstelation();

// Attributes
public:
private:
	double m_dData[2048];
	BOOL m_bData;
	BOOL m_bLine;
	int m_RctWid;
	int m_iDataNum;
	POINT m_ptData[1024];
    void ValueToDp(double*,int,POINT *,RECT);
	
// Operations
public:

  void SetTraceData(double *,int );
  BOOL SetParam(BOOL,int);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCanstelation)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCanstelation();
   
	// Generated message map functions
protected:
	//{{AFX_MSG(CCanstelation)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CANSTELATION_H__BB568589_2B5D_4E5E_ADDB_C5A15CFAB95C__INCLUDED_)
