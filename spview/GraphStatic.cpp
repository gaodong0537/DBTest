// GraphStatic.cpp : implementation file
//

#include "stdafx.h"
#include "GraphStatic.h"

#ifdef _DEBUG
#define new DEBUG_NEW

#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGraphStatic

CGraphStatic::CGraphStatic()
{
	memset(m_xData, 0, sizeof(m_xData));
	memset(m_yData, 0, sizeof(m_yData));
	m_fAtten = 0;
	m_fRef = 0;
	m_fDiv = 10;
	m_fCenter = 100000000;
	m_fSpan = 1000000;
	m_fRBW = 100;
	m_fVBW = 100;
	m_nRefUnit = 0;
	m_nAttenUnit = 0;
	m_nDataCount=1024;
	m_fftpts=1024;
	m_fSw=0;


	m_nDataCount = 0;
	m_ShowMarker = false;
	m_EnterViewFlag = false;
	memset(m_AlarmCSIDname, 0, sizeof(m_AlarmCSIDname));
	m_AlarmCSID = 0;

	m_EnableMeas = FALSE;
}

CGraphStatic::~CGraphStatic()
{
}


BEGIN_MESSAGE_MAP(CGraphStatic, CStatic)
	//{{AFX_MSG_MAP(CGraphStatic)
	ON_WM_PAINT()
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGraphStatic message handlers


double CGraphStatic::m_yLimitData[DATA_MAXCOUNT] = {0};
double CGraphStatic::m_yLimitData_top[DATA_MAXCOUNT] = {0};
double CGraphStatic::m_yLimitData_bom[DATA_MAXCOUNT] = {0};
double CGraphStatic::m_xData[DATA_MAXCOUNT] = {0};
double CGraphStatic::m_yData[DATA_MAXCOUNT] = {0};
double CGraphStatic::m_xMaxData[DATA_MAXCOUNT] = {0};  //maxtrace
double CGraphStatic::m_yMaxData[DATA_MAXCOUNT] = {0};
double CGraphStatic::m_xMinData[DATA_MAXCOUNT] = {0};  //mintrace 
double CGraphStatic::m_yMinData[DATA_MAXCOUNT] = {0};

//view
double CGraphStatic::m_xViewData[MAX_TRACE_NUM][DATA_MAXCOUNT] = {0};
double CGraphStatic::m_yViewData[MAX_TRACE_NUM][DATA_MAXCOUNT] = {0};
//int  m_ViewDataCount[MAX_TRACE_NUM];

//门限
double CGraphStatic::m_xTopData[DATA_MAXCOUNT] = {0};  
double CGraphStatic::m_yTopData[DATA_MAXCOUNT] = {0};
double CGraphStatic::m_xBelowData[DATA_MAXCOUNT] = {0};
double CGraphStatic::m_yBelowData[DATA_MAXCOUNT] = {0};


void CGraphStatic::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	
	
	int i;
	int j=0;
	RECT rcClient;
	GetClientRect(&rcClient);

	static CDC memdc;
	static CBitmap memBmp;
	static CBrush *pOldBr;
	static CPen *pOldPn;
	static CPen pnAxis;
	static CPen pnGrid;
	static CPen pnTrace;
	static CFont *pOldFn;
	static CFont fnText;
	static CFont EnText;

	static BOOL inital = FALSE;
	if (!inital)
	{
		memdc.CreateCompatibleDC(&dc);
		memBmp.CreateCompatibleBitmap(&dc, 
			rcClient.right - rcClient.left,
			rcClient.bottom - rcClient.top);
		memdc.SelectObject(&memBmp);

		pnAxis.CreatePen(PS_SOLID, 1, RGB(220, 220, 220));
		pnGrid.CreatePen(PS_SOLID, 1, RGB(140, 140, 140));
		pnTrace.CreatePen(PS_SOLID, 1, RGB(255, 255, 0));
		
		fnText.CreateFont(20, 0, 0, 0, 
			FW_THIN, 
			0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FF_MODERN, 
			"Times New Roman");
		//"Franklin Gothic Medium");
		
		EnText.CreateFont(20, 10, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FF_MODERN, "Arial");

		inital = TRUE;
	}

	//坐标
	RECT rect[] = {
		{60, 30, rcClient.right - 5, rcClient.bottom - 45}, //Grid
		{5, 5, 0, 0}, //REF
		{260, 5, 0, 0}, //ATTEN
		{5, (rect[0].bottom - rect[0].top)/2, 0, 0}, //DIV
		{5, rcClient.bottom - 40, 0, 0}, //Center
		{rcClient.right - 5, rcClient.bottom - 40, 0, 0}, //Span right-align
		{5, rcClient.bottom - 20, 0, 0}, //RBW 
		{(rcClient.left + rcClient.right)/2 , rcClient.bottom - 20, 0, 0}, //VBW center-align
		{rcClient.right - 5, rcClient.bottom - 20, 0, 0}, //Sweep time right-align
		//{350,1,0,0},//Marker1 x值
		{645,0,670,0},
		{645,13,670,0}, //y值
		{60, 35, rcClient.right - 20, 35+100},
		//{145,35,120,80}, //输入名称
        {60, 60, rcClient.right - 5, 35+100}, //输入数值 
		{645,34,670,0},
		{645,50,670,0},// 14
		{5,25,0,0},
		{0}
	};


	pOldFn = memdc.SelectObject(&fnText);

	memdc.SetTextColor(RGB(230, 230, 230));
	memdc.SetBkMode(TRANSPARENT);
	
	//背景
	pOldBr = (CBrush *)memdc.SelectStockObject(BLACK_BRUSH );
	memdc.Rectangle(&rcClient);

	//坐标框
	memdc.SelectStockObject(NULL_BRUSH);
	POINT pt0, pt1;
	pOldPn = memdc.SelectObject(&pnGrid);
	for (i=1; i< 10; i++)
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
	memdc.SelectObject(&pnAxis);
	memdc.Rectangle(&rect[0]);

	CString str;
	CString strUnit;

	//REF
	str.Format("Ref %1.0f ", m_fRef);
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
	str = str + strUnit;
	memdc.SetTextAlign(TA_LEFT);
	memdc.TextOut(rect[1].left, rect[1].top, str);
	//ATTEN
	str.Format("Atten %1.0f dB", m_fAtten);
	memdc.TextOut(rect[2].left, rect[2].top, str);
	//DIV
	memdc.TextOut(rect[3].left, rect[3].top, "Div");
	str.Format("%1.0fdB/", m_fDiv);
	memdc.TextOut(rect[3].left, rect[3].top + 20, str);
	//CF
	GetHzStr(m_fCenter, str);
	str = "Center " + str;
	memdc.TextOut(rect[4].left, rect[4].top, str);

	//Numb
	memdc.TextOut(rect[15].left, rect[15].top, "Point");
	str.Format("%d", m_nDataCount);
	memdc.TextOut(rect[15].left, rect[15].top + 20, str);


	//SPAN
	memdc.SetTextAlign(TA_RIGHT);
	GetHzStr(m_fSpan, str);
	str = "Span " + str;
	memdc.TextOut(rect[5].left, rect[5].top, str);
	//RBW
	memdc.SetTextAlign(TA_LEFT);
	GetBandHzStr(m_fRBW, str);
	str = "RBW " + str;
	memdc.TextOut(rect[6].left, rect[6].top, str);
	//VBW
	if(m_Fftdet == 1)
	{
		memdc.SetTextAlign(TA_CENTER);
		GetBandHzStr(m_fVBW, str);
		str = "VBW " + str;
		memdc.TextOut(rect[7].left, rect[7].top, str);
	}

	//Sweep time & FFTPoints
//  	memdc.SetTextAlign(TA_RIGHT);
	if(m_Fftdet == 0)
	{
		str.Format("FFTPoint  %d", m_fftpts);
		memdc.TextOut(rect[8].left, rect[8].top, str);
		//memdc.TextOut(rect[8].left, rect[8].top, "Sweep time 100 ms");
	}
	else if(m_Fftdet == 1)
	{
		GetMsStr(m_fSw, str);
	    str = "Sweep time " + str;
		//str.Format("Sweep time %d ms", m_fSw)
	    memdc.TextOut(rect[8].left, rect[8].top, str);
	}	
	
//	输入框
	//if(m_EnterViewFlag == TRUE)
	//{
//         strcpy(m_Input,m_InputParam);
// 	    strcat(m_Input,m_UnitEnter);

// 		pOldFn = memdc.SelectObject(&EnText);
// 		memdc.SetTextColor(RGB(255, 255, 0));
// 		TRACE(m_EnterChar);
// 		memdc.SetTextAlign(TA_LEFT);
// 		memdc.MoveTo(rect[11].left, rect[11].top);
// 		memdc.TextOut(rect[11].left, rect[11].top, m_EnterChar);
//         memdc.TextOut(rect[12].left, rect[12].top, m_Input);
// 
// 		pOldFn = memdc.SelectObject(&fnText);

