#pragma once

namespace stock_wrapper
{
	class MarketDayData;
}

class CStockDataTableHeaderUI : public CListHeaderItemUI
{
	DECLARE_DUICONTROL(CStockDataTableHeaderUI)
public:
	CStockDataTableHeaderUI();

	virtual LPCTSTR GetClass() const;
	virtual LPVOID GetInterface(LPCTSTR pstrName);
};

class CStockDayDataTableDataHandler : public IVirtualListDataHandler
{
public:
	virtual void UpdateColumnData(CControlUI* pColumnItem, int nRow, int nColumn);
	virtual int GetDataCount();

protected:
	stock_wrapper::DayDataArray m_dayData;
};

class CStockDataTableUI : public CVirtualListUI, public IVirtualListColumnCreater
{
	DECLARE_DUICONTROL(CStockDataTableUI)
public:
	CStockDataTableUI();
	~CStockDataTableUI();

	virtual LPCTSTR GetClass() const;
	virtual LPVOID GetInterface(LPCTSTR pstrName);

	CControlUI* CreaterColumn(int nColumn);
//	virtual IVirtualListDataHandler* CreateDataHandler();

protected:
	stock_wrapper::Stock m_stock;
	CVirtualListContainerElementDataProvider* m_pDataProvider;
};