#pragma once




class SynCode
{
public:
	SynCode();
	~SynCode();

	uint32 GetDataID();
	void SetDataID(uint32 id);

	int GetCodeSize();		// ��ȡ�������
	stock_wrapper::StockCode& GetCodeAt(int index);
	stock_wrapper::StockArray& GetCodeArray();
	void GetCodeArray(stock_wrapper::StockArray& ayCode);
	int FindCodeIndex(const stock_wrapper::StockCode& stockCode);
	int GetScrollIndex();	// ��ȡ��ǰ��������������ǰҳ��һ����������
	int GetCurIndex();		// ��ȡ��ǰѡ��Ĵ���
	int GetCodePage();		// ��ȡҳ��Ĵ������
	void SetCodePage(int nPage);
	void GetScrollInfo(int* nScroll, int* nPage, int* nTotal);

	void ScrollCode(uint32 uMode, int nPos, stock_wrapper::StockCode* stockCode = nullptr);
	void SetCodeList(stock_wrapper::StockArray& ayCode, uint64 uMode = 0, int nSeltectedIndex = -1, stock_wrapper::StockCode* pSelectedCode = nullptr);
	void Empty();

	void OnStockChange(uint64 uEvent);
	void RegisterStockChangeEventHandler(IStockChangeEventHandler* pHandler);
	void UnRegisterStockChangeEventHandler(IStockChangeEventHandler* pHandler);

protected:
	bool _ScrollCode(uint32 uMode, int nPos);
	bool _ScrollByCode(const stock_wrapper::StockCode& stockCode);
	int _CalcLastPageFirstIndex();
	int _CalcScrollPos(int nPos, uint32 uPosOption, bool bCycle = false);
	void _FixScrollAndSelected();

protected:
	uint32 m_nID = 0;				// Ψһ��ʶ
	int m_nScrollPos = 0;		// ������
	int m_nPage = 1;			// һҳ�ĸ���
	int m_nCurpos = -1;			// ��ǰѡ�е�

	stock_wrapper::StockArray m_ayCode;
	CPtrVector m_vcEventHandler;
};

