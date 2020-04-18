#pragma once




class SynCode
{
public:
	SynCode();
	~SynCode();

	uint32 GetDataID();
	void SetDataID(uint32 id);

	int GetCodeSize();		// 获取代码个数
	stock_wrapper::StockCode& GetCodeAt(int index);
	stock_wrapper::StockArray& GetCodeArray();
	void GetCodeArray(stock_wrapper::StockArray& ayCode);
	int FindCodeIndex(const stock_wrapper::StockCode& stockCode);
	int GetScrollIndex();	// 获取当前滚动索引，即当前页第一个代码索引
	int GetCurIndex();		// 获取当前选择的代码
	int GetCodePage();		// 获取页面的代码个数
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
	uint32 m_nID = 0;				// 唯一标识
	int m_nScrollPos = 0;		// 滚动点
	int m_nPage = 1;			// 一页的个数
	int m_nCurpos = -1;			// 当前选中的

	stock_wrapper::StockArray m_ayCode;
	CPtrVector m_vcEventHandler;
};

