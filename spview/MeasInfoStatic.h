#if !defined(AFX_MEASINFOSTATIC_H__E6F6A75F_A778_428D_A956_88257E9BA5C7__INCLUDED_)
#define AFX_MEASINFOSTATIC_H__E6F6A75F_A778_428D_A956_88257E9BA5C7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MeasInfoStatic.h : header file
//
//#include "GraphStatic.h"

/////////////////////////////////////////////////////////////////////////////
// CMeasInfoStatic window
//


typedef enum{CP = 0 ,OBW = 1,ACPR = 2,CN = 3,SCBlank = -1} TMeasure;

typedef struct tagMeasChannelPow
{
	//UINT nID;	//监测信道ID	
	//BOOL bEnable;	//
	double dStartf;//监测信号起始频率
	double dStopf;//监测信号终止频率
	double dRealStartf;//实际测量起始频率
	double dRealStopf;//实际测量终止频率
	int    iStartIndex;//数据起始点索引
	int    iStopIndex;//数据终止点索引
	double dCPSpan;   //测试带宽
	double dPow; 
	double dDensity; //

}TMeasChannelPow;

typedef struct tagMeasCN
{
	//UINT nID;	//监测信道ID	
	//BOOL bEnable;	//
	double dCarrierBand;//监测信号载波
	double dNoiseBand;//噪声带宽
	double dOffset; //偏移量
	
	double dStopf;//监测信号终止频率
	double dRealStartf;//实际测量起始频率
	double dRealStopf;//实际测量终止频率	
	
	double dCBPow; 
	double dNOPow;
	double dScale;
//	double dDensity; //

}TMeasCN;


typedef struct tagMeasACPRItem
{
	BOOL isEnable; //是否打开
	double dIntegSpan;//测量span
	double dOffset;//偏移量
}TMeasACPRItem;

typedef struct tagMeasACPR
{
	double MainSpan; 
	double AdjaSpan;
	double dRealStartf;//实际测量起始频率
	double dRealStopf;//实际测量终止频率
	double dMAPow; 
	double dADPow;
	double dScale;
	TMeasACPRItem Item[7];

}TMeasACPR;

typedef struct tagBasicParam //基本显示参数
{
	double fCenter;
	double fSpan;
	double fRBW;
	double fVBW;	
	int nDataNumb;
	
} TBasicParam;


//测试项目配置参数
typedef struct tagMeasItemConfig{
    TMeasCN CN;
	TMeasChannelPow CP;	
	TMeasACPR  ACPR;

	TMeasure type;
	
	double  Center;
	
}TMeasItemConfig;





class CMeasInfoStatic : public CStatic
{
// Construction
public:
	CMeasInfoStatic();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMeasInfoStatic)
	//}}AFX_VIRTUAL

// Implementation
public:
	void GetHzStr(double value, CString &str);
	void SetMeasConfig(TMeasItemConfig config);
	void SetCPMeas(double CPspan);
	void SetMode(TMeasure mode);
	double CPCal(int startfindex,int stopfindex);
	BOOL GetIndexbyFreq(double freq, int &index, double &realfreq);
	void SetBasicParam(double span,double cf,double RBW,double VBW);
	TBasicParam BasicParam;
	int m_nDataCount;
	void SetMeasData(double *x,double *y,int count);

	//CList<TMeasChannelPow,TMeasChannelPow> m_CPList;
	TMeasItemConfig MeasItem;	
	double m_xFreqVal[2000];
	double m_yAmpVal[2000];
	virtual ~CMeasInfoStatic();

	// Generated message map functions
protected:
	TMeasure m_Measmode;
	//{{AFX_MSG(CMeasInfoStatic)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MEASINFOSTATIC_H__E6F6A75F_A778_428D_A956_88257E9BA5C7__INCLUDED_)
