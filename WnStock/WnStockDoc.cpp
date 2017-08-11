
// WnStockDoc.cpp : CWnStockDoc ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "WnStock.h"
#endif

#include "WnStockDoc.h"
#include "WnStockView.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CWnStockDoc

IMPLEMENT_DYNCREATE(CWnStockDoc, CDocument)

BEGIN_MESSAGE_MAP(CWnStockDoc, CDocument)
END_MESSAGE_MAP()


// CWnStockDoc ����/����

CWnStockDoc::CWnStockDoc()
{
	// TODO: �ڴ����һ���Թ������

// 	while (1)
// 	{
// 		RealTimeStock* realTimeStock = (RealTimeStock *) malloc(sizeof(RealTimeStock));
// 		StockData stockData;
// 		stockData.GetRealTimeStockData("600016", realTimeStock);
// 		free(realTimeStock);
// 		realTimeStock = NULL;
// 
// 	}


	ReadMystock();
	GetMytockData();
	KDay = 60;		//Ĭ�ϻ�60��K��
	KPos = 0;		//Ĭ��ƫ����
	KBegin = 0;		//��ʼ��ʾ��K������	
	GetDayKData("600016");
}

CWnStockDoc::~CWnStockDoc()
{
}

BOOL CWnStockDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CWnStockDoc ���л�

void CWnStockDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}

#ifdef SHARED_HANDLERS

// ����ͼ��֧��
void CWnStockDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// �޸Ĵ˴����Ի����ĵ�����
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// ������������֧��
void CWnStockDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// ���ĵ����������������ݡ�
	// ���ݲ���Ӧ�ɡ�;���ָ�

	// ����:  strSearchContent = _T("point;rectangle;circle;ole object;")��
	SetSearchContent(strSearchContent);
}

void CWnStockDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CWnStockDoc ���

#ifdef _DEBUG
void CWnStockDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CWnStockDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CWnStockDoc ����


/*************************�Զ��庯��ʵ��***************************/

/*
*	@ ��ȡMy Stock����
*/
void CWnStockDoc::ReadMystock()
{
	StockData stockData;
	vMystock = stockData.ReadMystockCode();
}

/*
*	@ ����My Stock����
*/
void CWnStockDoc::WriteMystock()
{
	
}

void CWnStockDoc::AddMystock(char* stockCode)
{
	vMystock.push_back(stockCode);
}

void CWnStockDoc::DelMystock(char* stockCode)
{
	string strCode = stockCode;
	for (vector<string>::iterator it = vMystock.begin(); it != vMystock.end(); it++)
	{
		string strTemCode = *it;
		if (strTemCode == strCode)
		{
			vMystock.erase(it);
			break;
		}
	}
	
}

void CWnStockDoc::GetMytockData()
{
	//�ͷ�realtimeDataGroup�ڴ�
	for(vector <RealTimeStock *>::iterator it = vMystockRealTimeData.begin(); it != vMystockRealTimeData.end(); it ++)
	{
		if (NULL != *it) 
		{
			delete *it; 
			*it = NULL;
		}
	}
	vMystockRealTimeData.clear();

	for (int i=0; i<vMystock.size(); i++)
	{
 		StockData stockData;
 		RealTimeStock * realTimeStock = (RealTimeStock*)malloc(sizeof(RealTimeStock));
		memset(realTimeStock, 0, sizeof(RealTimeStock));
 		stockData.GetRealTimeStockData((char*)vMystock[i].c_str(),realTimeStock);
 		vMystockRealTimeData.push_back(realTimeStock);
	}
	return ;
}

