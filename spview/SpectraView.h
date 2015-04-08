#if !defined(AFX_SPECTRAVIEW_H__FF95EE43_8B88_4362_9916_242DA6F73DCE__INCLUDED_)
#define AFX_SPECTRAVIEW_H__FF95EE43_8B88_4362_9916_242DA6F73DCE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GraphStatic.h"


/////////////////////////////////////////////////////////////////////////////
// CSpectraView window

class CSpectraView : public CStatic
{
// Construction
public:
	CSpectraView();
	typedef struct tagViewPoints{
		long x;
		long y;
		COLORREF color;
	}ViewPoints ;

// Attributes
public:
BOOL SetGraphParam(double fRef, int nRefUnit, double fAtten, int nAttenUnit, 
		double fDiv, double fCenter, double fSpan, double fRBW, double fVBW);
void SetTraceData(double *x, double *y, int count);
void ValueToDP(RECT rcView, double x, double y, long &nViewX, COLORREF &color);

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSpectraView)
	//}}AFX_VIRTUAL

// Implementation
public:
	int GetcurFrame();
	int m_curFrame;
	BOOL m_isMove;
	int GetRefLineIndex();
	int m_refindex;
	void SetRefLine(int index,BOOL bisMove);
	void DrawRefLine(CDC *dc,RECT rect,int index);
	void GetHzStr(double value, CString &str);

	
	virtual ~CSpectraView();

	// Generated message map functions
protected:
//	double rate;
	double m_xData1[DATA_MAXCOUNT];
	double m_yData1[DATA_MAXCOUNT];

//
	double m_fRef;	int m_nDataCount;
	int m_nRefUnit;
	double m_fAtten;
	int m_nAttenUnit;
	double m_fDiv;
	double m_fCenter;
	double m_fSpan;
	double m_fRBW;
	double m_fVBW;

	bool m_bParamChanged;

    const static  COLORREF ColorTableAll[144];
	CPen pnAxis;
	CFont fnText;
    CDC memdc;
	CBitmap memBmp;

	CDC transdc;
	CBitmap transBmp;
    bool isinitial;
	
//	int maxframe;
	//{{AFX_MSG(CSpectraView)
	afx_msg void OnPaint();	// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPECTRAVIEW_H__FF95EE43_8B88_4362_9916_242DA6F73DCE__INCLUDED_)
