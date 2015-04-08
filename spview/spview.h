// spview.h : main header file for the SPVIEW application
//

#if !defined(AFX_SPVIEW_H__F3ACEB1F_4D65_42D0_8AFF_33845D05DF76__INCLUDED_)
#define AFX_SPVIEW_H__F3ACEB1F_4D65_42D0_8AFF_33845D05DF76__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSpviewApp:
// See spview.cpp for the implementation of this class
//

class CSpviewApp : public CWinApp
{
public:
	CSpviewApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSpviewApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSpviewApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPVIEW_H__F3ACEB1F_4D65_42D0_8AFF_33845D05DF76__INCLUDED_)
