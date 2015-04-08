// Public.cpp: implementation of the CPublic class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "spview.h"
#include "Public.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
double CPublic::m_dPublicCenter=0;
double CPublic::m_dPublicSpan=0;
int CPublic::m_iPublicPoint=0;
int CPublic::m_iPublicMode=0;

CPublic::CPublic()
{

}

CPublic::~CPublic()
{

}
