// SpectraView.cpp : implementation file
//

#include "stdafx.h"

#include "SpectraView.h"
#include <fstream.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSpectraView

CSpectraView::CSpectraView()
{

    memset(m_xData1, 0, sizeof(m_xData1));
	memset(m_yData1, 0, sizeof(m_yData1));
	m_fAtten = 0;
	m_fRef = 0;
	m_fDiv = 10;
	m_fCenter = 100000;
	m_fSpan = 1000000;
	m_fRBW = 100;
	m_fVBW = 100;
	m_nRefUnit = 0;
	m_nAttenUnit = 0;

	m_refindex = -1;
	m_nDataCount = 0;
	isinitial=false;
	m_bParamChanged = true;
	m_isMove = TRUE;
	m_curFrame = 1;
}

const COLORREF	CSpectraView::ColorTableAll[144] = {
		0x000000FF, 0x000010FF, 0x000020FF, 0x000030FF, 0x000040FF, 0x000050FF, 0x000060FF, 0x000070FF,
		0x000080FF, 0x000084FF, 0x000088FF, 0x00008CFF, 0x000090FF, 0x000094FF, 0x000098FF, 0x00009CFF,
		0x0000A0FF, 0x0000A4FF, 0x0000A8FF, 0x0000ACFF, 0x0000B0FF, 0x0000B4FF, 0x0000B8FF, 0x0000BCFF,
		0x0000C0FF, 0x0000C2FF, 0x0000C4FF, 0x0000C6FF, 0x0000C8FF, 0x0000CAFF, 0x0000CCFF, 0x0000CEFF,
		0x0000D0FF, 0x0000D2FF, 0x0000D4FF, 0x0000D6FF, 0x0000D8FF, 0x0000DAFF, 0x0000DCFF, 0x0000DEFF,
		0x0000E0FF, 0x0000E2FF, 0x0000E4FF, 0x0000E6FF, 0x0000E8FF, 0x0000EAFF, 0x0000ECFF, 0x0000EEFF,
		0x0000F0FF, 0x0000F2FF, 0x0000F4FF, 0x0000F6FF, 0x0000F8FF, 0x0000FAFF, 0x0000FCFF, 0x0000FEFF,
		0x0000FFFF, 0x0008FFF7, 0x0010FFEF, 0x0018FFE7, 0x0020FFDF, 0x0028FFD7, 0x0030FFCF, 0x0038FFC7,
		0x0040FFBF, 0x0048FFB7, 0x0050FFAF, 0x0058FFA7, 0x0060FF9F, 0x0068FF97, 0x0070FF8F, 0x0078FF87,
		0x0080FF7F, 0x0088FF77, 0x0090FF6F, 0x0098FF67, 0x00A0FF5F, 0x00A8FF57, 0x00B0FF4F, 0x00B8FF47,
		0x00C0FF3F, 0x00C8FF37, 0x00D0FF2F, 0x00D8FF27, 0x00E0FF1F, 0x00E8FF17, 0x00F0FF0F, 0x00F8FF07,
		0x00FFFF00, 0x00FFFD00, 0x00FFFB00, 0x00FFF900, 0x00FFF700, 0x00FFF500, 0x00FFF300, 0x00FFF100,
		0x00FFEF00, 0x00FFED00, 0x00FFEB00, 0x00FFE900, 0x00FFE700, 0x00FFE500, 0x00FFE300, 0x00FFE100,
		0x00FFDF00, 0x00FFDD00, 0x00FFDB00, 0x00FFD900, 0x00FFD700, 0x00FFD500, 0x00FFD300, 0x00FFD100,
		0x00FFCF00, 0x00FFCD00, 0x00FFCB00, 0x00FFC900, 0x00FFC700, 0x00FFC500, 0x00FFC300, 0x00FFC100,
		0x00FFBF00, 0x00FFBB00, 0x00FFB700, 0x00FFB300, 0x00FFAF00, 0x00FFAB00, 0x00FFA700, 0x00FFA300,
		0x00FF9F00, 0x00FF9B00, 0x00FF9700, 0x00FF9300, 0x00FF8F00, 0x00FF8B00, 0x00FF8700, 0x00FF8300,
		0x00FF7F00, 0x00FF6F00, 0x00FF5F00, 0x00FF4F00, 0x00FF3F00, 0x00FF2F00, 0x00FF1F00, 0x00FF0F00,
};

CSpectraView::~CSpectraView()
{
}