void CWnStockDoc::GetDayKData(char* stockCode)
{
	//�ͷ�vKData�ڴ�
	for(vector<KStock*>::iterator it = vKData.begin(); it != vKData.end(); it ++)
	{
		if (NULL != *it) 
		{
			delete *it; 
			*it = NULL;
		}
	}
	vKData.clear();

	StockData stockData;
	stockData.GetKStockData1(stockCode);
	for (int i=0; i<stockData.vKStockData.size(); i++)
	{
		KStock* kStock = (KStock*)malloc(sizeof(KStock));
		memset(kStock, 0, sizeof(KStock));
		memcpy(kStock->strStockCode,stockData.vKStockData[i]->strStockCode, strlen(stockData.vKStockData[i]->strStockCode));
		memcpy(kStock->strDate,stockData.vKStockData[i]->strDate, strlen(stockData.vKStockData[i]->strDate));
		kStock->fPassClose = stockData.vKStockData[i]->fPassClose;
		kStock->fOpen = stockData.vKStockData[i]->fOpen;
		kStock->fClose = stockData.vKStockData[i]->fClose;
		kStock->fLow = stockData.vKStockData[i]->fLow;
		kStock->fHigh = stockData.vKStockData[i]->fHigh;
		kStock->Volume = stockData.vKStockData[i]->Volume;
		kStock->Amount = stockData.vKStockData[i]->Amount;
		vKData.push_back(kStock);
	}


	//�ͷ�vKData�ڴ�
	for(vector<KStock*>::iterator it = stockData.vKStockData.begin(); it != stockData.vKStockData.end(); it ++)
	{
		if (NULL != *it) 
		{
			delete *it; 
			*it = NULL;
		}
	}
	stockData.vKStockData.clear();
}

void CWnStockDoc::GetKPoint()
{
	//�ͷ�vDayKPoint�ڴ�
	for (vector <KPoint *>::iterator it=vKPoint.begin(); it!=vKPoint.end(); it++)
	{
		if (*it!=NULL)
		{
			delete *it;
			*it = NULL;
		}
	}

	vKPoint.clear();

	POINT high, low, open , close;
	POSITION pos = GetFirstViewPosition();
	CWnStockView* pView = (CWnStockView*)GetNextView(pos);

	double ih = pView->m_interKH;//K�ߵ��и�

	// 	CRect rect;
	// 	pView->GetClientRect(&rect);
	// 	float ih = (rect.bottom - 67)/16;//K�ߵ��и�

	KHighMax = 0;
	KLowMin = 1000;

	double checkKHighMax = KHighMax;
	double checkKLowMin = KLowMin;

	int n = vKData.size();

	if (n==0)
	{
		return;
	}
	int i;

	if (n<KDay)
	{
		KDay = n;
	}
	for(i = n-KDay + KPos;i < n + KPos; i++) //�������10��
	{
		if (vKData[i]->fHigh > KHighMax)
			KHighMax = vKData[i]->fHigh;
		if (vKData[i]->fLow < KLowMin)
			KLowMin = vKData[i]->fLow;
	}

	// 	if(checkKHighMax != KHighMax || checkKLowMin != KLowMin)
	// 		pView->Invalidate();

	double KM = KHighMax - KLowMin;		//������С�ļ۸��
	double a = 9 * ih;					//��ֱ�߶�
	//float b = rect.right - 200 -62;	//ˮƽ�߶�
	double b = pView->m_interKW;
	double lw = b / KDay;

	for(i = 0 ; i < n ; i++)
	{
		high.x = (int)(lw*9.0/10.0/2.0 + 0.5);
		high.y = (int)(17 + 9.5*ih - (vKData[i]->fHigh - KLowMin) * a / KM /*+0.5*/);

		low.x = (int)(lw*9.0/10.0/2.0 + 0.5);
		low.y = (int)(17 + 9.5*ih - (vKData[i]->fLow - KLowMin) * a / KM);

		open.x =  0;
		open.y = (int)(17 + 9.5*ih - (vKData[i]->fOpen - KLowMin) * a / KM /*+0.5*/);

		close.x = (int)(lw*9.0/10.0 +0.5);
		close.y = (int)(17 + 9.5*ih - (vKData[i]->fClose - KLowMin) * a / KM /*+0.5*/);

		KPoint* pKPoint = (KPoint* )malloc(sizeof(KPoint));
		memset(pKPoint, 0, sizeof(KPoint));

		pKPoint->high = high;
		pKPoint->low = low;
		pKPoint->open = open;
		pKPoint->close = close;

		vKPoint.push_back(pKPoint);
	}
	return;
}