//	}	
	
	memdc.SetTextAlign(TA_LEFT);

	//绘制数据
//	POINT pts[DATA_MAXCOUNT];
	int m=0;

	while(m<MAX_TRACE_NUM)
	{
		DrawTrace(&memdc, m++/*, Traces[i].dataindex*/);
	}


	//drawmarker
	if(m_ShowMarker==TRUE)
    {
		while (j < MAX_MARKER_NUM) 
		{	     
			DrawMarker(&memdc, j++, Markers[j].dataindex);
 		}
		if(NdBPoint.Enabled && Markers[NdBPoint.CurrentMarkerID].Enabled)
		{
			ShowNdBMeasInfo(rect[13],rect[14],&memdc);
		}
    }

    if(m_ShowMarker==TRUE)
    {
	   int i=1;  //用于显示标注Mkr值所用，1对应Mkr[0]，2对应Mkr[1]
	   char str1[6]="";
	   char str2[10]="";
	   if(Markers[0].Selected==TRUE)
		  i=1;
	   else if(Markers[1].Selected==TRUE)
		  i=2;
	   else if(Markers[2].Selected==TRUE)
		   i=3;
	   else if(Markers[3].Selected==TRUE)
		   i=4;

	   sprintf(str1,"Mkr%d: ",i);
	   sprintf(str2,"Delta%d:",i);
	  	  	   
	   if(Markers[i-1].Enabled==TRUE)
	   {
           if(Markers[i-1].MkrDelta == FALSE)
           {
			   memdc.SetTextColor(Markers[i-1].color);		
//			   GetMrkHzStr(Markers[i-1].xdata, str);
			   GetHzStr(Markers[i-1].xdata, str);
			   str =  str1 + str;    //光标显示的X值
			   memdc.SetTextAlign(TA_RIGHT);
			   memdc.MoveTo(rect[9].right, rect[9].bottom);
			   memdc.TextOut(rect[9].left, rect[9].top, str);
			   str.Format(" %.1f ", Markers[i-1].ydata);
			   str = str + strUnit;  //光标显示的Y值
			   memdc.SetTextAlign(TA_RIGHT);
			   memdc.MoveTo(rect[10].right, rect[10].bottom);
			   memdc.TextOut(rect[10].left, rect[10].top, str);  
           } 
		   else if(Markers[i-1].MkrDelta == TRUE)   //显示Delta的值
		   {  
			   memdc.SetTextColor(Markers[i-1].color);
			   GetMrkHzStr(m_XDelta, str);
			   str = str2 + str;
			   memdc.SetTextAlign(TA_RIGHT);
			   memdc.MoveTo(rect[9].right, rect[9].bottom);
			   memdc.TextOut(rect[9].left, rect[9].top, str);
			   str.Format(" % .1f ", m_YDelta);
			   str = str + strUnit;  //光标显示的Y值
			   memdc.SetTextAlign(TA_RIGHT);
			   memdc.MoveTo(rect[10].right, rect[10].bottom);
			   memdc.TextOut(rect[10].left, rect[10].top, str);  
		   }
	   }
	  		
	}

	
	if(RefLine.Enabled)
	{
		DrawRefLine(&memdc,rect[0]);

	}
	




	
	 	

	if(m_bWatchFlag == TRUE)
	{
       DrawLimitTrace(&memdc);
	   RECT rect1 = {60, 30, rcClient.right - 5, rcClient.bottom - 50};

	   
	   //标明超界位置测试
// 	   POINT ptOnshow;
// 	   if(m_TLimitFlag == TRUE)
// 	   {
// 		   for(i=1;i<m_nDataCount;i++)
// 		   {
// 			   if(m_yData[i] >= m_yTopData[i])
// 			   {
// 				   ValueToDP(rect1,m_xData[i],m_yTopData[i],ptOnshow);
// 				   ptOnshow.y -= 5;
// 				   DrawArrow(1,&memdc,ptOnshow);				   
// 			   }
// 		   }
// 		   
// 	   }
// 	   if(m_BLimitFlag == TRUE)
// 	   {
// 		   for(i=1;i<m_nDataCount;i++)
// 		   {
// 			   if(m_yData[i] <= m_yBelowData[i])
// 			   {
// 				   ValueToDP(rect1,m_xData[i],m_yBelowData[i],ptOnshow);
// 				   ptOnshow.y += 5;
// 				   DrawArrow(0,&memdc,ptOnshow);
// 			   }
// 		   }
// 		   
// 	   }
	   //测试显示箭头
// 	   if(m_bTopWarnFlag == true || m_bBelowWarnFlag == true)
// 	   {
// 		   memdc.TextOut(rect1.right-20, 32, "WARNING!");
// 	   }
	   //测试结束
	   


	}
	
	memdc.SelectObject(pOldBr);
	memdc.SelectObject(pOldPn);
	memdc.SelectObject(pOldFn);

//	CViewTestDlg *pDlg=(CViewTestDlg*) GetParent();
	
	//MarkerTable显示信息
// 	if(pDlg->m_pMarkerTable!=NULL)
// 	{
// 		pDlg->m_pMarkerTable->Invalidate();
// 	}
	//测量画图
// 	if(pDlg->m_pMeasInfo)
// 	{
// 		DrawMeasItem(rect[0],&memdc);
// 	}	

	dc.BitBlt(rcClient.left, rcClient.top,
		rcClient.right - rcClient.left, rcClient.bottom - rcClient.top,
		&memdc,
		0, 0,
		SRCCOPY);

	// Do not call CStatic::OnPaint() for painting messages
}

void CGraphStatic::SetGraphParam(double fRef, int nRefUnit, double fAtten, int nAttenUnit, 
								 int fDiv, double fCenter, double fSpan, double fRBW, double fVBW, double fsweptime,int fftpts,int state)
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
	m_fRef = fRef;
	m_nRefUnit = nRefUnit;
	m_fAtten = fAtten;
	m_nAttenUnit = nAttenUnit;
	m_fDiv = fDiv;
	m_fCenter = fCenter;
	m_fSpan = fSpan;
	m_fRBW = fRBW;
	m_fVBW = fVBW;
	m_fSw = fsweptime;
	m_fftpts = fftpts;
	m_Fftdet = state;
}

void CGraphStatic::GetHzStr(double value, CString &str)
{
	if (value >= 1.0E9 || value <= -1.0E9)
	{		
		value /= 1.0E9;
		str.Format("%.6f", value);
		str.TrimRight("0");
		str.TrimRight(".");
		str = str + " GHz";
	} 
	else if(value >= 1.0E6 || value <= -1.0E6)
	{
		value /= 1.0E6;
		str.Format("%.6f", value);
		str.TrimRight("0");
		str.TrimRight(".");
		str = str + " MHz";
	}
	else if (value >= 1.0E3 || value <= -1.0E3)
	{
		value /= 1.0E3;
		str.Format("%.3f", value);
		str.TrimRight("0");
		str.TrimRight(".");
		str = str + " KHz";
	} 
	else
	{
		str.Format("%.0f Hz", value);
	}	
}

void CGraphStatic::GetBandHzStr(double value, CString &str)
{
	TCHAR valuestr[256];
	CString unitstr = "";

	if(value<0)
	{
		value = -value;
		return;
	}
	if (value >= 1.0E9)
	{		
		value /= 1.0E9;
		unitstr = " GHz";
	} 
	else if(value >= 1.0E6)
	{
		value /= 1.0E6;
		unitstr = " MHz";
	}
	else if (value >= 1.0E3)
	{
		value /= 1.0E3;
		unitstr = " kHz";
	} 
	else
	{
		unitstr = " Hz";
	}

	_stprintf(valuestr, "%.2f", value);
	TrimValue(valuestr, _tcslen(valuestr));
	str = valuestr;
	str += unitstr;
}

