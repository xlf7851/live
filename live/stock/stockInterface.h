#pragma once

typedef void* HSynCode;

// ������¼�
#define STOCK_CHANGE_EVENT_RESET		0x0001		// ���ô����
#define STOCK_CHANGE_EVENT_CURRENT		0x0002		// �л�ѡ�еĴ���
#define STOCK_CHANGE_EVENT_SCROLL		0x0004		// �����˴����

// ������������
#define STOCK_CODELIST_SCROLL_MASK			0x000000FF
#define STOCK_CODELIST_SCROLL_POS_MASK		0x00000007		// ����λ������
#define STOCK_CODELIST_SCROLL_POS_ONE		0x00000000		// ���λ�ù����������������
#define STOCK_CODELIST_SCROLL_POS_PAGE		0x00000001		// ���λ�ù�������ҳ����
#define STOCK_CODELIST_SCROLL_POS_ABS		0x00000002		// ����λ�ù���
#define STOCK_CODELIST_SCROLL_CYCLE			0x00000008		// ��ѭ������
#define STOCK_CODELIST_SCROLL_LAST_PAGE		0x00000010		// ���������һҳ
#define STOCK_CODELIST_SCROLL_BYCODE		0x00000020		// ������ָ������

#define GET_STOCK_CODELIST_SCROLL_POS(x)    (x&STOCK_CODELIST_SCROLL_POS_MASK)
#define IS_STOCK_CODELIST_SCROLL_CYCLE(x)	((x&STOCK_CODELIST_SCROLL_CYCLE)?true:false)

class IStockChangeEventHandler
{
public:
	virtual void OnStockChangeEvent(HSynCode hSynCode, uint32 uCodeListID,  uint64 uEvent) = 0;
};

namespace stock_wrapper
{
	// code interface
	// ���������
	HSynCode NewSynCode(uint32 id=0);
	void ReleaseSynCode(HSynCode hSynCode);
	// �����id
	void SetCodeDataID(HSynCode hSynCode, uint32 id);
	uint32  GetCodeDataID(HSynCode hSynCode);
	// ������ʺ���
	int GetCodeSize(HSynCode hSynCode);		// ��ȡ�������
	StockCode& GetCodeAt(HSynCode hSynCode, int index);
	bool GetCodeAt(HSynCode hSynCode, int index, StockCode& code);
	StockArray& GetCodeArray(HSynCode hSynCode);
	void GetCodeArray(HSynCode hSyncode, StockArray& ayCode);
	void SetCodeList(HSynCode hSynCode, StockArray& ayCode, uint64 uMode = 0, int nSeltectedIndex = -1, StockCode* pSelectedCode = nullptr);
	// ����������
	int GetCodeScrollIndex(HSynCode hSynCode);	// ��ȡ��ǰ��������������ǰҳ��һ����������
	int GetCodeCurIndex(HSynCode hSynCode);		// ��ȡ��ǰѡ��Ĵ���
	int GetCodePage(HSynCode hSynCode);		// ��ȡҳ��Ĵ������
	void SetCodePage(HSynCode hSynCode, int nPage);
	void GetCodeScrollInfo(HSynCode hSynCode, int* nScroll, int* nPage=nullptr, int* nTotal=nullptr);

	// ������Ϊ���λ�ù����;���λ�ù����Լ�һЩ�����ָ������(������һҳ�����������һҳ�ȵ�)�����⻹��ѭ���������ԡ�������ָ������
	void ScrollCode(HSynCode hSynCode, uint32 uMode, int nPos, StockCode* stockCode = nullptr);
	// �����¼�
	void RegisterStockChangeEventHandler(HSynCode hSynCode, IStockChangeEventHandler* pHandler);
	void UnRegisterStockChangeEventHandler(HSynCode hSynCode, IStockChangeEventHandler* pHandler);
}
