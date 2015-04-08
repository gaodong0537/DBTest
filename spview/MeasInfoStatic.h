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
	//UINT nID;	//����ŵ�ID	
	//BOOL bEnable;	//
	double dStartf;//����ź���ʼƵ��
	double dStopf;//����ź���ֹƵ��
	double dRealStartf;//ʵ�ʲ�����ʼƵ��
	double dRealStopf;//ʵ�ʲ�����ֹƵ��
	int    iStartIndex;//������ʼ������
	int    iStopIndex;//������ֹ������
	double dCPSpan;   //���Դ���
	double dPow; 
	double dDensity; //

}TMeasChannelPow;

typedef struct tagMeasCN
{
	//UINT nID;	//����ŵ�ID	
	//BOOL bEnable;	//
	double dCarrierBand;//����ź��ز�
	double dNoiseBand;//��������
	double dOffset; //ƫ����
	
	double dStopf;//����ź���ֹƵ��
	double dRealStartf;//ʵ�ʲ�����ʼƵ��
	double dRealStopf;//ʵ�ʲ�����ֹƵ��	
	
	double dCBPow; 
	double dNOPow;
	double dScale;
//	double dDensity; //

}TMeasCN;


typedef struct tagMeasACPRItem
{
	BOOL isEnable; //�Ƿ��
	double dIntegSpan;//����span
	double dOffset;//ƫ����
}TMeasACPRItem;

typedef struct tagMeasACPR
{
	double MainSpan; 
	double AdjaSpan;
	double dRealStartf;//ʵ�ʲ�����ʼƵ��
	double dRealStopf;//ʵ�ʲ�����ֹƵ��
	double dMAPow; 
	double dADPow;
	double dScale;
	TMeasACPRItem Item[7];

}TMeasACPR;

typedef struct tagBasicParam //������ʾ����
{
	double fCenter;
	double fSpan;
	double fRBW;
	double fVBW;	
	int nDataNumb;
	
} TBasicParam;


//������Ŀ���ò���
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
