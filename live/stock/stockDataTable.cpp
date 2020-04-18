#include "stdafx.h"
#include "stockDataTable.h"


CStockDataTableVirtualListDataProvider::CStockDataTableVirtualListDataProvider(CStockDataTableUI* pTable)
{
	m_pTable = pTable;
}

CControlUI* CStockDataTableVirtualListDataProvider::CreateElement()
{
	return m_pTable ? m_pTable->CreateRow() : nullptr;
}

void CStockDataTableVirtualListDataProvider::FillElement(CListUI* pList, CControlUI *pControl, int index)
{
	if (m_pTable)
	{
		m_pTable->FillRow(pList, pControl, index);
	}
}

int CStockDataTableVirtualListDataProvider::GetElementCount()
{
	return m_pTable ? m_pTable->GetDataCount() : 0;
}



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
	m_uCodeList = 0;
	SetElementHeight(24);
	m_pDataProvider = new CStockDataTableVirtualListDataProvider(this);
	SetDataProvider(m_pDataProvider);
}

CStockDataTableUI::~CStockDataTableUI()
{
	if (m_pDataProvider)
	{
		delete m_pDataProvider;
	}
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

void CStockDataTableUI::SetPos(RECT rc, bool bNeedInvalidate /* = true */)
{
	CVerticalLayoutUI::SetPos(rc, bNeedInvalidate);
	_SetPos(bNeedInvalidate);
}

CListContainerElementUI* CStockDataTableUI::CreateRow()
{
	CListContainerElementUI* pNew = new CListContainerElementUI;
	pNew->SetChildVAlign(DT_VCENTER);
	int nCnt = GetHeader()->GetCount();
	for (int i = 0; i < nCnt; i++)
	{
		CControlUI* pItem = CreaterColumn(i);
		if (pItem == nullptr)
		{
			pItem = new CLabelUI;
		}
		pNew->Add(pItem);
	}
	
	return pNew;
}

CControlUI* CStockDataTableUI::CreaterColumn(int nColumn)
{
	return new CLabelUI;
}

void CStockDataTableUI::FillRow(CListUI* pList, CControlUI *pControl, int index)
{
	CListContainerElementUI* pHor = static_cast<CListContainerElementUI*>(pControl->GetInterface(DUI_CTR_LISTCONTAINERELEMENT));
	if (pHor)
	{
		int nCnt = GetHeader()->GetCount();
		for (int i = 0; i < nCnt; i++)
		{
			UpdateColumnData(pHor->GetItemAt(i), index, i);
		}
	}
}

void CStockDataTableUI::UpdateColumnData(CControlUI* pColumnItem, int nRow, int nColumn)
{
	stock_wrapper::StockCode code = GetStockCodeAt(nRow);
	if (code.Empty())
	{
		return;
	}

	LPCTSTR lpszCode = code.GetStock().ToCString();
	CDuiString strText;
	CLabelUI* pLabel = (CLabelUI*)pColumnItem->GetInterface(DUI_CTR_LABEL);
	if (pLabel)
	{
		if (nColumn == 0)
		{
			strText.Format(_T("%d"), nRow);
			pLabel->SetText(strText);
		}
		else if (nColumn == 1)
		{
			pLabel->SetText(lpszCode);
		}
	}
}

void CStockDataTableUI::ReBuildControl()
{
	RemoveAll();
	InitElement(15);
	//Refresh();
}

CStockPageBaseUI* CStockDataTableUI::GetParentStockPage()
{
	CStockPageBaseUI* pFind = nullptr;
	CControlUI* pCurent = GetParent();
	while (pCurent)
	{
		if (pCurent->GetInterface(DUI_CUSTOM_CTRL_INTERFACE_StockPageBase) != nullptr)
		{
			pFind = (CStockPageBaseUI*)pCurent;
			break;
		}

		pCurent = pCurent->GetParent();
	}

	return pFind;
}

HSynCode CStockDataTableUI::GetSynCode()
{
	if (m_uCodeList != 0)
	{
		CStockPageBaseUI* pPage = GetParentStockPage();
		if (pPage)
		{
			return pPage->FindSynCode(m_uCodeList);
		}
	}

	return nullptr;
}

void CStockDataTableUI::InitTable()
{
	
}

void CStockDataTableUI::ReLoadCodeList()
{
	m_nStockSizeCache = 0;
	HSynCode hSynCode = GetSynCode();
	if (hSynCode)
	{
		m_nStockSizeCache = stock_wrapper::GetCodeSize(hSynCode);
	}

	ReBuildControl();
}

void CStockDataTableUI::InvalidTable()
{
	
}

void CStockDataTableUI::UpdateCodeList(stock_wrapper::StockArray& ayCode)
{
	HSynCode hSynCode = GetSynCode();
	if (hSynCode)
	{
		stock_wrapper::SetCodeList(hSynCode, ayCode);
	}
}

void CStockDataTableUI::SetCodeListID(uint32 codeid)
{
	m_uCodeList = codeid;
}

uint32 CStockDataTableUI::GetCodeListID()
{
	return m_uCodeList;
}


stock_wrapper::StockCode CStockDataTableUI::GetStockCodeAt(int Index)
{
	stock_wrapper::StockCode code;
	HSynCode hSynCode = GetSynCode();
	if ( hSynCode)
	{
		GetCodeAt(hSynCode, Index, code);
	}

	return code;
}

int CStockDataTableUI::GetDataCount()
{
	return m_nStockSizeCache;
}