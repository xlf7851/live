#include "stdafx.h"
#include "stockDataPage.h"
#include "../control/controlGlobalFunc.h"
#include <TlHelp32.h>
#include <tinyxml2.h>
#include "hqDataQuery.h"
#include "../Control/openFileDlg.h"
#include "stockdata.h"
#include "stockDataTable.h"



IMPLEMENT_DUICONTROL(CStockDataPageUI)

CStockDataPageUI::CStockDataPageUI()
{
	m_dataHwnd = nullptr;
	m_pLogEdit = nullptr;
	m_pDayDataTable = nullptr;
	m_pStockLabel = nullptr;
}

CStockDataPageUI::~CStockDataPageUI()
{

}

LPCTSTR CStockDataPageUI::GetClass() const
{
	return DUI_CUSTOM_CTRL_CLASS_StockDataPage;
}

LPVOID CStockDataPageUI::GetInterface(LPCTSTR pstrName)
{
	if (_tcsicmp(pstrName, DUI_CUSTOM_CTRL_INTERFACE_StockDataPage) == 0) return static_cast<CStockDataPageUI*>(this);
	return _StockDataPageUIBase::GetInterface(pstrName);

	
}

void CStockDataPageUI::InitPage()
{
	m_pLogEdit = static_cast<CRichEditUI*>(control_utl::FindChildByName(this, _T("stockhisdataPageEditLog")));
	m_pDayDataTable = static_cast<CStockDataTableUI*>(control_utl::FindChildByName(this, _T("StockhisdataPageDayTable")));
	m_pStockLabel = static_cast<CLabelUI*>(control_utl::FindChildByName(this, _T("StockhisdataPageStockLabel")));
}

void CStockDataPageUI::OnNotify(TNotifyUI& msg)
{
	CDuiString name = msg.pSender->GetName();
	if (msg.sType == DUI_MSGTYPE_CLICK)
	{
		if (name.CompareNoCase(_T("stockhisdataPageFindWnd"))== 0)
		{
			FindGetDataWindow();
		}
		else if (name.CompareNoCase(_T("btnStockhisdataPageLoad")) == 0)
		{
			OnLoadData();
		}
		else if (name.CompareNoCase(_T("btnStockhisdataPageTestDayData")) == 0)
		{
			OnTestDayData();
		}
		else if (name.CompareNoCase(_T("btnStockhisdataPageTestMinData")) == 0)
		{
			OnTestMinData();
		}
	}
 }



void CStockDataPageUI::FindGetDataWindow()
{
	if (m_dataHwnd == nullptr)
	{
		m_dataHwnd = xlf::TSingleton<xlf::CGlobalData>::Instance()->GetHqDataIpcDestHwnd();
		CDuiString strLog;
		strLog.Format(_T("´°¿Ú¾ä±ú£º0x%x\r\n"), m_dataHwnd);
		AddLog(strLog);
	}

	if (m_dataHwnd)
	{
		stock_wrapper::StockArray ayCode;
		//ayCode.AddStockCode(_T("300033"));
		std::vector<DWORD> vcDataItem;
		vcDataItem.push_back(10);

		stock_wrapper::CHqDataQueryHandler::GetHqDataQueryHandler().SetCopyDataDstHwnd(m_dataHwnd);
		stock_wrapper::QueryHqData(ayCode, vcDataItem, 16384, 20200304, 0, NULL);
	}
}

void CStockDataPageUI::AddLog(LPCTSTR lpszLog)
{
	if (nullptr == m_pLogEdit)
	{
		return;
	}

	m_pLogEdit->AppendText(lpszLog);

}

void CStockDataPageUI::OnLoadData()
{
	COpenFileHelp openFileHelp;
	if (!openFileHelp.BrowseForFolder())
	{
		return;
	}

	CDuiString strPath = openFileHelp.GetFilePath();
	if (strPath.IsEmpty())
	{
		return;
	}

	LoadPoolData(strPath);
}


