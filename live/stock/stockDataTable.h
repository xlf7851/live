#pragma once


class CStockDataTableUI;
class CStockDataTableVirtualListDataProvider : public IVirtualDataProvider
{
public:
	CStockDataTableVirtualListDataProvider(CStockDataTableUI* pTable);
	
	virtual CControlUI* CreateElement();
	virtual void FillElement(CListUI* pList, CControlUI *pControl, int index);
	virtual int GetElementCount();

protected:
	CStockDataTableUI* m_pTable = nullptr;
};

class CStockDataTableHeaderUI : public CListHeaderItemUI
{
	DECLARE_DUICONTROL(CStockDataTableHeaderUI)
public:
	CStockDataTableHeaderUI();

	virtual LPCTSTR GetClass() const;
	virtual LPVOID GetInterface(LPCTSTR pstrName);
};

class CStockPageBaseUI;
class CStockDataTableUI : public CVirtualListUI
{
	DECLARE_DUICONTROL(CStockDataTableUI)
public:
	CStockDataTableUI();
	~CStockDataTableUI();

	virtual LPCTSTR GetClass() const;
	virtual LPVOID GetInterface(LPCTSTR pstrName);
	virtual void SetPos(RECT rc, bool bNeedInvalidate = true);

	CListContainerElementUI* CreateRow();
	virtual CControlUI* CreaterColumn(int nColumn);
	virtual void FillRow(CListUI* pList, CControlUI *pControl, int index);
	virtual void UpdateColumnData(CControlUI* pColumnItem, int nRow, int nColumn);
	void ReBuildControl();

	void InitTable();
	void InvalidTable();
	void ReLoadCodeList();
	void UpdateCodeList(stock_wrapper::StockArray& ayCode);

	CStockPageBaseUI* GetParentStockPage();
	virtual HSynCode GetSynCode();
	uint32 GetCodeListID();
	void SetCodeListID(uint32 codeid);
	
	virtual int GetDataCount();
	stock_wrapper::StockCode GetStockCodeAt(int Index);
protected:
	CStockDataTableVirtualListDataProvider* m_pDataProvider = nullptr;
	uint32 m_uCodeList = 0;
	int m_nStockSizeCache = 0;
	int m_nPageRow = 1;
};