void CGraphStatic::GetMrkHzStr(double value, CString &str)
{
	BOOL NegFlag;
	int Unitindex;
	int precision=0;
    NegFlag = FALSE;
	if(value<0)
	{
		value = -value;
        NegFlag = TRUE;
	}
	if (value >= 1.0E9)
	{	
		Unitindex = 3;
		value /= 1.0E9;
		if(NegFlag == TRUE)
		{
           value = -value;
           NegFlag = FALSE;
		}
		GetMrkViewPrecision(&precision,Unitindex);
		str.Format("%.*f GHz",precision,value);
	} 
	else if(value >= 1.0E6)
	{
		value /= 1.0E6;
		Unitindex = 2;
		if(NegFlag == TRUE)
		{
			value = -value;
			NegFlag = FALSE;
		}
		GetMrkViewPrecision(&precision,Unitindex);
		str.Format("%.*f MHz",precision,value);
	}
	else if (value >= 1.0E3)
	{
		value /= 1.0E3;
		Unitindex = 1;
		if(NegFlag == TRUE)
		{
			value = -value;
			NegFlag = FALSE;
		}
		GetMrkViewPrecision(&precision,Unitindex);
		str.Format("%.*f kHz",precision, value);
	} 
	else
	{
		Unitindex = 0;
		if(NegFlag == TRUE)
		{
			value = -value;
			NegFlag = FALSE;
		}
		GetMrkViewPrecision(&precision,Unitindex);
		str.Format("%.*f Hz",precision,value);
	}	
}
void CGraphStatic::GetMsStr(double value, CString &str)
{
	if (value >= 1.0E3)
	{
		value /= 1.0E3;
		str.Format("%.1f s", value);
	} 
	else
	{
		str.Format("%.1f ms", value);
	}
}


void CGraphStatic::ValueToDP(RECT rcView, double x, double y, POINT &pt)
{
	//坐标转换；数值==>设备坐标
	pt.x = (rcView.left + rcView.right)/2 + (long)((x - m_fCenter)/m_fSpan * (rcView.right - rcView.left));
	pt.y = rcView.top + (long)((m_fRef - y)/(10*m_fDiv)*(rcView.bottom - rcView.top));
	if(pt.y > rcView.bottom)
       pt.y = rcView.bottom;
	if(pt.y < rcView.top)
       pt.y = rcView.top;
    if(pt.x < rcView.left)
		pt.x = rcView.left;
	if(pt.x > rcView.right)
       pt.x = rcView.right;
}

void CGraphStatic::SetTraceData(double *x, double *y, int count)
{
//    double max,min;

	//设置显示数据
	memcpy(m_xData, x, count * sizeof(double));
	memcpy(m_yData, y, count * sizeof(double));
	m_nDataCount = count;
}
void CGraphStatic::SetLimitTraceData(double *y,double *y_top,double *y_bom, int count)
{
//将门限数据拷贝到显示的数组中
	
	memcpy(m_yLimitData, y, count * sizeof(double));
	memcpy(m_yLimitData_top, y, count * sizeof(double));
	memcpy(m_yLimitData_bom, y, count * sizeof(double));
	m_nDataCount = count;
}
void CGraphStatic::SetMaxTraceData(double *x, double *y, int count)
{	
	//设置显示数据
	memcpy(m_xMaxData, x, count * sizeof(double));
	memcpy(m_yMaxData, y, count * sizeof(double));
	m_nDataCount = count;
	//	m_ShowMarker = true;
	
}
void CGraphStatic::SetMinTraceData(double *x, double *y, int count)
{
	
	//设置显示数据
	memcpy(m_xMinData, x, count * sizeof(double));
	memcpy(m_yMinData, y, count * sizeof(double));
	m_nDataCount = count;
	//	m_ShowMarker = true;
	
}

void CGraphStatic::DrawMarker(CDC*pDC, int i, int index)
{
	char chMkrid[1];
	char chreMkrid[3];
	RECT mrRect;
	RECT mridRect;
	RECT rcClient;
	RECT moutRect;
	RECT deltarect;
	RECT remkridrect;
	GetClientRect(&rcClient);
	RECT rect = {60, 30, rcClient.right - 5, rcClient.bottom - 50};

	//检查Index超界
	if (index < 0) {
		index =0;
	}
	else if (index > m_nDataCount - 1) {
		index = m_nDataCount - 1;
	}

	//检查Trace类型，如果不存在ClearWrite类型，则不绘制Marker
	bool bDrawMarker = false;
	int j = 0;
	for (j = 0; j< MAX_TRACE_NUM; j++) 
	{
		if(Traces[j].Enabled==TRUE && Traces[j].mode==ClearWrite)
		{
			bDrawMarker = true;
		}
		
	}
	if (!bDrawMarker)
	{
		return;
	}

	//设定Marker颜色   
	CPen pen(PS_SOLID,1,Markers[i].color);
	CPen*oldPen = pDC->SelectObject(&pen);
		  
    sprintf(chMkrid, "%d", i+1);
	sprintf(chreMkrid, "r%d", i+1);
	POINT pts[DATA_MAXCOUNT];
    if (Markers[i].Enabled) 
	{
		if(Markers[i].isPKHold)
		{
			Markers[i].dataindex=m_peakindex;
		}


       ValueToDP(rect, m_xData[index], m_yData[index], pts[index]);
	   if(Markers[i].DeltaData == TRUE)
	   {
		   //获取Delta坐标点
		   Markers[i].refxData = m_xData[index];
		   Markers[i].refyData = m_yData[index];
		   Markers[i].refpt = pts[index]; 
		   Markers[i].DeltaData = FALSE;   
	   } 
	   //根据Delta实际值画出坐标值
	   //if(Markers[i].MkrDelta == TRUE)
	  // { 
		   POINT ptDelta;
		   ValueToDP(rect, Markers[i].refxData, Markers[i].refyData, ptDelta);
		   Markers[i].refpt = ptDelta; 
	 //  }
	  
	   


	   //画Marker
	   mrRect.left = pts[index].x - 4;
	   mrRect.top = pts[index].y - 4;
	   mrRect.right = pts[index].x + 4;
	   mrRect.bottom = pts[index].y + 4;
       
	   //标注Marker的序号
	   mridRect.left = pts[index].x+1; //  更改在 中心   
	   mridRect.top = mrRect.top - 15;
	   mridRect.right = pts[index].x+1;
       mridRect.bottom = mrRect.bottom - 15;

	   //标注Marker超出范围
	   moutRect.left = pts[index].x - 2;
	   moutRect.top = pts[index].y - 2;
	   moutRect.right = pts[index].x + 2;
	   moutRect.bottom = pts[index].y + 2;

	   //参考光标
	   deltarect.left = Markers[i].refpt.x - 4;
       deltarect.top = Markers[i].refpt.y - 3;
	   deltarect.right = Markers[i].refpt.x + 4;
	   deltarect.bottom = Markers[i].refpt.y + 3;

	   //标注参考Marker的序号
	   remkridrect.left = Markers[i].refpt.x;  //  更改在 中心   
       remkridrect.top = deltarect.top - 16;
	   remkridrect.right = Markers[i].refpt.x;
	   remkridrect.bottom = deltarect.bottom -16;

	   if(Markers[i].MkrDelta == TRUE || Markers[i].isPhaseNoise == TRUE)
	   {   //画参考坐标
           pDC->MoveTo(deltarect.left, deltarect.top);
		   pDC->LineTo(deltarect.right,deltarect.top);
		   pDC->LineTo(deltarect.right,deltarect.bottom);
		   pDC->LineTo(deltarect.left,deltarect.bottom);
		   pDC->LineTo(deltarect.left,deltarect.top);
		   pDC->SetTextColor(Markers[i].color);
		   pDC->SetTextAlign(TA_LEFT);	
		   pDC->DrawText(chreMkrid, -1, &remkridrect, DT_CENTER|DT_SINGLELINE|DT_NOCLIP);

		   m_XDelta = Markers[i].xdata - Markers[i].refxData; //光标Delta值
           m_YDelta = Markers[i].ydata - Markers[i].refyData;
		   
	   }

	   if(pts[index].x==60|| pts[index].x==rcClient.right-5||pts[index].y==30||pts[index].y==rcClient.bottom - 45)
       {
		   SetMarkerStyle(MS_OUT,i);
       }else
	   {
           SetMarkerStyle(MS_DIAMOND,i);
	   }

	   if(pts[index].x==60|| pts[index].x==rcClient.right-5||pts[index].y==30||pts[index].y==rcClient.bottom - 50)
       {
		   SetMarkerStyle(MS_OUT,i);
       }
	   Markers[i].xdata = m_xData[index]; //用于屏幕显示光标所指点的数值
       Markers[i].ydata = m_yData[index];	   
       
	   switch(Markers[i].style)
	   {
		   	case MS_DIAMOND:
				  pDC->MoveTo(mrRect.left, pts[index].y);
				  pDC->LineTo(pts[index].x,mrRect.top);
				  pDC->LineTo(mrRect.right,pts[index].y);
				  pDC->LineTo(pts[index].x,mrRect.bottom);
				  pDC->LineTo(mrRect.left,pts[index].y);				 
				  
				  pDC->SetTextColor(Markers[i].color);				  
				  pDC->SetTextAlign(TA_LEFT);	
				  pDC->DrawText(chMkrid, 1, &mridRect, DT_CENTER|DT_SINGLELINE|DT_NOCLIP);
				 break;
            case MS_OUT:
                  pDC->MoveTo(moutRect.left, pts[index].y);
				  pDC->LineTo(pts[index].x,moutRect.top);
				  pDC->LineTo(moutRect.right,pts[index].y);
				  pDC->LineTo(pts[index].x,moutRect.bottom);
				  pDC->LineTo(moutRect.left,pts[index].y);  
				 break;				         
	   }
	   POINT PtView;
	   PtView = pts[index];
	  // DrawArrow(0,pDC,pts[index]);
	   if(NdBPoint.Enabled && Markers[i].Selected)
	   {
		   ShowArrow(rect,pDC,GetMarkerIndex(),NdBPoint.NdB);
	   }
	}
}

