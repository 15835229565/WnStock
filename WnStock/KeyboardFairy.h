#pragma once


// CKeyboardFairy �Ի���

class CKeyboardFairy : public CDialogEx
{
	DECLARE_DYNAMIC(CKeyboardFairy)

public:
	CKeyboardFairy(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CKeyboardFairy();

// �Ի�������
	enum { IDD = IDD_DLG_KEYBOARDFAIRY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_stockInfo;
	CListBox m_stockList;
	afx_msg void OnChangeInputStockInfo();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	CFont font;
	afx_msg void OnDblclkListStock();
};
