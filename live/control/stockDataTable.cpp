#include "stdafx.h"
#include "stockDataTable.h"



IMPLEMENT_DUICONTROL(CStockDataTableHeaderUI)
CStockDataTableHeaderUI::CStockDataTableHeaderUI()
{

}

LPCTSTR CStockDataTableHeaderUI::GetClass() const
{
	return DUI_CUSTOM_CTRL_CLASS_StockDataTableHeader;
}

LPVOID CStockDataTableHeaderUI::GetInterface(LPCTSTR pstrName)
{
	if (_tcsicmp(pstrName, DUI_CUSTOM_CTRL_INTERFACE_StockDataTableHeader) == 0) return static_cast<CStockDataTableHeaderUI*>(this);
	return CListHeaderItemUI::GetInterface(pstrName);
}


IMPLEMENT_DUICONTROL(CStockDataTableUI)

CStockDataTableUI::CStockDataTableUI()
{
	m_pDataProvider = new CVirtualListContainerElementDataProvider;
	SetDataProvider(m_pDataProvider);
}

CStockDataTableUI::~CStockDataTableUI()
{

}

LPCTSTR CStockDataTableUI::GetClass() const
{
	return DUI_CUSTOM_CTRL_CLASS_StockDataTable;
}

LPVOID CStockDataTableUI::GetInterface(LPCTSTR pstrName)
{
	if (_tcsicmp(pstrName, DUI_CUSTOM_CTRL_INTERFACE_StockDataTable) == 0) return static_cast<CStockDataTableUI*>(this);
	return CVirtualListUI::GetInterface(pstrName);
}

CControlUI* CStockDataTableUI::CreaterColumn(int nColumn)
{
	return new CLabelUI;
}