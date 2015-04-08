#if !defined(AFX_GRAPHSTATIC_H__98CC5531_1830_44AC_A76C_DEFBBA723F68__INCLUDED_)
#define AFX_GRAPHSTATIC_H__98CC5531_1830_44AC_A76C_DEFBBA723F68__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GraphStatic.h : header file
//
#include "math.h"
#include <afxtempl.h>
#include "MeasInfoStatic.h"

#define DATA_MAXCOUNT	4048
#define MAX_MARKER_NUM	4   //Marker个数
#define MAX_TRACE_NUM	3   //Trace个数
#define MAX_CHARS	252
	

/////////////////////////////////////////////////////////////////////////////
// CGraphStatic window

class CGraphStatic : public CStatic
{
// Construction
public:
	CGraphStatic();
	void SetGraphParam(double fRef, int nRefUnit, double fAtten, int nAttenUnit, 
	int fDiv, double fCenter, double fSpan, double fRBW, double fVBW, double fsweptime,int fftpts,int state);


typedef enum MARKER_STYLE {
	MS_CROSS,
	MS_DIAMOND,
	MS_OUT	//Marker出绘图区(上溢或下溢)，给出标记
} MARKER_STYLE;

typedef enum MMARKER_DIR {
	MM_LEFT,
	MM_RIGHT
} MMARKER_DIR;
typedef struct _TRefLine
{
	BOOL Enabled;//显示使能标志位
	double Amplitude;//幅度信息
	COLORREF Color;   //光标颜色

}TRefLine;
typedef struct _TMeasItem
{
	double dstartf;//监测信号起始频率
	double dstopf;//监测信号终止频率
	double drealstartf;//实际信号开始频率
	double drealstopf;//实际信号终止频率
	int    istartindex;//数据
	int    istopindex;//数据终止点索引
	int type ; //0 红色 1 蓝色

	//COLORREF color;//绘图颜色
}TMeasItem;
	
typedef struct _Marker {
	MARKER_STYLE style;
	BOOL   Enabled;   //MkrNum的标志
	BOOL   Selected;  //表明选中该光标 SelectMkr键的标志
	BOOL   MkrDelta;  //表明显示参考光标
	BOOL   isPhaseNoise; //相位燥声测量使能
	double Pha_xData;   //相位燥声测量x轴坐标索引
	double Pha_yData;   //相位燥声测量Y坐标索引
	BOOL   DeltaData; //获取参考光标的位置
	COLORREF color;   //光标颜色
	double xdata;     //每一点的x座标值
	double ydata;     //每一点的y座标值
	int    dataindex; //传入的数据点序号
	double refxData;  //参考光标的坐标
	double refyData;
	POINT  refpt;
	BOOL isPKHold;
}Marker, *pMarker;

typedef enum TRACE_STYLE {
   Blank = 0,        //消除当前选中轨迹
   MaxHold,      //最大值轨迹
   MinHold,      //最小值轨迹
   ClearWrite,   //当前实时波形
   View          //冻结当前波形
   
} Trace_MODE;


typedef struct _Trace {
	Trace_MODE mode;
	BOOL   Selected;
	BOOL   Enabled;
	COLORREF color;
}Trace, *pTrace;

//N DB 带宽测量相关

typedef struct _TNdBPoints{
	BOOL Enabled;//Ndb带宽功能使能
	int CurrentMarkerID;//当前选择MAKER编号
	double FreqVal_LeftNdBPoint;//左边NdB点频率值
	double FreqVal_RightNdBPoint;//右边NdB点频率值
	int Dataindex_LeftNdBPoint;//左边NdB点数据序号
	int Dataindex_RighttNdBPoint;//右边NdB点数据序号
	BOOL isLPointFinded;//左边NdB点查找结果标志
	BOOL isRPointFinded;//右边NdB点查找结果标志
	double NdBSpan;//Ndb带宽
	double NdB;//要查询的衰减值
	
}TNdBPoints;

typedef enum ArrowType {
	LEFT_ARROW,
	RIGHT_ARROW
} ArrowType;

// Attributes
public:

// Operations
public:
	

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGraphStatic)
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetMeasItem(TMeasItemConfig config);
	void DrawMeasItem(RECT rect, CDC *dc);