void CGraphStatic::SetMarkerStyle(MARKER_STYLE ms,int index)
{
    Markers[index].style = ms;
}

void CGraphStatic::SetMarkerColor(COLORREF color,int index)
{
   Markers[index].color = color;
}

void CGraphStatic::EnableMarker(int index)
{
    Markers[index].Enabled = TRUE;
}

void CGraphStatic::DisableMarker(int index)
{
    Markers[index].Enabled = FALSE;
}

void CGraphStatic::InitGraphData()
{
	//初始化Marker的属性
    Markers[0].Enabled = TRUE;
	Markers[1].Enabled = FALSE;
	Markers[2].Enabled = FALSE;
	Markers[3].Enabled = FALSE;
    Markers[0].style = MS_DIAMOND;
	Markers[0].color = RGB(165, 230, 241);
	Markers[1].color = RGB(255, 128, 255);
	Markers[2].color = RGB(116, 116, 255);
	Markers[3].color = RGB(170, 239, 133);
	Markers[0].xdata=0;
	Markers[0].ydata=0;
	Markers[1].xdata=0;
	Markers[1].ydata=0;
	Markers[2].xdata=0;
	Markers[2].ydata=0;
	Markers[3].xdata=0;
	Markers[3].ydata=0;
	Markers[0].dataindex= 160;
	Markers[1].dataindex= 160;
	Markers[2].dataindex= 200;
	Markers[3].dataindex= 200;
    Markers[0].Selected = TRUE;   
	Markers[1].Selected = FALSE;
	Markers[2].Selected = FALSE;
	Markers[3].Selected = FALSE;
	Markers[0].MkrDelta = FALSE;
    Markers[1].MkrDelta = FALSE;
	Markers[2].MkrDelta = FALSE;
	Markers[3].MkrDelta = FALSE;
	Markers[0].DeltaData = FALSE;
    Markers[1].DeltaData = FALSE;
	Markers[2].DeltaData = FALSE;
	Markers[3].DeltaData = FALSE;
	Markers[0].isPKHold = FALSE;
    Markers[1].isPKHold = FALSE;
	Markers[2].isPKHold = FALSE;
	Markers[3].isPKHold = FALSE;
	Markers[0].isPhaseNoise = FALSE;
    Markers[1].isPhaseNoise = FALSE;
	Markers[2].isPhaseNoise = FALSE;
	Markers[3].isPhaseNoise = FALSE;



	Traces[0].Selected = TRUE;
	Traces[0].Enabled = TRUE;
    Traces[0].mode = ClearWrite;
    Traces[0].color = RGB(255,255,0);
	Traces[1].Selected = FALSE;
	Traces[1].Enabled = FALSE;
    Traces[1].mode = ClearWrite;
    Traces[1].color = RGB(255,0,255);
	Traces[2].Selected = FALSE;
	Traces[2].Enabled = FALSE;
    Traces[2].mode = ClearWrite;
    Traces[2].color = RGB(0,255,0);





	m_TLimitFlag = false;
	m_BLimitFlag = false;
	m_bWatchFlag = FALSE;
	m_bBelowWarnFlag = false;
	m_bTopWarnFlag = false;
	alarmflag = false;

	strcpy(m_EnterChar,"");
	strcpy(m_InputParam,"");
	strcpy(m_UnitEnter,"");
	strcpy(m_Input,"");

	m_WatchStartIndex = 0;
	m_WatchStopIndex = 0;
	m_XDelta = 0;
	m_YDelta = 0;
	m_peakindex=200;

	NdBPoint.CurrentMarkerID=0;
	NdBPoint.Enabled=FALSE;
	NdBPoint.Dataindex_LeftNdBPoint=0;
	NdBPoint.Dataindex_RighttNdBPoint=0;
	NdBPoint.FreqVal_LeftNdBPoint=0;
	NdBPoint.FreqVal_RightNdBPoint=0;
	NdBPoint.isLPointFinded=FALSE;
	NdBPoint.isRPointFinded=FALSE;
	NdBPoint.NdBSpan=-100;
	NdBPoint.NdB=3;
	RefLine.Amplitude=-40;
	RefLine.Color=RGB(18,228,129);
	RefLine.Enabled=FALSE;

//	memset(&alarmStartTime, 0, sizeof(alarmStartTime));
//	memset(&alarmStopTime, 0, sizeof(alarmStopTime));
}


void CGraphStatic::MoveMarker(MMARKER_DIR dir,int index)
{   
	//double HDelta; 
	//HDelta = (m_xData[m_nDataCount-1] - m_xData[0])/(m_nDataCount-1);
    if (MM_LEFT == dir && Markers[index].dataindex > 0) 
	{
		Markers[index].dataindex --;
	//	Marker_Data_index--;
	}
	if (MM_RIGHT == dir && Markers[index].dataindex < m_nDataCount-1) 
	{
		Markers[index].dataindex ++;
		//Marker_Data_index++;
	}
	
}

void CGraphStatic::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: 光标响应键盘
//	int mrkid;
	CStatic::OnKeyDown(nChar, nRepCnt, nFlags);

	//OnKillFocus(NULL); 
	
//	if(Markers[0].Enabled==TRUE&&Markers[0].Selected == TRUE)
//	{
//        mrkid = 0;
//	}
//    else if(Markers[1].Enabled==TRUE&&Markers[1].Selected == TRUE)
//	{
//        mrkid = 1;
//	}
//	if (39 ==(int)nChar)
//	{
//       /*if(Markers[mrkid].Selected == TRUE)*/    
//	    MoveMarker(MM_RIGHT,mrkid);      
//	}
//	else if (37 == (int)nChar) 
//	{ //left arrow
//		//if(Markers[mrkid].Selected ==TRUE)       
//		MoveMarker(MM_LEFT,mrkid);       	      	
//	}
}

void CGraphStatic::SelectMarker(int index)
{
     Markers[index].Selected = TRUE;
}

void CGraphStatic::UnSelectMarker(int index)
{
     Markers[index].Selected = FALSE;
}

void CGraphStatic::SetMkrDelta()
{
	int i=0;
    //m_MkrDelta = TRUE;
	//m_DeltaData = TRUE;
	while(i<MAX_MARKER_NUM)
	{
		if(Markers[i].Selected == TRUE)
		{
			Markers[i].MkrDelta = TRUE;
			Markers[i].DeltaData = TRUE;
		    break;
		}
	    i++;
	}
}

void CGraphStatic::SetMkrNorm()
{
	int i=0;
    //m_MkrDelta = FALSE;
	while(i<MAX_MARKER_NUM)
	{
		if(Markers[i].Selected == TRUE)
		{
		   Markers[i].MkrDelta = FALSE;
           break;  
		}
		i++;
	}
}

void CGraphStatic::MkrtoCF(double *pcf, double *pspan)
{
	int mkrindex=0;
	int i;
	for(i=0;i<MAX_MARKER_NUM;i++)
	{
		while(Markers[i].Selected==TRUE)
		{
            mkrindex=i;
			i = MAX_MARKER_NUM;
			break;
		}
	}
//   if(Markers[mkrindex].xdata-*pspan/2 > 0 && Markers[mkrindex].xdata-*pspan/2 < 3.0E9)
//   {
	   *pcf = Markers[mkrindex].xdata;
 //  }
}

void CGraphStatic::SetPeakIndex(int index)
{
   int i=0;
   while(i<MAX_MARKER_NUM)
   {
	   if(Markers[i].Selected==TRUE)    //将最大峰值点的序号传给选定的Marker
	   {
		  Markers[i].dataindex = index;
	   }
	   i++;
   }
}

void CGraphStatic::SetNextPeak(int maxindex, int PKflag)
{
   int i=0;
   while(i<MAX_MARKER_NUM)
   {
	   if(Markers[i].Selected==TRUE)    //将峰值点的序号传给选定的Marker
	   {
		   if(PKflag==1)         //找到右边第一个峰值点，作为新的位置
		   {
			   if(Markers[i].dataindex < maxindex)
			   {
			      Markers[i].dataindex = maxindex;
				 /* break;*/
			   }
		   }
		   else if(PKflag==-1)   //找到左边第一个峰值点，作为新的位置
		   {
               if(Markers[i].dataindex > maxindex)
			   {
			      Markers[i].dataindex = maxindex;
				 /* break;*/
			   }
		   }		   		   
	   }
	   i++;
   }
}