void CWnStockDoc::GetMaPrice()
{
	//�ͷ��ڴ�
	vMa5Price.clear();
	vMa10Price.clear();
	vMa20Price.clear();
	vector<double >(vMa5Price).swap(vMa5Price);
	vector<double >(vMa10Price).swap(vMa10Price);
	vector<double >(vMa20Price).swap(vMa20Price);


	int n = vKData.size();
	if (n ==0)
	{
		return;
	}

	if (n<=5) //������MA5
	{
		return;
	}
	int i,j;
	//ǰ�油4�����ݱ�֤ma5Group���ݺ�kDataGroup���ݳ���һ��
	for (int i=0; i<4; i++)
	{
		vMa5Price.push_back(0);
	}
	for (i = 4;i < n;i++)
	{
		double sumPrice = 0;
		for (j = i;j>=i-4; j--)
		{
			sumPrice += vKData[j]->fClose;
		}
		vMa5Price.push_back(sumPrice/5);
	}

	if (n<=10)//������MA10
	{
		return;
	}
	//ǰ�油9�����ݱ�֤ma10Group���ݺ�kDataGroup���ݳ���һ��
	for (int i=0; i<9; i++)
	{
		vMa10Price.push_back(0);
	}

	for (i = 9;i < n;i++)
	{
		double sumPrice = 0;
		for (j = i;j>=i-9; j--)
		{
			sumPrice += vKData[j]->fClose;
		}
		vMa10Price.push_back(sumPrice/10);
	}

	if (n<=20)//������MA20
	{
		return;
	}

	//ǰ�油19�����ݱ�֤ma20Group���ݺ�kDataGroup���ݳ���һ��
	for (int i=0; i<19; i++)
	{
		vMa20Price.push_back(0);
	}

	for (i = 19;i < n;i++)
	{
		double sumPrice = 0;
		for (j = i;j>=i-19; j--)
		{
			sumPrice += vKData[j]->fClose;
		}
		vMa20Price.push_back(sumPrice/20);
	}
	return;
}

void CWnStockDoc::GetMaPoint()
{
	//�ͷ��ڴ�
	vMa5Point.clear();
	vMa10Point.clear();
	vMa20Point.clear();
	vector<POINT >(vMa5Point).swap(vMa5Point);
	vector<POINT >(vMa10Point).swap(vMa10Point);
	vector<POINT >(vMa20Point).swap(vMa20Point);

	int n = vMa5Price.size();
	if (n<=5)
	{
		return;
	}

	POSITION pos = GetFirstViewPosition();
	CWnStockView* pView = (CWnStockView*)GetNextView(pos);

	double kmPrice = KHighMax - KLowMin; //kDay�����ֵ��Сֵ��


	double a = pView->m_interKH*9;     //��ֱ�߶�
	double b = pView->m_interKW;       //ˮƽ���
	double lw = b/KDay; 


	POINT point;
	//ma5������ 
	for (int i = 0 + KPos; i<KDay+KPos; i++)
	{
		point.x = (int)(62+(i-KPos)*lw+0.5*lw);
		point.y = (int)(17 + pView->m_interKH*9.5 -(vMa5Price[n-KDay+i]-KLowMin)*a/kmPrice);
		vMa5Point.push_back(point);
	}

	//ma10����
	n = vMa10Price.size();
	if(n==0)
		return;
	for (int i = 0 + KPos; i<KDay+KPos; i++)
	{
		point.x = (int)(62+(i-KPos)*lw+0.5*lw);
		point.y = (int)(17 + pView->m_interKH*9.5 -(vMa10Price[n-KDay+i]-KLowMin)*a/kmPrice);
		vMa10Point.push_back(point);
	}

	//ma20����
	n = vMa20Price.size();
	if (n==0)
		return;
	for (int i = 0 + KPos; i<KDay+KPos; i++)
	{
		point.x = (int)(62+(i-KPos)*lw+0.5*lw);
		point.y = (int)(17 + pView->m_interKH*9.5 -(vMa20Price[n-KDay+i]-KLowMin)*a/kmPrice);
		vMa20Point.push_back(point);
	}
	return;
}

