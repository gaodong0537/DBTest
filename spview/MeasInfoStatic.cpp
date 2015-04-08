// MeasInfoStatic.cpp : implementation file
//

#include "stdafx.h"
//#include "ViewTest.h"
//#include "ViewTestDlg.h"
#include "MeasInfoStatic.h"
#include <math.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMeasInfoStatic

CMeasInfoStatic::CMeasInfoStatic()
{
	 m_Measmode = SCBlank;
	 m_nDataCount = 400;
	 memset(m_xFreqVal,0,sizeof(m_xFreqVal));
	 memset(m_yAmpVal,0,sizeof(m_yAmpVal));
	 memset(&BasicParam,0,sizeof(BasicParam));
	 memset(&MeasItem,0,sizeof(MeasItem));
	// MeasItem = Config;
	 
}

CMeasInfoStatic::~CMeasInfoStatic()
{
}


BEGIN_MESSAGE_MAP(CMeasInfoStatic, CStatic)
	//{{AFX_MSG_MAP(CMeasInfoStatic)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMeasInfoStatic message handlers

void CMeasInfoStatic::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	//绘图初始化
	static CDC memdc;
	static CBitmap memBmp;
	static CPen pnAxis;
	static BOOL inital = FALSE;
	RECT rcClient;
	static CFont fnText,fnText1,fnText2;
	GetClientRect(&rcClient);
	if (!inital)
	{
		memdc.CreateCompatibleDC(&dc);
		memBmp.CreateCompatibleBitmap(&dc, 
			rcClient.right - rcClient.left,
			rcClient.bottom - rcClient.top);
		memdc.SelectObject(&memBmp);

		pnAxis.CreatePen(PS_SOLID, 1, RGB(220, 220, 220));
		CFont *pOldFn;
		
		fnText.CreateFont(31, 0, 0, 0, FW_HEAVY, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FF_MODERN, "Arial");
		pOldFn = memdc.SelectObject(&fnText);	
		
		fnText1.CreateFont(31, 0, 0, 0, 
			FW_HEAVY, 
			0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS,	
			CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FF_MODERN, 
			"Times New Roman");

		fnText2.CreateFont(20, 0, 0, 0, 
			FW_THIN, 
			0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FF_MODERN, 
			"Times New Roman");

		inital = TRUE;
	}
	
	int nItemWidth = 210;
	int m_nTextHeight=16;
	//颜色
	COLORREF m_clTitle=RGB(255, 255, 255);
	COLORREF m_clText=WHITE_PEN;
	COLORREF m_clBack=RGB(0,0,0);
	memdc.SelectObject(&fnText);
	memdc.SetTextColor(RGB(230, 230, 230));
	memdc.SetBkMode(TRANSPARENT);//及其重要的，字体穿越

		//背景
    (CBrush *)memdc.SelectStockObject(BLACK_BRUSH );
	memdc.Rectangle(&rcClient);
	memdc.SelectStockObject(NULL_BRUSH);

	int nstart,nstop;
	nstart=nstop=0;

	COLORREF clOld = RGB(230, 230, 230);


	CString str,str2;
	RECT CPrect[]={
			{rcClient.left +10, rcClient.top+15 , rcClient.right - 50,rcClient.bottom},
			{rcClient.left +10, rcClient.top+46 , rcClient.right - 50,rcClient.bottom},
			{0}
		};

	//CViewTestDlg *pDlg = (CViewTestDlg *)GetParent();
		CString strParam;
		CString strName;

	//显示信息
	switch (MeasItem.type)
	{
	case CP:
		MeasItem.CP.dPow = 0;
		if(!GetIndexbyFreq(MeasItem.CP.dStartf,MeasItem.CP.iStartIndex,MeasItem.CP.dRealStartf) ||
			!GetIndexbyFreq(MeasItem.CP.dStopf,MeasItem.CP.iStopIndex,MeasItem.CP.dRealStopf))
		{
			MeasItem.CP.iStartIndex = 0;
			MeasItem.CP.iStopIndex = m_nDataCount - 1;			
		}		

		MeasItem.CP.dPow = CPCal(MeasItem.CP.iStartIndex,MeasItem.CP.iStopIndex);

		str.Format("CHANNEL POWER  %f dBm",MeasItem.CP.dPow);	
	
		static CFont *pCPOldPn;
		
		memdc.SelectObject(&fnText);
		
		pCPOldPn = memdc.SelectObject(&fnText);
		memdc.SetTextAlign(TA_LEFT);
		memdc.TextOut(CPrect[0].left,CPrect[0].top,str);
		memdc.SelectObject(&pCPOldPn);
		break;
		
	case OBW:
		break;
	case ACPR:
		int nACPRStartIndex,nACPRStopIndex;
		BOOL isACPRCalulate;
		isACPRCalulate = TRUE;		
		double dACPRStartf,dACPRStopf;
        

		dACPRStartf = BasicParam.fCenter - MeasItem.ACPR.MainSpan/2;
		dACPRStopf = BasicParam.fCenter + MeasItem.ACPR.MainSpan/2;
		if(GetIndexbyFreq(dACPRStartf,nACPRStartIndex,MeasItem.ACPR.dRealStartf) &&
			GetIndexbyFreq(dACPRStopf,nACPRStopIndex,MeasItem.ACPR.dRealStopf))
		{
			MeasItem.ACPR.dMAPow = CPCal(nACPRStartIndex,nACPRStopIndex);						
		}
		else
		{
			isACPRCalulate = FALSE;
		}

		dACPRStartf = BasicParam.fCenter - MeasItem.ACPR.AdjaSpan/2
			+ MeasItem.ACPR.Item[1].dOffset;
		dACPRStopf = BasicParam.fCenter + MeasItem.ACPR.AdjaSpan/2
			+ MeasItem.ACPR.Item[1].dOffset;
		if(GetIndexbyFreq(dACPRStartf,nACPRStartIndex,MeasItem.ACPR.dRealStartf) &&
			GetIndexbyFreq(dACPRStopf,nACPRStopIndex,MeasItem.ACPR.dRealStopf))
		{
			MeasItem.ACPR.dADPow = CPCal(nACPRStartIndex,nACPRStopIndex);						
		}
		else
		{
			isACPRCalulate = FALSE;
		}

		if(isACPRCalulate)
		{
		//	MeasItem.CN.dScale = 10 * log10(MeasItem.CN.dCBPow / MeasItem.CN.dNOPow);
		//	MeasItem.ACPR.dScale = 10 * log10(MeasItem.ACPR.dMAPow / 0.2236) -   10 * log10(MeasItem.ACPR.dADPow / 0.2236);
			MeasItem.ACPR.dScale = MeasItem.ACPR.dMAPow - MeasItem.ACPR.dADPow;
			str.Format("M/A %.4f dB",MeasItem.ACPR.dScale);
			
			double CNo = MeasItem.ACPR.dScale + 10 * log10(MeasItem.ACPR.AdjaSpan);  ;			

			str2.Format("M/Ad %.4f dB/Hz",CNo);
		}
		else
		{			
			str.Format("M/A ---.---  dBm");				
			str2.Format("M/Ad ---.---  dBm");	
		}	

		static CFont *pOldPn;

		memdc.SelectObject(&fnText);

		pOldPn = memdc.SelectObject(&fnText);
		memdc.SetTextAlign(TA_LEFT);
		
				
		memdc.TextOut(CPrect[0].left,CPrect[0].top,str);

		
		memdc.TextOut(CPrect[1].left,CPrect[1].top,str2);
	//	memdc.SelectObject(&pOldPn);		
		
		
		pOldPn = memdc.SelectObject(&fnText2);
		memdc.SetTextAlign(TA_RIGHT);

		memdc.SetTextColor(RGB(0,0,255));	
		GetHzStr(MeasItem.ACPR.MainSpan,strParam);
		strName.Format("Main Span:  ");
		strParam = strName + strParam;
		memdc.TextOut(rcClient.right,CPrect[1].top,strParam);

		memdc.SetTextColor(RGB(255,0,0));
		GetHzStr(MeasItem.ACPR.AdjaSpan,strParam);
		strName.Format("Adja Span:  ");
		strParam = strName + strParam;
		memdc.TextOut(rcClient.right,CPrect[1].top+20,strParam);

		memdc.SetTextColor(clOld);
		GetHzStr(MeasItem.ACPR.Item[1].dOffset,strParam);
		strName.Format("OffSet:  ");
		strParam = strName + strParam;
		memdc.TextOut(rcClient.right,CPrect[1].top+40,strParam);

		memdc.SelectObject(&pOldPn); 
		break;
	case CN:
		int nStartIndex,nStopIndex;
		BOOL isCalulate;
		isCalulate = TRUE;		
		double dStartf,dStopf;
		
		
		dStartf = BasicParam.fCenter - MeasItem.CN.dCarrierBand/2;
		dStopf = BasicParam.fCenter + MeasItem.CN.dCarrierBand/2;
		if(GetIndexbyFreq(dStartf,nStartIndex,MeasItem.CN.dRealStartf) &&
			GetIndexbyFreq(dStopf,nStopIndex,MeasItem.CN.dRealStopf))
		{
			MeasItem.CN.dCBPow = CPCal(nStartIndex,nStopIndex);						
		}
		else
		{
			isCalulate = FALSE;
		}
		
		dStartf = BasicParam.fCenter - MeasItem.CN.dNoiseBand/2 +MeasItem.CN.dOffset;
		dStopf = BasicParam.fCenter + MeasItem.CN.dNoiseBand/2 +MeasItem.CN.dOffset;
		if(GetIndexbyFreq(dStartf,nStartIndex,MeasItem.CN.dRealStartf) &&
			GetIndexbyFreq(dStopf,nStopIndex,MeasItem.CN.dRealStopf))
		{
			MeasItem.CN.dNOPow = CPCal(nStartIndex,nStopIndex);						
		}
		else
		{
			isCalulate = FALSE;
		}


//		MeasItem.CN.dCBPow /= MeasItem.CN.dCarrierBand;
//
//		MeasItem.CN.dNOPow /= MeasItem.CN.dNoiseBand;		
		
		if(isCalulate)
		{
		//	MeasItem.CN.dScale = 10 * log10(MeasItem.CN.dCBPow / MeasItem.CN.dNOPow);
		//	MeasItem.CN.dScale = 10 * log10(MeasItem.CN.dCBPow / 0.2236) -   10 * log10(MeasItem.CN.dNOPow / 0.2236);
			MeasItem.CN.dScale = MeasItem.CN.dCBPow - MeasItem.CN.dNOPow;
			str.Format("C/N %.4f dB",MeasItem.CN.dScale);
			
			double CNo = MeasItem.CN.dScale + 10 * log10(MeasItem.CN.dNoiseBand);  ;			

			str2.Format("C/No %.4f dB/Hz",CNo);
		}
		else
		{			
			str.Format("C/N ---.---  dBm");				
			str2.Format("C/No ---.---  dBm");	
		}	

		static CFont *pACPROldPn;

		memdc.SelectObject(&fnText);

		pACPROldPn = memdc.SelectObject(&fnText);
		memdc.SetTextAlign(TA_LEFT);
		
				
		memdc.TextOut(CPrect[0].left,CPrect[0].top,str);

		
		memdc.TextOut(CPrect[1].left,CPrect[1].top,str2);
	//	memdc.SelectObject(&pOldPn);		
		
		
		pACPROldPn = memdc.SelectObject(&fnText2);
		memdc.SetTextAlign(TA_RIGHT);

		memdc.SetTextColor(RGB(0,0,255));	
		GetHzStr(MeasItem.CN.dCarrierBand,strParam);
		strName.Format("Carrier Bandwidth:  ");
		strParam = strName + strParam;
		memdc.TextOut(rcClient.right,CPrect[1].top,strParam);

			memdc.SetTextColor(RGB(255,0,0));
		GetHzStr(MeasItem.CN.dNoiseBand,strParam);
		strName.Format("Noise Bandwidth:  ");
		strParam = strName + strParam;
		memdc.TextOut(rcClient.right,CPrect[1].top+20,strParam);

		memdc.SetTextColor(clOld);
		GetHzStr(MeasItem.CN.dOffset,strParam);
		strName.Format("OffSet:  ");
		strParam = strName + strParam;
		memdc.TextOut(rcClient.right,CPrect[1].top+40,strParam);

		memdc.SelectObject(&pOldPn); 
	//	memdc.SelectStockObject(NULL_BRUSH);

		//memdc.SelectObject(&fnText);

		
		break;
	default:
		break;
	}
	
	
	dc.BitBlt(rcClient.left, rcClient.top,
		rcClient.right - rcClient.left, rcClient.bottom - rcClient.top,
		&memdc,
		0, 0,
		SRCCOPY);
	
	// TODO: Add your message handler code here
	
	// Do not call CStatic::OnPaint() for painting messages
}