BEGIN_MESSAGE_MAP(CSpectraView, CStatic)
	//{{AFX_MSG_MAP(CSpectraView)
		ON_WM_PAINT()	// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSpectraView message handlers


void CSpectraView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
    RECT rcClient;
		
	GetClientRect(&rcClient);

//	int nTextHeight = (rcClient.bottom - rcClient.top) / 16;
	int nTextHeight = 20;

	RECT rect[] = {
		{60+3, rcClient.top+5, rcClient.right - 5 -3, rcClient.bottom - nTextHeight - 5},
		{rect[0].left - 3, rect[0].top}, //top blocks, right align
		{rect[0].left - 3, rect[0].bottom - 2*nTextHeight}, //bottom blocks, right align
		{rect[0].left - 3, rect[1].top + 2*nTextHeight}, //top ref, right align
		{rect[0].left - 3, rect[2].top - 2*nTextHeight}, //bottom ref, right align
		{rcClient.left + 25, rect[3].top + 2*nTextHeight, rect[0].left - 5, rect[4].top}, //Color bar
		{rect[0].left, rect[0].bottom + 2}, //center, left align
		{rect[0].right, rect[0].bottom + 2}, //span, right align
		{0}
	};

	//测试
//	m_fCenter = m_xData1[m_nDataCount/2];
//	m_fSpan = m_xData1[m_nDataCount-1] - m_xData1[0];
	//测试

	//初始化（第一次）
	if(!isinitial)
	{
		memdc.CreateCompatibleDC(&dc);
		memBmp.CreateCompatibleBitmap(&dc, 
			rcClient.right - rcClient.left,
			rcClient.bottom - rcClient.top);
		
		memdc.SelectObject(&memBmp);

		pnAxis.CreatePen(PS_SOLID, 1, RGB(200, 200, 200));
		fnText.CreateFont(nTextHeight, 0, 0, 0, FW_THIN, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FF_MODERN, "Arial");

		transdc.CreateCompatibleDC(&dc);
		
		transBmp.CreateCompatibleBitmap(&dc,rect[0].right - rect[0].left,
			rect[0].bottom - rect[0].top-2);
		transdc.SelectObject(&transBmp);
		
		transdc.BitBlt(0, 0, 
			rect[0].right - rect[0].left, rect[0].bottom - rect[0].top-2,
			&memdc,rect[0].left+1,
			rect[0].top+1,
			SRCCOPY);
		isinitial = true;
	}
	//参数改变，清空旧的绘图区，重绘背景
	if (m_bParamChanged) 
	{
		CBrush *pOldBr;
		CPen *pOldPn;
		
		CFont *pOldFn;
		pOldFn = memdc.SelectObject(&fnText);
		
		memdc.SetTextColor(RGB(230, 230, 230));
		memdc.SetBkMode(TRANSPARENT);
		
		//背景
		pOldBr = (CBrush *)memdc.SelectStockObject(BLACK_BRUSH );
		memdc.Rectangle(&rcClient);
		
		pOldPn = memdc.SelectObject(&pnAxis);
		memdc.Rectangle(&rect[0]);
		
		//显示左边的颜色条
		for (int m=rect[5].top; m <= rect[5].bottom; m++)
		{
			CPen pnColorBar;
			int nColorIndex = (m - rect[5].top)*143/(rect[5].bottom - rect[5].top);
			pnColorBar.CreatePen(PS_SOLID, 1, ColorTableAll[nColorIndex]);
			CPen *pOldPn = memdc.SelectObject(&pnColorBar);
			memdc.MoveTo(rect[5].left, m);
			memdc.LineTo(rect[5].right, m);
			memdc.SelectObject(pOldPn);
		}
		memdc.MoveTo(rcClient.right,rcClient.top);
        memdc.LineTo(rcClient.right,rcClient.bottom);
		//显示文字
		int nBlocks = rect[0].top - rect[0].bottom;
		CString str;
		CString strUnit;
		
		//blocks
		str.Format("%d", nBlocks);
		memdc.SetTextAlign(TA_RIGHT);
		memdc.TextOut(rect[1].left, rect[1].top, str);
		memdc.TextOut(rect[1].left, rect[1].top + nTextHeight, "block");
		str.Format("%d", nBlocks);
		memdc.SetTextAlign(TA_RIGHT);
		memdc.TextOut(rect[2].left, rect[2].top, "0");
		memdc.TextOut(rect[2].left, rect[2].top + nTextHeight, "block");
		
		//REF
		switch(m_nRefUnit)
		{
		case 0:
			strUnit = "dBm";
			break;
		case 1:
			strUnit = "dBv";
			break;
		case 2:
			strUnit = "dBmv";
			break;
		case 3:
			strUnit = "dBuv";
			break;
		case 4:
			strUnit = "mv";
			break;
		case 5:
			strUnit = "w";
			break;
		default:
			strUnit = "dBm";
		}
		str.Format("%1.0f", m_fRef);
		memdc.TextOut(rect[3].left, rect[3].top, str);
		memdc.TextOut(rect[3].left, rect[3].top + nTextHeight, strUnit);
		str.Format("%1.0f", m_fRef - 10*m_fDiv);
		memdc.TextOut(rect[4].left, rect[4].top, str);
		memdc.TextOut(rect[4].left, rect[4].top + nTextHeight, strUnit);
		
		//CF
		memdc.SetTextAlign(TA_LEFT);
		GetHzStr(m_fCenter, str);
		str = "Center " + str;
		memdc.TextOut(rect[6].left, rect[6].top, str);
		//SPAN
		memdc.SetTextAlign(TA_RIGHT);
		GetHzStr(m_fSpan, str);
		str = "Span " + str;
		memdc.TextOut(rect[7].left, rect[7].top, str);
		
		transdc.BitBlt(0, 0, 
			rect[0].right - rect[0].left, rect[0].bottom - rect[0].top-2,
			&memdc,rect[0].left+1,
			rect[0].top+1,
			SRCCOPY);
		m_bParamChanged = false;
	}
	
	//绘图区向上移动1像素
	if(m_isMove)
	{
		transdc.BitBlt(0,0,
		rect[0].right - rect[0].left, rect[0].bottom - rect[0].top-3,
		&transdc,
		0,
		1,
		SRCCOPY);
		m_curFrame--;
		if(m_curFrame<-208)
			m_curFrame = -208;
		m_refindex  = -1;

	}
	m_isMove = TRUE;
	
	//绘制新的光谱线
    ViewPoints pts[DATA_MAXCOUNT];
	RECT rcNewLine = {0, rect[0].bottom - rect[0].top-3, 
		rect[0].right - rect[0].left, rect[0].bottom - rect[0].top-3};
	for (int i = 0; i < m_nDataCount; i++)
	{
		ValueToDP(rcNewLine, m_xData1[i], m_yData1[i], pts[i].x, pts[i].color);
	}
	transdc.MoveTo(pts[0].x, rcNewLine.bottom);
	if (m_nDataCount < rect[0].right - rect[0].left) 
	{
		for (int j = 0; j < m_nDataCount; j++)
		{	   
			transdc.SetPixel(pts[j].x, rcNewLine.bottom, pts[j].color);
			transdc.SetPixel(pts[j].x+1, rcNewLine.bottom, pts[j].color);
			transdc.SetPixel(pts[j].x+2, rcNewLine.bottom, pts[j].color);
			transdc.SetPixel(pts[j].x+3, rcNewLine.bottom, pts[j].color); 
		}	
	}else{
		for (int j = 0; j < m_nDataCount; j++)
		{	   
			transdc.SetPixel(pts[j].x, rcNewLine.bottom, pts[j].color);
		}	
	}	


	
	//把绘制图形拷贝到后备显示区
	memdc.BitBlt(rect[0].left+1,rect[0].top+1,
		rect[0].right - rect[0].left-2, rect[0].bottom - rect[0].top-2,
		&transdc,
		0,
		0,
		SRCCOPY);

	//把后备显示区的显示内容输出到图形界面
	dc.BitBlt(rcClient.left, rcClient.top,
		rcClient.right - rcClient.left, rcClient.bottom - rcClient.top,
		&memdc,
		0,
		0,
		SRCCOPY);
	DrawRefLine(&dc,rect[0],m_refindex);//m_refindex
	
}