void CWnStockDoc::GetKVolumePoint()
{
	//�ͷ�kPointGroup�ڴ�
	for (vector <KVolumePoint *>::iterator it=vVolumePoint.begin(); it!=vVolumePoint.end(); it++)
	{
		if (*it!=NULL)
		{
			delete *it;
			*it = NULL;
		}
	}
	vVolumePoint.clear();


	POSITION pos = GetFirstViewPosition();
	CWnStockView* pView = (CWnStockView*)GetNextView(pos);

	POINT top, bottom;
	double ih = pView->m_interKH;

	double a = 2 * ih;               //��ֱ�߶�
	double b = pView->m_interKW;       //ˮƽ���
	double lw = b / KDay;

	KMaxVolume = 0;

	int n = vKData.size();
	if (n == 0)
	{
		return;
	}
	int i;
	for(i =  n-KDay + KPos; i <  n + KPos; i++)
	{
		if(vKData[i]->Volume >= KMaxVolume)
		{
			KMaxVolume = vKData[i]->Volume;
		}
	}

	for(i = 0; i < n; i++)
	{
		top.x =0;
		//top.y = pView->rect.bottom - 50 - 4*ih - ((double)vKData[i]->Volume) * a / KMaxVolume; //���ַ�������׼ȷ
		top.y = pView->m_interKH*12+17 - ((double)vKData[i]->Volume) * a / KMaxVolume;
		bottom.x = (double)(lw*9.0/10.0 +0.5);
		//bottom.y = pView->rect.bottom - 50 - 4*ih;
		bottom.y = pView->m_interKH*12+17;

		KVolumePoint* pKVolumePoint = (KVolumePoint*)malloc(sizeof(KVolumePoint));
		memset(pKVolumePoint, 0, sizeof(KVolumePoint));
		pKVolumePoint->top = top;
		pKVolumePoint->bottom =bottom;

		vVolumePoint.push_back(pKVolumePoint); 
	}   

	return;
}

void CWnStockDoc::GetMacd()
{
	//�ͷ��ڴ�
	vMacdDif.clear();
	vMacdDea.clear();
	vMacd.clear();
	vector<double>(vMacdDif).swap(vMacdDif);
	vector<double>(vMacdDea).swap(vMacdDea);
	vector<double>(vMacd).swap(vMacd);


	//ѡ��MACD����ΪMACD(12,26,9)
	int n = vKData.size();	
	if (n==0)
	{
		return;
	}
	double EMA12 = 0;
	double YesDayEMA12 = 0;
	double EMA26 = 0;
	double YesDayEMA26 = 0;

	//�������ֵDIF
	for (int i=0; i<n; i++)
	{
		if (i == 0)
		{
			EMA12 = vKData[i]->fClose;
			YesDayEMA12 = EMA12;
			EMA26 = vKData[i]->fClose;
			YesDayEMA26 = EMA26;
		}
		else
		{
			EMA12 = vKData[i]->fClose*2/13 + YesDayEMA12*11/13;
			YesDayEMA12 = EMA12;
			EMA26 = vKData[i]->fClose*2/27 + YesDayEMA26*25/27;
			YesDayEMA26 = EMA26;
		}
		vMacdDif.push_back(EMA12 - EMA26);
	}

	//����DEA
	double DEA = 0;
	double YesDayDEA = 0;
	for (int i=0; i<n; i++)
	{
		if(i == 0)
		{
			DEA = 0;
			YesDayDEA = DEA;
		}
		else
		{
			DEA = vMacdDif[i]*2/10 + YesDayDEA*8/10;
			YesDayDEA = DEA;
		}
		vMacdDea.push_back(DEA);
	}

	//����MACD
	for (int i=0; i<n; i++)
	{
		double fMacd = 0;
		fMacd = 2*(vMacdDif[i] - vMacdDea[i]);
		vMacd.push_back(fMacd);
	}
	return;
}