void CMeasInfoStatic::SetMeasData(double *x, double *y, int count)
{
	memcpy(m_xFreqVal, x, count * sizeof(double));
	memcpy(m_yAmpVal, y, count * sizeof(double));
	m_nDataCount = count;
}

void CMeasInfoStatic::SetBasicParam(double span,double cf,double RBW,double VBW)
{
	BasicParam.fCenter = cf;
	BasicParam.fRBW = RBW;
	BasicParam.fVBW = VBW;
	BasicParam.fSpan = span;
}

BOOL CMeasInfoStatic::GetIndexbyFreq(double freq, int &index, double &realfreq)
{
	if(m_xFreqVal[0]>freq  || freq>m_xFreqVal[m_nDataCount-1])
		return FALSE;
	else 
	{
		for(int j=0;j<m_nDataCount-1;j++)
		{
			if(freq == m_xFreqVal[m_nDataCount-1])
			{
				index = m_nDataCount -1;
				realfreq = m_xFreqVal[index];
				return TRUE;
			}

			if(freq>=m_xFreqVal[j] && freq<m_xFreqVal[j+1])
			{	
				double centerfreq;
				centerfreq = (m_xFreqVal[j+1] - m_xFreqVal[j]) / 2 + m_xFreqVal[j];
				index = (centerfreq>freq) ? j:j+1;
				realfreq = m_xFreqVal[index];
				return TRUE;
			}	
		}	
		return FALSE;
	}

}

