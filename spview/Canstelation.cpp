// Canstelation.cpp : implementation file
//

#include "stdafx.h"
#include "spview.h"
#include "Canstelation.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCanstelation

CCanstelation::CCanstelation()
{
	memset(m_dData,0,2048*sizeof(double));
	m_bData=FALSE;
	m_iDataNum = 0;
	memset(m_ptData,0,sizeof(POINT)*1024);
	m_bLine=FALSE;
	m_RctWid=2;
	
}

CCanstelation::~CCanstelation()
{
}


BEGIN_MESSAGE_MAP(CCanstelation, CStatic)
	//{{AFX_MSG_MAP(CCanstelation)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCanstelation message handlers
BOOL CCanstelation::SetParam(BOOL bLine,int RetWid)
{
	m_bLine = bLine;
	m_RctWid = RetWid ;
	return TRUE;
}
void CCanstelation::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	RECT rcClient;
	GetClientRect(&rcClient);
	static CDC memdc;
	static CBitmap memBmp;
	static CBrush *pOldBr;
	static CPen *pOldPn;
	static CPen pnGrid;
    static CPen pnTrace;
	static CFont fnText;
	static CFont *pOldFn;
	static BOOL inital = FALSE;
	RECT rect[] = {
		{rcClient.left+20,rcClient.top+20,rcClient.right-20,rcClient.bottom-20},
		{10,rcClient.bottom-20,0,0},
		{rcClient.right-30,rcClient.bottom-20,0,0},
		{10,0,0,0},
		{rcClient.right/2-2,rcClient.bottom-20,0,0},
		{10,rcClient.bottom/2-8,0,0}
	};
   if (!inital)
   {
	   memdc.CreateCompatibleDC(&dc);
	   memBmp.CreateCompatibleBitmap(&dc,
		   rcClient.right-rcClient.left,
		   rcClient.bottom-rcClient.top);
	   memdc.SelectObject(&memBmp);
	   fnText.CreateFont(16, 0, 0, 0, 
		   FW_THIN, 
		   0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
		   CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FF_MODERN, 
			"Times New Roman");
	   pnGrid.CreatePen(PS_SOLID,1,RGB(140, 140, 140));
	   pnTrace.CreatePen(PS_SOLID,1,RGB(142,255,142));
	   inital =TRUE;
	   
	   
   }
   memdc.SetBkMode(TRANSPARENT);
   pOldBr = (CBrush *)memdc.SelectStockObject(BLACK_BRUSH );
   memdc.Rectangle(&rcClient);
   memdc.SelectStockObject(NULL_BRUSH);
   POINT pt0, pt1;
   pOldPn = memdc.SelectObject(&pnGrid);
   int i;
   for (i=0; i<=10; i++)
   {
	   pt0.x = rect[0].left + (rect[0].right - rect[0].left) * i / 10;
	   pt0.y = rect[0].top;
	   pt1.x = pt0.x;
	   pt1.y = rect[0].bottom;
	   memdc.MoveTo(pt0); 
	   memdc.LineTo(pt1);
	   
	   pt0.x = rect[0].left;
	   pt0.y = rect[0].top + (rect[0].bottom - rect[0].top) * i / 10;
	   pt1.x = rect[0].right;
	   pt1.y = pt0.y;
	   memdc.MoveTo(pt0); 
	   memdc.LineTo(pt1);
	}
   
   if (m_bData)
   {
	   POINT point[1024];
	   memset(point,0,sizeof(POINT)*1024);
	   ValueToDp(m_dData,m_iDataNum,point,rect[0]);
       CBrush brush(RGB(255,0,0));
	   int iLeftTemp,iTopTemp,iRightTemp,iBottomTemp;
	   int iBox = m_RctWid;
     for (i=0;i<m_iDataNum;++i)
     {
		 iLeftTemp = (((point[i].x-iBox) > rect[0].left) ? (point[i].x-iBox):(rect[0].left));
		 iTopTemp = (((point[i].y-iBox) > rect[0].top) ? (point[i].y-iBox):(rect[0].top));
		 iRightTemp =(((point[i].x+iBox) > rect[0].right)?(rect[0].right):(point[i].x+iBox));
		 iBottomTemp =(((point[i].y+iBox) > rect[0].bottom)?(rect[0].bottom):(point[i].y+iBox));
//		 memdc.SetPixel(point[i],RGB(255,0,0));
         memdc.FillRect(CRect(iLeftTemp,iTopTemp,iRightTemp,iBottomTemp),&brush);


     }		
	 if (m_bLine)
	 {	
		 memdc.SelectObject(&pnTrace);
		 memdc.Polyline(point,m_iDataNum);
	 }
   }
	memdc.SelectObject(fnText);
	memdc.SetTextColor(RGB(0, 230, 230));
	memdc.TextOut(rect[1].left,rect[1].top,"-1");
	memdc.TextOut(rect[2].left,rect[2].top,"1 /I");
	memdc.TextOut(rect[3].left,rect[3].top,"1 /Q");
	memdc.TextOut(rect[4].left,rect[4].top,"0");
	memdc.TextOut(rect[5].left,rect[5].top,"0");

   dc.BitBlt(rcClient.left, rcClient.top,
	   rcClient.right - rcClient.left, rcClient.bottom - rcClient.top,
	   &memdc,
	   0, 0,
	   SRCCOPY);

   
    memdc.SelectObject(pOldBr);
	memdc.SelectObject(pOldPn);
	memdc.SelectObject(pOldFn);
	// TODO: Add your message handler code here
	
	// Do not call CStatic::OnPaint() for painting messages
}

void CCanstelation::SetTraceData(double *recvdata,int datapoint)
{
	memcpy(m_dData,recvdata,datapoint*2*sizeof(double));
	m_bData=TRUE;
	m_iDataNum=datapoint;

}

void CCanstelation::ValueToDp(double *value,int datanum,POINT * datapoint,RECT rc)
{
	int i;
	double dPointX,dPointY;
	for (i=0;i<datanum;++i)
	{
	 if (value[2*i] < -1) dPointX = -1;
	 else if (value [2*i] > 1) dPointX =1;
     else dPointX = value[2*i];
     
	 if (value[2*i+1] < -1) dPointY = -1;
	 else if (value [2*i+1] > 1) dPointY =1;
     else dPointY = value[2*i+1];

     (datapoint[i]).x =(int) (rc.left+((dPointX+1)/2)*(rc.right-rc.left));
	 (datapoint[i]).y =(int)(rc.bottom-(((dPointY+1)/2)*(rc.bottom-rc.top)));
	}
}