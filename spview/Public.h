// Public.h: interface for the CPublic class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PUBLIC_H__77A6C10F_AA5E_4D52_86CD_1796C8F2E440__INCLUDED_)
#define AFX_PUBLIC_H__77A6C10F_AA5E_4D52_86CD_1796C8F2E440__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPublic  
{
public:
	CPublic();
	virtual ~CPublic();
public:
	static double m_dPublicCenter;
	static double m_dPublicSpan;
	static int    m_iPublicPoint;
	static int    m_iPublicMode;
};

#endif // !defined(AFX_PUBLIC_H__77A6C10F_AA5E_4D52_86CD_1796C8F2E440__INCLUDED_)