BOOL CSpectraView::SetGraphParam(double fRef, int nRefUnit, double fAtten, int nAttenUnit, 
								 double fDiv, double fCenter, double fSpan, double fRBW, double fVBW)
{
	//设置图像参数
    //因单片机将衰减20和30互换了，故通过上层将其换回来
//	if(fAtten==30)
//	{
//		fAtten=20;
//	}else if(fAtten==20)
//	{
//		fAtten=30;
//	}

	if (m_fRef != fRef) 
	{
		m_fRef = fRef;
		m_bParamChanged = true;
	}
	if (m_nRefUnit != nRefUnit) 
	{
		m_nRefUnit = nRefUnit;
		m_bParamChanged = true;
	}
	if (m_fAtten != fAtten) 
	{
		m_fAtten = fAtten;
		m_bParamChanged = true;
	}
	if (m_nAttenUnit != nAttenUnit) 
	{
		m_nAttenUnit = nAttenUnit;
		m_bParamChanged = true;
	}
	if (m_fDiv != fDiv) 
	{
		m_fDiv = fDiv;
		m_bParamChanged = true;
	}
	if (m_fCenter != fCenter) 
	{
		m_fCenter = fCenter;
		m_bParamChanged = true;
	}
	if (m_fSpan != fSpan) 
	{
		m_fSpan = fSpan;
		m_bParamChanged = true;
	}
	if (m_fRBW != fRBW) 
	{
		m_fRBW = fRBW;
		m_bParamChanged = true;
	}
	if (m_fVBW != fVBW) 
	{
		m_fVBW = fVBW;
		m_bParamChanged = true;
	}
	if(m_bParamChanged)
	{
		m_curFrame = 1;
		m_refindex = -1;
	}
	//测试
//	m_fCenter = m_xData1[m_nDataCount/2-1];
//	m_fSpan = (m_fCenter - m_xData1[0])*2;
	//测试
	return m_bParamChanged;
}