void CGraphStatic::SelectTrace(int index)
{
    Traces[index].Selected = TRUE;   //表明选中该轨迹
}

void CGraphStatic::UnSelectTrace(int index)
{
    Traces[index].Selected = FALSE;   //表明选中该轨迹
}


void CGraphStatic::SetTraceMode(int index)
{
	int i=0;
    while(i<MAX_TRACE_NUM)
	{
		if(Traces[i].Selected == TRUE)
		{
           Traces[i].mode = (Trace_MODE)(index);
		 //  SetTraceColor(i,index);
           SetTraceEnable(i);
		   if(index == (Trace_MODE)View)
		   {
			   memcpy(m_xViewData[i], m_xData, m_nDataCount * sizeof(double));
			   memcpy(m_yViewData[i], m_yData, m_nDataCount * sizeof(double));
			   m_ViewDataCount[i]	= m_nDataCount;
		   }
		   break;
		}
		i++;
	}
	
}

//void CGraphStatic::SetEnterView(int index)
//{
//	strcpy(m_EnterChar,"");
//	strcpy(m_InputBox,"");
//    switch(index)
//	{
//		case 1:
//			strcpy(m_EnterChar,"Center:");
//			break;
//		case 2:
//            strcpy(m_EnterChar,"StartF:");
//			break;
//		case 3:
//			break;
//		case 4:
//			break;
//		case 5:
//			break;
//		case 6:
//			break;
//		case 7:
//			break;
//		case 8:
//			break;
//		case 0:
//			break;
//	}
//    m_EnterViewFlag=true;
//}

void CGraphStatic::SetTraceColor(int i,int index)
{
   switch(index)
   {
	   case MaxHold:
			Traces[i].color = RGB(255,0,255);
		   break;
	   case MinHold:      //最小值轨迹
		   Traces[i].color = RGB(0,255,0);
		   break;
	   case ClearWrite:
		   Traces[i].color = RGB(255,255,0);
		   break;
	   case View:
		   Traces[i].color = RGB(255,255,0);
		   break;
	   case Blank:
		   break;	   
   }
}



void CGraphStatic::DrawTrace(CDC *pDC, int i)
{
	CPen pnTrace;
	RECT rcClient;
	GetClientRect(&rcClient);
	RECT rect = {60, 30, rcClient.right - 5, rcClient.bottom - 50};
	POINT pts[DATA_MAXCOUNT];
	int j;

	if(Traces[i].Enabled==TRUE)
	{
       if(Traces[i].mode==MaxHold)
	   {
		   for (j = 0; j < m_nDataCount; j++)
		   {
			   ValueToDP(rect, m_xMaxData[j], m_yMaxData[j], pts[j]);
		   }
		   pnTrace.CreatePen(PS_SOLID, 1, Traces[i].color); 
	   }
	   else if(Traces[i].mode==MinHold)
	   {
		   for (j = 0; j < m_nDataCount; j++)
		   {
			   ValueToDP(rect, m_xMinData[j], m_yMinData[j], pts[j]);
		   }
           pnTrace.CreatePen(PS_SOLID, 1, Traces[i].color); 
	   }
	   else if(Traces[i].mode==ClearWrite)
	   {
		   for (j = 0; j < m_nDataCount; j++)
		   {
			   ValueToDP(rect, m_xData[j], m_yData[j], pts[j]);
		   }
		   pnTrace.CreatePen(PS_SOLID, 1, Traces[i].color); 
	   }

	   else if(Traces[i].mode==View)
	   {
		   for (j = 0; j < m_ViewDataCount[i]; j++)
		   {
			   ValueToDP(rect, m_xViewData[i][j], m_yViewData[i][j], pts[j]);
		   }
		   pnTrace.CreatePen(PS_SOLID, 1, Traces[i].color); 		  
	   }


	}	


// 	for (j = 0; j < m_nDataCount; j++)
// 	{
// 		ValueToDP(rect, m_xData[j], m_yData[j], pts[j]);
// 	}
// 	pnTrace.CreatePen(PS_SOLID, 1, Traces[i].color); 
	   CPen *pOldPn = pDC->SelectObject(&pnTrace);
	   pDC->Polyline(pts, m_nDataCount);
	   pDC->SelectObject(pOldPn);


}

void CGraphStatic::SetTraceEnable(int index)
{
     //int i;
     Traces[index].Enabled = TRUE;
	 if(Traces[index].mode==ClearWrite)
	 {
		 //for(i=0;i<MAX_MARKER_NUM;i++)
		 //{
			 Markers[0].Enabled = TRUE;
		 //}		
	 }
}

void CGraphStatic::SetTraceDisable(int index)
{
	int i;
	Traces[index].Enabled = FALSE;
	if(Traces[index].mode==ClearWrite)
	{
		for(i=0;i<MAX_MARKER_NUM;i++)
		{
		    Markers[i].Enabled = FALSE;
        }		
	}
}

void CGraphStatic::SetTraceBlank()
{
    int i=0;
	while(i<MAX_TRACE_NUM)
	{
		if(Traces[i].Selected==TRUE)
		{
			SetTraceDisable(i);
		}
		i++;
	}

}

void CGraphStatic::SetWatchStartFIndex(int index)
{
      m_WatchStartIndex = index;
}

void CGraphStatic::SetWatchStopFIndex(int index)
{
      m_WatchStopIndex = index;
}

void CGraphStatic::DrawLimitTrace(CDC *pDC/*, int i*/)
{
	int count;
	int j;
	POINT ptstop[DATA_MAXCOUNT];
	POINT ptsbelow[DATA_MAXCOUNT];
	RECT rcClient;
	GetClientRect(&rcClient);
	RECT rect = {60, 30, rcClient.right - 5, rcClient.bottom - 50};
 	CPen pnLimitTrace;
	pnLimitTrace.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	count =  m_WatchStopIndex - m_WatchStartIndex;

	for (j = 0; j < m_nDataCount; j++)
	{
		ValueToDP(rect, m_xTopData[j], m_yTopData[j], ptstop[j]);
		ValueToDP(rect, m_xBelowData[j], m_yBelowData[j], ptsbelow[j]);
	}
	pDC->SelectObject(&pnLimitTrace);
	if(m_TLimitFlag == true)
	{
		pDC->Polyline(ptstop, m_nDataCount);
		CompareRTTopLimit();
	}
	if(m_BLimitFlag == true)
	{
		pDC->Polyline(ptsbelow, m_nDataCount);
		CompareRTBelowLimit();
	}
	bool Flag1 = m_bTopWarnFlag;
	bool Flag = m_bBelowWarnFlag;
	//获取报警开始时间和结束时间
	if(m_bTopWarnFlag == true || m_bBelowWarnFlag == true)
	{
		if (alarmflag == false) {
			GetLocalTime(&alarmStartTime);
			//没有结束
			memset(&alarmStopTime, 0, sizeof(alarmStopTime));

			alarmflag = true;
			strcpy(m_AlarmCSIDname,m_CSIDname);
			m_AlarmCSID = m_CSID;
			return;
		}
	}


	if(m_bTopWarnFlag == false && m_bBelowWarnFlag == false)
	{
		if(alarmflag == true)
		{
			GetLocalTime(&alarmStopTime);
            alarmflag = false;
		}
	}
}

int CGraphStatic::GetMarkerIndex()
{
   int i=0;
   int MkrIndex = 160;
   while(i<MAX_MARKER_NUM)
   {
	   if(Markers[i].Selected==TRUE)
	   {
          MkrIndex = i;
		  break;
	   }
	   i++;
   }
   return Markers[MkrIndex].dataindex;
}

void CGraphStatic::SetMarkerIndex(int index)
{
	int i = 0;
    while(i<MAX_MARKER_NUM)
	{
	   if(Markers[i].Selected==TRUE)
	   {
          Markers[i].dataindex = index;
		  break;
	   }
	   i++;
	}
}
int CGraphStatic::GetMoveMkrIndex()
{
	int i=0;
	while(i<MAX_MARKER_NUM)
	{
		if(Markers[i].Selected==TRUE&&Markers[i].Enabled==TRUE)
		{
		    return i;
			break;
		}
		i++;
	}
	return 0;
}
void CGraphStatic::MoveLeft()
{
	int index=0;
	index = GetMoveMkrIndex(); 
	MoveMarker(MM_LEFT,index);
	Invalidate();
}
void CGraphStatic::MoveRight()
{
	int index=0;
	index = GetMoveMkrIndex();
	MoveMarker(MM_RIGHT,index);
	Invalidate();
}