void CStockDataPageUI::LoadPoolData(LPCTSTR lpszFile)
{
	stock_wrapper::StockDataPool* pPool = stock_wrapper::StockDataPool::Instance();
	if (pPool)
	{
		pPool->Clear();
		pPool->InitDataPath(lpszFile);
		pPool->LoadData();
	}
}

void CStockDataPageUI::UpdateStockText(const stock_wrapper::Stock& stock)
{
	if (m_pStockLabel)
	{
		m_pStockLabel->SetText(stock);
	}
}

void CStockDataPageUI::ShowDayData(stock_wrapper::MarketDayData& data, const stock_wrapper::Stock& stock)
{
	if (m_pDayDataTable == NULL)
	{
		return;
	}
	//m_pDayDataTable->RemoveAll();
	stock_wrapper::DayDataArray* pArray = (stock_wrapper::DayDataArray*)data.GetData(stock);
	if (pArray)
	{
		
		CDuiString strLog;
		strLog.Format(_T("%s day data:\r\n"), (LPCTSTR)stock);
		for (int i = 0; i < pArray->GetSize(); i++)
		{
// 			stock_wrapper::_day_data_item_t* pItem = pArray->GetAt(i);
// 			CListTextElementUI* pLabelItem = new CListTextElementUI;
// 			//pLabelItem->SetOwner(m_pDayDataTable);
// 			m_pDayDataTable->Add(pLabelItem);
// 
// 			CDuiString strItem;
// 			strItem.Format(_T("%d"), pItem->m_lDate);
// 			pLabelItem->SetText(0, strItem);
// 
// 			strItem.Format(_T("%.2lf"), pItem->m_open);
// 			pLabelItem->SetText(1, strItem);
// 
// 			strItem.Format(_T("%.2lf"), pItem->m_close);
// 			pLabelItem->SetText(2, strItem);
// 
// 			strItem.Format(_T("%.2lf"), pItem->m_vol);
// 			pLabelItem->SetText(3, strItem);
// 
// 			strItem.Format(_T("%.2lf"), pItem->m_money);
// 			pLabelItem->SetText(4, strItem);
// 
// 			strItem.Format(_T("%.2lf"), pItem->m_change);
// 			pLabelItem->SetText(5, strItem);
// 
// 			strItem.Format(_T("%.2lf"), pItem->m_high);
// 			pLabelItem->SetText(6, strItem);
// 
// 			strItem.Format(_T("%.2lf"), pItem->m_low);
// 			pLabelItem->SetText(7, strItem);
// 
// 			strItem.Format(_T("%.2lf"), pItem->m_outVol);
// 			pLabelItem->SetText(8, strItem);
		}

	}
}

void CStockDataPageUI::ShowMinuteData(stock_wrapper::MarketMinuteData& data, const stock_wrapper::Stock& stock)
{
	if (m_pDayDataTable == NULL)
	{
		return;
	}
	stock_wrapper::MinuteDataArray* pArray = (stock_wrapper::MinuteDataArray*)data.GetData(stock);
	if (pArray)
	{

		CDuiString strLog;
		strLog.Format(_T("%s minute data:\r\n"), (LPCTSTR)stock);
		for (int i = 0; i < pArray->GetSize(); i++)
		{
			stock_wrapper::_minute_data_node_t* node = pArray->GetAt(i);
			long lData = node->m_lDate;
		}

	}
}

void CStockDataPageUI::OnTestMinData()
{
	stock_wrapper::StockDataPool* stockDataPool = stock_wrapper::StockDataPool::Instance();
	if (stockDataPool == nullptr)
	{
		return;
	}

// 	std::string market = _T("USHA");
// 	stock_wrapper::Stock stock = _T("000597");
// 
// 	stock_wrapper::MinuteDataArray ayMin;
// 	stockDataPool->GetMinuteData(market, stock, ayMin);
// 
// 	stock_wrapper::_minute_data_node_t data;
// 	stockDataPool->GetMinuteData(market, stock, 20200318, data);


}

void CStockDataPageUI::OnTestDayData()
{
	stock_wrapper::StockDataPool* stockDataPool = stock_wrapper::StockDataPool::Instance();
	if (stockDataPool == nullptr)
	{
		return;
	}
}