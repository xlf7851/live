#include "stdafx.h"
#include "basePage.h"
#include "../Control/controlGlobalFunc.h"

IMPLEMENT_DUICONTROL(CBasePageUI)
CBasePageUI::CBasePageUI()
{
}

CBasePageUI::~CBasePageUI()
{
	CPaintManagerUI* pManager = GetManager();
	if (pManager)
	{
		pManager->RemoveNotifier(this);
	}
}

LPCTSTR CBasePageUI::GetClass() const
{
	return _T("BasePageUI");
}

LPVOID CBasePageUI::GetInterface(LPCTSTR pstrName)
{
	if (_tcsicmp(pstrName, _T("BasePage")) == 0)
	{
		return this;
	}

	return _BasePageUIBase::GetInterface(pstrName);
}

void CBasePageUI::DoInit()
{
	CPaintManagerUI* pManager = GetManager();
	if (pManager)
	{
		pManager->AddNotifier(this);
	}

	InitPage();
}

void CBasePageUI::Notify(TNotifyUI& msg)
{
	if (IsThisNotify(msg))
	{
		OnNotify(msg);
	}
}

void CBasePageUI::OnNotify(TNotifyUI& msg)
{

}

bool CBasePageUI::IsThisNotify(TNotifyUI& msg)
{
	return msg.pSender == this || control_utl::IsChildControl(this, msg.pSender);
}

HWND CBasePageUI::GetParentHwnd()
{
	return m_pManager ? m_pManager->GetPaintWindow() : nullptr;
}