//	BOOL GetIndexbyFreq(double freq, int &index,double &realfreq);

	void GetFileLineCount(char* str);	
	BOOL isMrkEnable(int index);
	BOOL GetMrkInfo(int MrkIndex, double &freq, double &amplitude);
	void DrawArrow(int type,CDC *pDC,POINT pt,COLORREF color);
	BOOL FindRNdBPoint(int srcIndex, double Ndb);
	BOOL FindLNdBPoint(int srcIndex, double Ndb);
	void ShowArrow(RECT rect, CDC *pDC, int srcIndex, double NdB);
	void GetRefUnit(CString &strUnit);
	void DisableDelta(int index);
	void SetNdBMeas(double val);
	void SetNdBMeasOFF();
	BOOL GetDeltaInfo(int DeltaIndex, double &freq, double &amplitude);
	void DisplayRefLine();
	void DrawRefLine(CDC *pDC,RECT rect);
	void ShowNdBMeasInfo(RECT rect1,RECT rect2,CDC *pDC);
	BOOL GetIndexbyFreq(double freq, int &index, double &realfreq);
	void SetMrkPos(int x);//	
	int GetDataNumb();	
	void DrawArrow(int type,CDC *pDC,POINT pt);
	void SetPeakHoldOff();
	void SetPeakHoldOn();	
	void SetMkrVal(double &MkrVal);
	void GetSelectMkrxdata(double &data);
	void SetMkrtoDelta(double &step);
	int TrimValue(TCHAR* pValue, int len);
	void SetAlarmStop();
	int GetAlarmCSInfo(char *csname);
	void SetCSInfo(int nCSID, char *csname);
	void GetAlarmStartTime(SYSTEMTIME* StartTime);
	void GetAlarmStopTime(SYSTEMTIME* StopTime);
	void GetWarnFlag(bool *warnflag);
	void CompareRTTopLimit();
	void CompareRTBelowLimit();
	void DisableLimitView();
	void EnableLimitView();
	void GetMrkHzStr(double value, CString &str);
	void GetMrkViewPrecision(int *data,int unitindex);
	void GetSelectMkrIndex(int *index);
	void SetDnLimit(double *dnxdata,double *dnydata,int count);
	void SetUpLimit(double *upxdata,double *upydata,  int count);
//	void GetViewInfor(TViewInfor *pviewinfor);
	void GetRTData(double *data);
	void GetRTData(float* data);
	void GetBelowLimitData(double *pData/*, double *pYData*/);
	void GetTopLimitData(double *pData/*, double *pYData*/);
	void GetRecordData(double *data);
	void SetTopLimitData(double shift,double quotiety);
	void SetBelowLimitData(double shift,double quotiety);
	void SetMaxMinData(double maxdata, double mindata);
	void EnableBelowLimit();
	void EnableTopLimit();
	void DisableTopLimit();
	void DisableBelowLimit();
//	void GetState();
	void MoveLeft();
	void MoveRight();
	int GetMoveMkrIndex();
	void SetMarkerIndex(int index);
	int GetMarkerIndex();
	void DrawLimitTrace(CDC *pDC/*, int i*/);
	void SetWatchStopFIndex(int index);
	void SetWatchStartFIndex(int index);
	
	void SetTraceBlank();
	void SetTraceMode(int index);
	void SelectTrace(int index);
	void UnSelectTrace(int index);
	void SetNextPeak(int maxindex, int PKflag);
	void SetPeakIndex(int index);
	void MkrtoCF(double *pcf,double *pspan);
	void SetMkrNorm();
	void SetMkrDelta();
	void UnSelectMarker(int index);
	void SelectMarker(int index);
	void MoveMarker(MMARKER_DIR dir,int index);
	void InitGraphData();
	
	
	void DisableMarker(int index);
	void EnableMarker(int index);
	void SetMarkerColor(COLORREF color,int Curmark);
	void SetMarkerStyle(MARKER_STYLE ms,int Curmark);
	void DrawMarker(CDC*pDC, int i,int index);
	void SetLimitTraceData(double *y,double *y_top,double *y_bom,int count);
	void SetTraceData(double *x, double *y, int count);
	void SetMaxTraceData(double *x, double *y, int count);
	void SetMinTraceData(double *x, double *y, int count);
	void ValueToDP(RECT rcView, double x, double y, POINT& pt);
	void GetHzStr(double value, CString &str);
	void GetMsStr(double value, CString &str);	
	void GetBandHzStr(double value, CString &str);
	void SetTraceColor(int i,int index);
	virtual ~CGraphStatic();

	char m_EnterChar[20];
	char m_InputParam[20];
	char m_UnitEnter[10];
	char m_Input[30];
	BOOL m_ShowMarker;
	int m_peakindex;
	TNdBPoints NdBPoint;

	BOOL m_EnableMeas;
