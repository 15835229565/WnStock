
// WnStockDoc.h : CWnStockDoc ��Ľӿ�
//


#pragma once


class CWnStockDoc : public CDocument
{
protected: // �������л�����
	CWnStockDoc();
	DECLARE_DYNCREATE(CWnStockDoc)

// ����
public:

// ����
public:

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// ʵ��
public:
	virtual ~CWnStockDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// ����Ϊ����������������������ݵ� Helper ����
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS

	
public:
	StockDoc* stockDoc;
	/****************�Զ�������ͺ���******************/
	
//��ѡ�������������ͺ���
	vector<string>	vMystock;
	void			ReadMystock();					//��ȡ��ѡ�ɹ�Ʊ����
	void			WriteMystock();					//������ѡ�ɵ��������ݿ�(sqlite)
	void			GetMytockData();				//��ȡ������ѡ�ɵ�����
	vector<RealTimeStock*> vMystockRealTimeData;	//��ѡ��ʵʱ��������

	//��������
	void AddMystock(char* stockCode);				//�Ҽ������������ѡ��ʹ��
	void DelMystock(char* stockCode);				//�Ҽ�������ɾ����ѡ��ʹ��
};