void CWnStockDoc::GetMacdPoint()
{
	//�ͷ��ڴ�
	vMacdPoint.clear();
	vMacdDifPoint.clear();
	vMacdDeaPoint.clear();
	vector<POINT >(vMacdPoint).swap(vMacdPoint);
	vector<POINT >(vMacdDifPoint).swap(vMacdDifPoint);
	vector<POINT >(vMacdDeaPoint).swap(vMacdDeaPoint);


	POSITION pos = GetFirstViewPosition();
	CWnStockView* pView = (CWnStockView*)GetNextView(pos);

	int n = vMacd.size();
	if (n==0)
	{
		return;
	}
	CRect rect;
	pView->GetClientRect(&rect);

	double a = pView->m_interKH;
	double b = pView->m_interKW;
	double lw = b/KDay;

	maxMACD = -100;
	minMACD = 100;
	double maxDIF = -100;
	double minDIF = 100;
	double maxDEA = -100;
	double minDEA = 100;

	//������ʾ������MACD�������
	for (int i = n-KDay+KPos; i < n+KPos; i++)
	{
		if (vMacd[i] > maxMACD)
		{
			maxMACD = vMacd[i];
		}
		if (vMacd[i] < minMACD)
		{
			minMACD = vMacd[i];
		}
	}
	POINT macdPoint;
	for (int i = n-KDay+KPos; i< (n+KPos); i++)
	{
		if (vMacd[i] >= 0)
		{
			macdPoint.x = 62+(i-n+KDay-KPos)*lw+0.5*lw;
			macdPoint.y = (a - a* vMacd[i]/maxMACD) + 13*a +17;
		}
		else
		{
			macdPoint.x = 62+(i -n+KDay-KPos)*lw+0.5*lw;
			macdPoint.y = a * (vMacd[i]) / minMACD + 14*a + 17;
		}

		//����MACD������
		vMacdPoint.push_back(macdPoint);
	}


	//������ʾ������DIF����
	a = pView->m_interKH*2;
	for (int i = n-KDay+KPos; i < n+KPos; i++)
	{
		if (vMacdDif[i] >= maxDIF)
		{
			maxDIF = vMacdDif[i];
		}
		if (vMacdDif[i] < minDIF)
		{
			minDIF = vMacdDif[i];
		}
	}
	POINT difPoint;
	for (int i = n-KDay+KPos; i< (n+KPos); i++)
	{
		if (vMacdDif[i] >= 0)
		{
			difPoint.x = 62+(i-n+KDay-KPos)*lw+0.5*lw;
			difPoint.y = (a -a * vMacdDif[i] / maxDIF) + 12*(pView->m_interKH) +17;
		}
		else
		{
			difPoint.x = 62+(i-n +KDay-KPos)*lw+0.5*lw;
			difPoint.y = a * (vMacdDif[i]) / minDIF + 14*(pView->m_interKH) + 17;
		}
		//����DIF������
		vMacdDifPoint.push_back(difPoint);
	}


	//������ʾ������DEA����
	a = pView->m_interKH*2;
	for (int i= n- KDay + KPos; i < n + KPos; i++)
	{
		if (vMacdDea[i] >= maxDEA)
		{
			maxDEA = vMacdDea[i];
		}
		if (vMacdDea[i] < minDEA)
		{
			minDEA = vMacdDea[i];
		}
	}
	POINT deaPoint;
	for (int i = n - KDay + KPos; i< (n + KPos); i++)
	{
		if (vMacdDea[i] >= 0)
		{
			deaPoint.x = 62+(i -n + KDay - KPos)*lw+0.5*lw;
			deaPoint.y = (a -a * vMacdDea[i] / maxDEA) + 12*(pView->m_interKH) +17;
		}
		else
		{
			deaPoint.x = 62+(i -n + KDay - KPos)*lw+0.5*lw;
			deaPoint.y = a * (vMacdDea[i]) / minDEA + 14*(pView->m_interKH) + 17;
		}
		//����DIF������
		vMacdDeaPoint.push_back(deaPoint);
	}
	return;
}