//bool CGraphStatic::GetState()
//{
//    
//}

void CGraphStatic::EnableTopLimit()
{
   //if(data>0)
	  m_TLimitFlag = true;
}

void CGraphStatic::EnableBelowLimit()
{
    //if(data>0)
	  m_BLimitFlag = true;
}
void CGraphStatic::DisableTopLimit()
{
     m_TLimitFlag = false;
	 if (m_bBelowWarnFlag || m_bTopWarnFlag) {
		 SetAlarmStop();
		 m_bBelowWarnFlag = false;
		 m_bTopWarnFlag = false;
	 }
}
void CGraphStatic::DisableBelowLimit()
{
     m_BLimitFlag = false;
	 if (m_bBelowWarnFlag || m_bTopWarnFlag) {
		 
		 SetAlarmStop();
		 m_bBelowWarnFlag = false;
		 m_bTopWarnFlag = false;
	 }
}
//DEL void CGraphStatic::SetTopLimitData(double data)
//DEL {
//DEL    int i;
//DEL //   for(i=0;i<m_nDataCount;i++)
//DEL //   {
//DEL //	   m_yTopData[i] = m_yTopData[i] * data;
//DEL //   }
//DEL }

//DEL void CGraphStatic::SetBelowLimitData(double data)
//DEL {
//DEL 	 int i;
//DEL //	for(i=0;i<m_nDataCount;i++)
//DEL //	{
//DEL //		m_yBelowData[i] = m_yTopData[i] * data;
//DEL //	}
//DEL }

void CGraphStatic::SetMaxMinData(double maxdata, double mindata)
{
    m_maxYData = maxdata;
	m_minYData = mindata;
}

void CGraphStatic::SetTopLimitData(double shift, double quotiety)
{
    double kdata[1300];
	double adata = 0.25;
	int i;
	for(i=0;i<m_nDataCount;i++)
	{
        kdata[i] = (m_maxYData - m_yLimitData[i]) / (m_maxYData - m_minYData);
		m_yTopData[i] = m_yLimitData_top[i] + adata * shift * kdata[i] + (1 - adata) * shift;
	    m_xTopData[i] = m_xData[i];
	}
    m_yTopData[0] = m_yTopData[1]; 
}

void CGraphStatic::SetBelowLimitData(double shift, double quotiety)
{
	double kdata[1300];
	double adata = 0.3;
	int i;
	for(i=0;i<m_nDataCount;i++)
	{
        //kdata[i] = (m_yData[i] - m_minYData) / (m_maxYData - m_minYData);
		kdata[i] = (m_maxYData - m_yLimitData[i]) / (m_maxYData - m_minYData);
		m_yBelowData[i] = m_yLimitData_bom[i] - adata * shift * kdata[i] - (1 - adata) * shift;
        m_xBelowData[i] = m_xData[i];
	}
    m_yBelowData[0] = m_yBelowData[1]; 
}

//获取上下门限的数值
void CGraphStatic::GetTopLimitData(double *pData)
{
//	memcpy(pXData, m_xTopData, m_nDataCount * sizeof(double));
//	memcpy(pYData, m_yTopData, m_nDataCount * sizeof(double));
	int i;
	for(i=0;i<m_nDataCount;i++)
	{
        *pData = m_xTopData[i];
        pData++;
        *pData = m_yTopData[i];
		pData++;
	}
}

void CGraphStatic::GetBelowLimitData(double *pData/*, double *pYData*/)
{
	//memcpy(pXData, m_xBelowData, m_nDataCount * sizeof(double));
	//memcpy(pYData, m_yBelowData, m_nDataCount * sizeof(double));
	int i;
	for(i=0;i<m_nDataCount;i++)
	{
        *pData = m_xBelowData[i];
        pData++;
        *pData = m_yBelowData[i];
		pData++;
	}
}


void CGraphStatic::GetRecordData(double *data)
{
	int i;
	for(i=0;i<m_nDataCount;i++)
	{
        *data = m_xData[i];
        data++;
        *data = m_yData[i];
		data++;
	}
}

void CGraphStatic::GetRTData(double *data)
{
    memcpy(data, m_xData, m_nDataCount * sizeof(double));
	memcpy(data+m_nDataCount, m_yData, m_nDataCount * sizeof(double));
}

void CGraphStatic::GetRTData(float* data)
{
	for (int i = 0; i < m_nDataCount; i++) {
		data[i] = (float)m_xData[i];
		data[i+m_nDataCount] = (float)m_yData[i];
	}
}

void CGraphStatic::SetUpLimit(double *upxdata, double *upydata, int count)
{
   memcpy(m_xTopData,upxdata,sizeof(double)*2*count);
   memcpy(m_yTopData,upydata,sizeof(double)*2*count);
}

void CGraphStatic::SetDnLimit(double *dnxdata, double *dnydata, int count)
{
	memcpy(m_xBelowData,dnxdata,sizeof(double)*2*count);
    memcpy(m_yBelowData,dnydata,sizeof(double)*2*count);
}

void CGraphStatic::GetSelectMkrIndex(int *index)
{
	int i=0;
   	while(i<MAX_MARKER_NUM)
	{
		if(Markers[i].Selected == TRUE)
		{
			*index = i;
		}
		i++;
	}
}

void CGraphStatic::GetMrkViewPrecision(int *data,int unitindex)
{
    double freqstep;
	

    freqstep = m_fSpan / m_nDataCount;
	switch(unitindex)
	{
		case 0:  //Hz
			freqstep = freqstep;
			break;
		case 1:  //kHz
			freqstep = freqstep/1.0E3;
			break;
		case 2:  //MHz
			freqstep = freqstep/1.0E6;
			break;
		case 3:  //GHz
			freqstep = freqstep/1.0E9;
			break;
	}
    if(freqstep >= 1)
	{
		*data = 3;
	}
	else if(freqstep < 1)
	{
		while(freqstep < 1)
		{
            freqstep = freqstep * 10;
            *data = *data + 1;
		}
	}
}

void CGraphStatic::EnableLimitView()
{
    m_bWatchFlag = TRUE;     //控制Graph里显示门限的标志，一旦监测启动，则置为TRUE
}

void CGraphStatic::DisableLimitView()
{
    m_bWatchFlag = FALSE;
}
void CGraphStatic::CompareRTTopLimit()
{
	int i;
	int count = 0;
    for(i=1;i<m_nDataCount;i++)
	{
        if(m_yData[i] >= m_yTopData[i])
		{
            //m_bWarnFlag = true;
			count++;
		}//else if(m_yData[i] < m_yTopData[i])
	}
	if(count>0)
	{
       m_bTopWarnFlag = true;
	}else if(count == 0)
	{
	   m_bTopWarnFlag = false;
	}
}

void CGraphStatic::CompareRTBelowLimit()
{
	int i;
	int count = 0;
    for(i=1;i<m_nDataCount;i++)
	{
        if(m_yData[i] <= m_yBelowData[i])
		{
			//m_bWarnFlag = true;
			count++;
		}//else if(m_yData[i] > m_yBelowData[i]
	}
	if(count>0)
	{
       m_bBelowWarnFlag = true;
	}else if(count == 0)
	{
	   m_bBelowWarnFlag = false;
	}

}

void CGraphStatic::GetWarnFlag(bool *warnflag)
{
	if(m_bTopWarnFlag == true||m_bBelowWarnFlag == true)
	{
		m_bWarnFlag = true;
	}
	if(m_bTopWarnFlag == false&&m_bBelowWarnFlag == false)
	{
		m_bWarnFlag = false;
	}
	*warnflag = m_bWarnFlag;
}

void CGraphStatic::GetAlarmStartTime(SYSTEMTIME *StartTime)
{
    *StartTime = alarmStartTime;
}

void CGraphStatic::GetAlarmStopTime(SYSTEMTIME *StopTime)
{
    *StopTime = alarmStopTime;
}

void CGraphStatic::SetCSInfo(int nCSID, char *csname)
{
	m_CSID = nCSID;
    strcpy(m_CSIDname,csname);
}

int CGraphStatic::GetAlarmCSInfo(char *csname)
{
    strncpy(csname,m_AlarmCSIDname, 40);
	return m_AlarmCSID;
}
void CGraphStatic::SetAlarmStop()
{
	if(alarmflag == true)
	{
		GetLocalTime (&alarmStopTime);
		alarmflag = false;
	}
}

