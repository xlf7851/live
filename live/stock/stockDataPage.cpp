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
	
}


void CStockDataPageUI::OnTestMinData()
{
	stock_wrapper::StockDataPool* stockDataPool = stock_wrapper::StockDataPool::Instance();
	if (stockDataPool == nullptr)
	{
		return;
	}
}

void CStockDataPageUI::OnTestDayData()
{
	stock_wrapper::StockDataPool* stockDataPool = stock_wrapper::StockDataPool::Instance();
	if (stockDataPool == nullptr)
	{
		return;
	}
}