double CMeasInfoStatic::CPCal(int startfindex, int stopfindex)
{
	int i=0;
	double PtSum=0;
	double PtPow=0;
	double NBW=0;
	double CP = 0;
    double Pi = 0;
	double Gain ;
//	CViewTestDlg *pDlg = (CViewTestDlg *)GetParent();
//	Gain = pDlg ->GetCurStdCalVal();
	Gain = 10;

	
	for(i=startfindex;i<=stopfindex;i++)
	{		
		Pi += pow(10,((m_yAmpVal[i] + Gain )/ 10))*0.2236;//* 50;
	
	//	CP += pow(10,((m_yAmpVal[i] - Gain)/ 20))* 0.2236;
		
	}
	PtPow = BasicParam.fSpan/(m_nDataCount-1);
	NBW = BasicParam.fRBW/1.2;
	CP = 10 * log10(Pi/0.2236) - Gain;
	
	return CP;

}

void CMeasInfoStatic::SetMode(TMeasure mode)
{
	m_Measmode = mode;
}

void CMeasInfoStatic::SetCPMeas(double CPspan)
{
	MeasItem.CP.dCPSpan = CPspan;

	
	//
	m_Measmode = CP;	
    
	
}

void CMeasInfoStatic::SetMeasConfig(TMeasItemConfig config)
{
	MeasItem = config;
}

void CMeasInfoStatic::GetHzStr(double value, CString &str)
{
// 	if(value<0)
// 	{
// 		value = -value;
//  	}
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