//去掉浮点数字符串尾部的0以及.和0
int CGraphStatic::TrimValue(TCHAR* pValue, int len)
{
	ASSERT(pValue!=NULL);
	ASSERT(len>0);
	BOOL dotted = FALSE;
	int tcl = len;
	
	if (len <= 0) return 0;
	
	while (len) {
		len--;
		if(*(pValue+len) == TEXT('0')) {
			continue;
		}
		
		if (*(pValue+len) == TEXT('.')) {
			*(pValue+len) = TEXT('\0');
			break;
		}
		else {
			int cl = len;
			while (cl >= 0 && *(pValue+cl) != TEXT('.')) cl--;
			if (cl >= 0) {
				*(pValue+(++len)) = TEXT('\0');
			}
			else {
				len = tcl;
			}
			break;
		}
	}
	
	return len;
}

void CGraphStatic::SetMkrtoDelta(double &step)
{
	//edit by D.T

	int i = GetMoveMkrIndex(); 
	double realfreq,centerfreq;
	realfreq=Markers[i].refxData+step;

	//判断是否超界，如果超出范围MAKER移动到边界
	if(realfreq>=m_xData[m_nDataCount-1])
		Markers[i].dataindex = m_nDataCount-1;
	else if(realfreq<=m_xData[0])
		Markers[i].dataindex=0;
	else//在显示范围以内，与附近2点比较，选择靠近的点为MARKER显示点
	{		
		for(int j=0;j<m_nDataCount-1;j++)
		{
		    if(realfreq>=m_xData[j]&&realfreq<m_xData[j+1])
			{
			centerfreq= (m_xData[j+1]-m_xData[j])/2 + m_xData[j];
			Markers[i].dataindex = (centerfreq>realfreq) ? j:j+1;		
			}
		}
	}

}

void CGraphStatic::GetSelectMkrxdata(double &data)
{//获取MARKER的X轴信息
	int i=0;
   	while(i<MAX_MARKER_NUM)
	{
		if(Markers[i].Selected == TRUE)
		{
			data=Markers[i].xdata;
			break;
		}
		i++;
	}

}

void CGraphStatic::SetMkrVal(double &MkrVal)
{
	int i = GetMoveMkrIndex(); 
	double centerfreq;
	

	//判断是否超界，如果超出范围MAKER移动到边界
	if(MkrVal>=m_xData[m_nDataCount-1])
		Markers[i].dataindex = m_nDataCount-1;
	else if(MkrVal<=m_xData[0])
		Markers[i].dataindex=0;
	else//在显示范围以内，与附近2点比较，选择靠近的点为MARKER显示点
	{		
		for(int j=0;j<m_nDataCount-1;j++)
		{
			if(MkrVal>=m_xData[j]&&MkrVal<m_xData[j+1])
			{
				
				centerfreq=(m_xData[j+1]-m_xData[j])/2+m_xData[j];
				Markers[i].dataindex=(centerfreq>MkrVal)?j:j+1;	
				MkrVal=m_xData[Markers[i].dataindex];	
			}
		}
	}
	


}

void CGraphStatic::SetPeakHoldOn()
{
	int i=0;
	while(i<MAX_MARKER_NUM)
   {
	   if(Markers[i].Selected==TRUE)    //将最大峰值点的序号传给选定的Marker
	   {
		  Markers[i].isPKHold=true;
	   }
	   i++;
   }

}


void CGraphStatic::SetPeakHoldOff()
{
	int i=0;
	while(i<MAX_MARKER_NUM)
	{ 
		Markers[i].isPKHold=false;
	   	i++;
	}

}

void CGraphStatic::DrawArrow(int type,CDC *pDC, POINT pt)
{
	CPen pen,pen1;
	//Markers[i].color;
	pen.CreatePen(PS_SOLID,1,RGB(255,255,255));
	pen1.CreatePen(PS_SOLID,1,RGB(255,255,255));
	
	// 为箭头向下
	if(type == 1)
	{
	pDC->SelectObject(&pen);
	pDC->MoveTo(pt.x,pt.y-9);
	pDC->LineTo(pt);
	pDC->SelectObject(&pen1);
	pDC->MoveTo(pt.x+3,pt.y-3);
	pDC->LineTo(pt);
	pDC->MoveTo(pt.x-3,pt.y-3);
	pDC->LineTo(pt);
	}
	else
	{//为箭头向上
	pDC->SelectObject(&pen);
	pDC->MoveTo(pt.x,pt.y+9);
	pDC->LineTo(pt);
	pDC->SelectObject(&pen1);
	pDC->MoveTo(pt.x+3,pt.y+3);
	pDC->LineTo(pt);
	pDC->MoveTo(pt.x-3,pt.y+3);
	pDC->LineTo(pt);

	}
	
		 

}



int CGraphStatic::GetDataNumb()
{
	return m_nDataCount;

}




void CGraphStatic::SetMrkPos(int x)
{
	RECT rcClient;
	GetClientRect(&rcClient);
	RECT rect={60, 30, rcClient.right - 5, rcClient.bottom - 45}; //Grid
	int index=0;
	double weight = (double)(rect.right - rect.left)/m_nDataCount;
	index = (int)((x-60)/weight) ;
	SetMarkerIndex(index);
	Invalidate();
}

BOOL CGraphStatic::GetIndexbyFreq(double freq, int &index, double &realfreq)
{
	if(m_xData[0]>freq  || freq>m_xData[m_nDataCount-1])
		return FALSE;
	else 
	{
		for(int j=0;j<m_nDataCount;j++)
		{

			if(freq == m_xData[m_nDataCount-1])
			{
				index = m_nDataCount -1;
				realfreq = m_xData[index];
				return TRUE;
			}

			if(freq>=m_xData[j] && freq<m_xData[j+1])
			{	
				double centerfreq;
				centerfreq = (m_xData[j+1] - m_xData[j]) / 2 + m_xData[j];
				index = (centerfreq>freq) ? j:j+1;
				realfreq = m_xData[index];
				return TRUE;
			}	
			
		}	
		return FALSE;
	}

}

void CGraphStatic::ShowNdBMeasInfo(RECT rect1, RECT rect2, CDC *pDC)
{

	pDC->SetTextColor(Markers[NdBPoint.CurrentMarkerID].color);	

	CString str,str1;
	str1.Format("%1.0f dB Wide: ",NdBPoint.NdB);
	pDC->SetTextAlign(TA_RIGHT);
	
	if(NdBPoint.NdBSpan > 0)
	{
		GetMrkHzStr(NdBPoint.NdBSpan, str);
	}
	else 
		str.Format("-100  Hz");
	
	str =  str1 + str;    //光标显示的X值
	pDC->TextOut(rect1.left, rect1.top, str);	

}

BOOL CGraphStatic::GetMrkInfo(int MrkIndex, double &freq, double &amplitude)
{
	if(Markers[MrkIndex].Enabled == FALSE)
		return FALSE;
	else
	{
		freq = Markers[MrkIndex].xdata;
		amplitude = Markers[MrkIndex].ydata;
	}
	return TRUE;
}


BOOL CGraphStatic::GetDeltaInfo(int DeltaIndex, double &freq, double &amplitude)
{
	if(Markers[DeltaIndex].MkrDelta == FALSE)
		return FALSE;
	else
	{
		freq = Markers[DeltaIndex].xdata - Markers[DeltaIndex].refxData; //光标Delta值
        amplitude = Markers[DeltaIndex].ydata - Markers[DeltaIndex].refyData;
		
	}
	return TRUE;
}

void CGraphStatic::SetNdBMeasOFF()
{
	NdBPoint.Enabled = FALSE;
}

void CGraphStatic::SetNdBMeas(double val)
{
	if(NdBPoint.Enabled == FALSE)
		NdBPoint.Enabled = TRUE;
	NdBPoint.NdB = val;
}

void CGraphStatic::ShowArrow(RECT rect, CDC *pDC, int srcIndex, double NdB)
{
	NdBPoint.CurrentMarkerID=GetMoveMkrIndex();
	NdBPoint.isLPointFinded=FindLNdBPoint(srcIndex,NdBPoint.NdB);
	NdBPoint.isRPointFinded=FindRNdBPoint(srcIndex,NdBPoint.NdB);

	if(NdBPoint.isLPointFinded&&NdBPoint.isRPointFinded)
	{
		NdBPoint.NdBSpan=NdBPoint.FreqVal_RightNdBPoint-NdBPoint.FreqVal_LeftNdBPoint;
		double Pt_yData=Markers[NdBPoint.CurrentMarkerID].ydata-NdBPoint.NdB;
		POINT Lpt,Rpt;
		ValueToDP(rect,NdBPoint.FreqVal_LeftNdBPoint,Pt_yData,Lpt);
		ValueToDP(rect,NdBPoint.FreqVal_RightNdBPoint,Pt_yData,Rpt);
		DrawArrow(1,pDC,Lpt,Markers[GetMoveMkrIndex()].color);
		DrawArrow(0,pDC,Rpt,Markers[GetMoveMkrIndex()].color);			
	}
	else NdBPoint.NdBSpan=-100;	
}

