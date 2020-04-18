#include "stdafx.h"
#include "stockPageBase.h"

IMPLEMENT_DUICONTROL(CStockPageBaseUI)

CStockPageBaseUI::CStockPageBaseUI()
{
}

CStockPageBaseUI::~CStockPageBaseUI()
{
	ReleaseSynCode();
}

LPCTSTR CStockPageBaseUI::GetClass() const
{
	return DUI_CUSTOM_CTRL_CLASS_StockPageBase;
}

LPVOID CStockPageBaseUI::GetInterface(LPCTSTR pstrName)
{
	if (_tcsicmp(pstrName, DUI_CUSTOM_CTRL_INTERFACE_StockPageBase) == 0) return static_cast<CStockPageBaseUI*>(this);
	return CBasePageUI::GetInterface(pstrName);
}


HSynCode CStockPageBaseUI::AddSynCode(uint32& codelistid)
{
	uint32 uid = NewSynCodeID();
	HSynCode hNew = stock_wrapper::NewSynCode(uid);
	if (hNew)
	{
		stock_wrapper::RegisterStockChangeEventHandler(hNew, this);
		m_vcSyncode.push_back(hNew);
		codelistid = uid;
	}
	return hNew;
}

HSynCode CStockPageBaseUI::FindSynCode(uint32 codelistid)
{
	for (void* pCode : m_vcSyncode)
	{
		HSynCode hCode = (HSynCode)pCode;
		if (hCode && stock_wrapper::GetCodeDataID(hCode) == codelistid)
		{
			return hCode;
		}
	}

	return nullptr;
}

void CStockPageBaseUI::ReleaseSynCode()
{
	for (void* pCode : m_vcSyncode)
	{
		if (pCode)
		{
			stock_wrapper::ReleaseSynCode((HSynCode)pCode);
		}
	}
}

uint32 CStockPageBaseUI::NewSynCodeID()
{
	uint32 id = 1;
	while (FindSynCode(id) != nullptr)
	{
		id++;
		break;
	}
	return id;
}