void CSpectraView::SetTraceData(double *x, double *y, int count)
{
	//设置显示数据
	if (count >= 0 && count < DATA_MAXCOUNT) 
	{
		memcpy(m_xData1, x, count * sizeof(double));
		memcpy(m_yData1, y, count * sizeof(double));
		m_nDataCount = count;
	}
	else{
		m_nDataCount = 0;
	}
	if (m_nDataCount <= 0) 
	{
		return;
	}

}

void CSpectraView::ValueToDP(RECT rcView, double x, double y, long &nViewX, COLORREF &color)
{
	if(y>m_fRef) y=m_fRef;
 	
	//坐标转换；数值==>设备坐标
	nViewX = (rcView.left + rcView.right)/2 + (long)((x - m_fCenter)/m_fSpan * (rcView.right - rcView.left));
	int nColorIndex = (long)((m_fRef - y)*143/(10*m_fDiv));//(sizeof(ColorTableAll)/sizeof(COLORREF)));

	if(nColorIndex < 0)
	{
		nColorIndex = 0;
	}
	else if (nColorIndex >= 144) 
	{
		nColorIndex = 143;
	}
    if(nViewX < rcView.left)
	{
		nViewX = rcView.left;
	}
	if(nViewX > rcView.right)
	{
		nViewX = rcView.right;
	}

	color = ColorTableAll[nColorIndex];
}

void CSpectraView::GetHzStr(double value, CString &str)
{
	if(value<0)
	{
		value = -value;
	}
	if (value >= 1.0E9)
	{		
		value /= 1.0E9;
		str.Format("%.9f", value);
		str.TrimRight("0");
		str.TrimRight(".");
		str = str + " GHz";
	} 
	else if(value >= 1.0E6)
	{
		value /= 1.0E6;
		str.Format("%.6f", value);
		str.TrimRight("0");
		str.TrimRight(".");
		str = str + " MHz";
	}
	else if (value >= 1.0E3)
	{
		value /= 1.0E3;
		str.Format("%.3f", value);
		str.TrimRight("0");
		str.TrimRight(".");
		str = str + " kHz";
	} 
	else
	{
		str.Format("%.0f Hz", value);
	}	
}
void CSpectraView::DrawRefLine(CDC *dc,RECT rect,int index)
{
	CPen pen,*oldpen;
	pen.CreatePen(PS_SOLID,1,RGB(255,0,0));	
	oldpen = dc->SelectObject(&pen);

	LONG curWeight = rect.bottom + index;
	int count = rect.bottom - rect.top;
	
	dc->MoveTo(rect.left+1,curWeight);
	dc->LineTo(rect.right-1,curWeight);

	dc->SelectObject(oldpen);

}

void CSpectraView::SetRefLine(int index,BOOL bisMove)
{
	m_refindex = index;
	if(m_refindex> -1)
		m_refindex = -1;
	if(m_refindex<=m_curFrame-1)
		m_refindex = m_curFrame-1;	
	m_isMove = bisMove;
		    
}

int CSpectraView::GetRefLineIndex()
{
	return m_refindex;
}

int CSpectraView::GetcurFrame()
{
	return m_curFrame;
}