BOOL CGraphStatic::FindLNdBPoint(int srcIndex, double Ndb)
{
	int i;
	for(i=srcIndex; i>=0; i--)
	{
		if(m_yData[srcIndex] - m_yData[i] >= Ndb)
		{
			NdBPoint.Dataindex_LeftNdBPoint = i;
			NdBPoint.FreqVal_LeftNdBPoint = m_xData[i];
			return TRUE;
		}
		
	}
	return FALSE;
}

BOOL CGraphStatic::FindRNdBPoint(int srcIndex, double Ndb)
{
	int i;
	for(i=srcIndex;i<=m_nDataCount;i++)
	{
		if(m_yData[srcIndex]-m_yData[i] >= Ndb)
		{
			NdBPoint.Dataindex_LeftNdBPoint = i;
			NdBPoint.FreqVal_RightNdBPoint = m_xData[i];
			return TRUE;
		}
		
	}
	return FALSE;
}

void CGraphStatic::DrawArrow(int type, CDC *pDC, POINT pt, COLORREF color)
{
	CPen pen,pen1;
	//Markers[i].color;
	pen.CreatePen(PS_SOLID,1,color);
	//pen1.CreatePen(PS_SOLID,1,Markers[GetMoveMkrIndex()].color);
	
	if(type<1)// 为箭头向左
	{
		pDC->SelectObject(&pen);
		pDC->MoveTo(pt.x+9,pt.y);
		pDC->LineTo(pt);
		//pDC->SelectObject(&pen1);
		pDC->MoveTo(pt.x+3,pt.y+3);
		pDC->LineTo(pt);
		pDC->MoveTo(pt.x+3,pt.y-3);
		pDC->LineTo(pt);
	} 
	else// 为箭头向右
	{
		pDC->SelectObject(&pen);
		pDC->MoveTo(pt.x-9,pt.y);
		pDC->LineTo(pt);
		//pDC->SelectObject(&pen1);
		pDC->MoveTo(pt.x-3,pt.y+3);
		pDC->LineTo(pt);
		pDC->MoveTo(pt.x-3,pt.y-3);
		pDC->LineTo(pt);
	}
}

void CGraphStatic::DrawRefLine(CDC *pDC,RECT rect)
{
	CPen pen;
	pen.CreatePen(PS_SOLID,1,RefLine.Color);
	POINT pt;
	ValueToDP(rect,m_xData[100],RefLine.Amplitude,pt);
	pDC->SelectObject(&pen);
	pDC->MoveTo(rect.left,pt.y);
	pDC->LineTo(rect.right,pt.y);
	
}

void CGraphStatic::DisplayRefLine()
{
	RefLine.Enabled = TRUE;
}

void CGraphStatic::GetRefUnit(CString &strUnit)
{
	//REF
	switch(m_nRefUnit)
	{
		case 0:
			strUnit = strUnit+"dBm";
			break;
		case 1:
			strUnit = strUnit+"dBv";
			break;
		case 2:
			strUnit = strUnit+"dBmv";
			break;
		case 3:
			strUnit = strUnit+"dBuv";
			break;
		case 4:
			strUnit = strUnit+"mv";
			break;
		case 5:
			strUnit = strUnit+"w";
			break;
		default:
			strUnit = strUnit+"dBm";
	}	

}

void CGraphStatic::DisableDelta(int index)
{
	Markers[index].MkrDelta = FALSE;
}

void CGraphStatic::DrawMeasItem(RECT rect, CDC *dc)
{
	CPen *oldpen;
	CPen Redline,BlueLine;
	Redline.CreatePen(PS_SOLID, 1, RGB(255,0,0));
	BlueLine.CreatePen(PS_SOLID, 1, RGB(0,0,255));
	oldpen=dc->SelectObject(&BlueLine);//存储以前所使用的笔
	
	double starf=m_xData[0];
	double stopf=m_xData[m_nDataCount-1];
	POSITION pos = MeasItemList.GetHeadPosition();
	while (pos)
	{
		TMeasItem Item = MeasItemList.GetNext(pos);
		POINT pt1,pt2;
		pt1.x = rect.left;
		pt2.x = rect.right;
		pt1.y = pt2.y = rect.top + (rect.bottom - rect.top)/5 ;		
		//判断同时是否在显示范围之内
		if(GetIndexbyFreq(Item.dstartf,Item.istartindex,Item.drealstartf)&&
			GetIndexbyFreq(Item.dstopf,Item.istopindex,Item.drealstopf))
		{
			//根据模式选择不同颜色的笔 
			COLORREF curColor;
			int nMovePoint = 0;
			if(Item.type == 0)
			{
				dc->SelectObject(&BlueLine);
				curColor = RGB(0,0,255);
				nMovePoint = -40;
			}
			else
			{
				dc->SelectObject(&Redline);	
				curColor = RGB(255,0,0);
				nMovePoint = -25;
			}

			
			ValueToDP(rect,m_xData[Item.istartindex],m_yData[Item.istartindex],pt1);
			ValueToDP(rect,m_xData[Item.istopindex],m_yData[Item.istopindex],pt2);
			dc->MoveTo(pt1.x,rect.top + 2);
			dc->LineTo(pt1.x,rect.bottom);
			dc->MoveTo(pt2.x,rect.top + 2);
			dc->LineTo(pt2.x,rect.bottom);	
			
			//选择连接线位置
			pt1.y = pt2.y = rect.top + (rect.bottom - rect.top)/5 + nMovePoint;

			
			//画箭头
			DrawArrow(0,dc,pt1,curColor);
			DrawArrow(1,dc,pt2,curColor);
			dc->MoveTo(pt1);
			dc->LineTo(pt2);
		}
	}
	
 	dc->SelectObject(oldpen);//恢复过去笔	
}


BOOL CGraphStatic::isMrkEnable(int index)
{
	if(Markers[index].Enabled)
		return TRUE;
	else
		return FALSE;
}

void CGraphStatic::SetMeasItem(TMeasItemConfig config)
{
	m_EnableMeas = TRUE;
	MeasItemList.RemoveAll();

// 	CViewTestDlg *pDlg=(CViewTestDlg*) GetParent();
// 	if(pDlg->m_pMeasInfo != NULL)
// 	pDlg->m_pMeasInfo->SetMeasData(m_xData, m_yData, m_nDataCount);
	TMeasItem item;

	switch(config.type)
	{
	case CN:
		item.dstartf = m_fCenter - config.CN.dCarrierBand/2;
		item.dstopf = m_fCenter + config.CN.dCarrierBand/2;
		item.type = 0;
		MeasItemList.AddTail(item);	

		item.dstartf = m_fCenter + config.CN.dOffset - config.CN.dNoiseBand/2;
		item.dstopf = m_fCenter + config.CN.dOffset + config.CN.dNoiseBand/2;
		item.type = 1;
		MeasItemList.AddTail(item);	
		
		break;
	case CP:		
		item.dstartf = m_fCenter - config.CP.dCPSpan/2;
		item.dstopf = m_fCenter + config.CP.dCPSpan/2;
		item.type = 0;
		MeasItemList.AddTail(item);	
		break;
	case ACPR:
		int i;
		for(i=0; i< 3; i++)
		{
			if(config.ACPR.Item[i].isEnable)
			{
				item.dstartf = m_fCenter - config.ACPR.AdjaSpan/2
					+ (i - 3) * config.ACPR.Item[i].dOffset;
				item.dstopf = m_fCenter + config.ACPR.AdjaSpan/2
					+ (i - 3) * config.ACPR.Item[i].dOffset;
				item.type = 0;
				MeasItemList.AddTail(item);
			}
		}

		for(i=4; i<= 6; i++)
		{
			if(config.ACPR.Item[i].isEnable)
			{
				item.dstartf = m_fCenter - config.ACPR.AdjaSpan/2
					+ (i - 3) * config.ACPR.Item[i].dOffset;
				item.dstopf = m_fCenter + config.ACPR.AdjaSpan/2
					+ (i - 3) * config.ACPR.Item[i].dOffset;
				item.type = 0;
				MeasItemList.AddTail(item);
			}
		}
		
		item.dstartf = m_fCenter - config.ACPR.MainSpan/2;
		item.dstopf = m_fCenter + config.ACPR.MainSpan/2;	
		item.type = 1;
		MeasItemList.AddTail(item);
		break;
	default:
		m_EnableMeas = FALSE;
		break;
	}

}


void CGraphStatic::GetFileLineCount(char* str)
{
	sprintf(str,"\n%s is %d",__FILE__,__LINE__);	
}