//	TCHAR m_InputBox[MAX_CHARS+4];

	// Generated message map functions
//protected:
public:
    char m_AlarmCSIDname[80];
	int m_AlarmCSID;
	int m_CSID;
	char m_CSIDname[80];
	bool alarmflag;
	SYSTEMTIME alarmStartTime;
	SYSTEMTIME alarmStopTime;
	double m_fRef;
	int m_nRefUnit;
	double m_fAtten;
	int m_nAttenUnit;
	double m_fDiv;
	double m_fCenter;
	double m_fSpan;
	double m_fRBW;
	double m_fVBW;
	double m_fSw;
	int m_fftpts;
	int m_nDataCount;

//	double m_refxData;
//	double m_refyData;
public:	
	 static double m_yLimitData[DATA_MAXCOUNT];
	 static double m_yLimitData_top[DATA_MAXCOUNT];
	 static double m_yLimitData_bom[DATA_MAXCOUNT];
	 static double m_xData[DATA_MAXCOUNT];
	 static double m_yData[DATA_MAXCOUNT];
	 static double m_xMaxData[DATA_MAXCOUNT];  //maxtrace
	 static double m_yMaxData[DATA_MAXCOUNT];
	 static double m_xMinData[DATA_MAXCOUNT];  //mintrace 
	 static double m_yMinData[DATA_MAXCOUNT];
protected:
	//view
	 static double m_xViewData[MAX_TRACE_NUM][DATA_MAXCOUNT];
	 static double m_yViewData[MAX_TRACE_NUM][DATA_MAXCOUNT];
    int  m_ViewDataCount[MAX_TRACE_NUM];
	
	//门限
	 static double m_xTopData[DATA_MAXCOUNT];  
	 static double m_yTopData[DATA_MAXCOUNT];
     static double m_xBelowData[DATA_MAXCOUNT];
	static double m_yBelowData[DATA_MAXCOUNT];
	
	
	//Marker
	//int m_nMarkerCount; //Marker个数
	Marker Markers[MAX_MARKER_NUM];
	//int Marker_Data_index;
	TRefLine RefLine;
	Trace Traces[MAX_TRACE_NUM];
	CList<TMeasItem,TMeasItem> MeasItemList;//测量部分




protected:
	bool m_bTopWarnFlag;
    bool m_bBelowWarnFlag;
	bool m_bWarnFlag;
	BOOL m_bWatchFlag;
	double m_minYData;   //设置门限所用每一帧中y最大值
	double m_maxYData;   //设置门限所用每一帧中y最小值
	bool m_BLimitFlag;
	bool m_TLimitFlag;
	int m_Fftdet;  //fft和检波模式的标志位
	int m_WatchStopIndex;
	int m_WatchStartIndex;
	void SetTraceEnable(int index);
	void SetTraceDisable(int index);
	void DrawTrace(CDC *pDC, int i);
	
	BOOL m_EnterViewFlag;
	
	double m_YDelta;
	double m_XDelta;


	
	//{{AFX_MSG(CGraphStatic)
	afx_msg void OnPaint();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	
	
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRAPHSTATIC_H__98CC5531_1830_44AC_A76C_DEFBBA723F68__INCLUDED_)
