#include "stdafx.h"
#include "toolbar.h"
#include "../global/functioncall.h"

IMPLEMENT_DUICONTROL(CToolbarItemUI)
CToolbarItemUI::CToolbarItemUI()
{
}

CToolbarItemUI::~CToolbarItemUI()
{

}

LPCTSTR CToolbarItemUI::GetClass() const
{
	return DUI_CUSTOM_CTRL_CLASS_ToolbarItem;
}

LPVOID CToolbarItemUI::GetInterface(LPCTSTR pstrName)
{
	if (_tcsicmp(pstrName, DUI_CUSTOM_CTRL_INTERFACE_ToolbarItem) == 0)
	{
		return this;
	}

	return CButtonUI::GetInterface(pstrName);
}

void CToolbarItemUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	if (_tcsicmp(pstrName, FUNCTION_CALL_CallName) == 0)
	{
		m_functionCall.SetName(pstrValue);
	}
	else if (_tcsicmp(pstrName, FUNCTION_CALL_CallParam) == 0)
	{
		m_functionCall.ParseParam(pstrValue);
	}
	else
	{
		CButtonUI::SetAttribute(pstrName, pstrValue);
	}
}

CFunctionCallItem& CToolbarItemUI::GetFunctionCall()
{
	return m_functionCall;
}


IMPLEMENT_DUICONTROL(CToolbarPageUI)
CToolbarPageUI::CToolbarPageUI()
{
}

CToolbarPageUI::~CToolbarPageUI()
{
	
}

LPCTSTR CToolbarPageUI::GetClass() const
{
	return _T("ToolbarPageUI");
}

LPVOID CToolbarPageUI::GetInterface(LPCTSTR pstrName)
{
	if (_tcsicmp(pstrName, _T("ToolbarPage")) == 0)
	{
		return this;
	}

	return _ToolbarPageBase::GetInterface(pstrName);
}

void CToolbarPageUI::OnNotify(TNotifyUI& msg)
{
	CToolbarItemUI* pItem = static_cast<CToolbarItemUI*>(msg.pSender->GetInterface(DUI_CUSTOM_CTRL_INTERFACE_ToolbarItem));
	if (nullptr == pItem)
	{
		return;
	}

	if (!control_utl::IsChildControl(this, pItem))
	{
		return;
	}
	CDuiString name = msg.pSender->GetName();
	if (msg.sType == DUI_MSGTYPE_CLICK)
	{
		OnClickToolBarItem(pItem);
	}
}

void CToolbarPageUI::OnClickToolBarItem(CToolbarItemUI* pItem)
{
	if (pItem == nullptr)
	{
		return;
	}

	CFunctionCall::Call(pItem->GetFunctionCall());
}
