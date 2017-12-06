/********************************************************
Copyright (C), 2016-2017,
FileName: 	WnStockDoc
Author: 	woniu201
Email: 		wangpengfei.201@163.com
Created: 	2017/11/07
Description:���ĵ��ĵ���
********************************************************/

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "WnStock.h"
#endif

#include "WnStockDoc.h"
#include "WnStockView.h"
#include "KeyboardFairy.h"

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
	stockDoc = new StockDoc();


	ReadMystock();
	GetMytockData();
	stockDoc->KDay = 60;		//Ĭ�ϻ�60��K��
	stockDoc->KPos = 0;			//Ĭ��ƫ����
	stockDoc->KBegin = 0;		//��ʼ��ʾ��K������	
	
	//GetDayKData("600016");
	
	ReadAllStock();
	int n = vAllStock.size();

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
*	@��ȡ���й�Ʊ����
*/
void CWnStockDoc::ReadAllStock()
{
	StockData stockData;
	vAllStock = stockData.ReadAllStock();
}



void CWnStockDoc::AddMystock(char* stockCode)
{
	vMystock.push_back(stockCode);
	StockData stockData;
	stockData.WriteMystockCode(vMystock);

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
	
	//�����Ʊ���뵽���ش洢��
	StockData stockData;
	stockData. WriteMystockCode(vMystock);
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
