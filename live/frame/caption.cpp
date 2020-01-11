#include "stdafx.h"
#include "caption.h"
#include "../control/controlGlobalFunc.h"
#include "../skin/skinSetWnd.h"

IMPLEMENT_DUICONTROL(CCaptionPageUI)
CCaptionPageUI::CCaptionPageUI()
{
	
}

CCaptionPageUI::~CCaptionPageUI()
{
	CPaintManagerUI* pManager = GetManager();
	if (pManager)
	{
		pManager->RemoveNotifier(this);
	}
}

LPCTSTR CCaptionPageUI::GetClass() const
{
	return _T("CaptionPageUI");
}

LPVOID CCaptionPageUI::GetInterface(LPCTSTR pstrName)
{
	if (_tcsicmp(pstrName, _T("CaptionPage")) == 0)
	{
		return this;
	}

	return _CaptionPageBase::GetInterface(pstrName);
}

void CCaptionPageUI::Notify(TNotifyUI& msg)
{
	if (!control_utl::IsChildControl(this, msg.pSender))
	{
		return;
	}
	CDuiString name = msg.pSender->GetName();
	if (msg.sType == DUI_MSGTYPE_CLICK)
	{
		if (name == _T("cpationSkin"))
		{
			OnSkinSetting();
		}
		else if (name == _T("cpationSettings"))
		{
			OnSetting();
		}
	}

}

void CCaptionPageUI::DoInit()
{
	CPaintManagerUI* pManager = GetManager();
	if (pManager)
	{
		pManager->AddNotifier(this);
	}
}

void CCaptionPageUI::OnSkinSetting()
{
	HWND hParent = m_pManager ? m_pManager->GetPaintWindow() : NULL;
	CSkinSetWnd dlg;
	dlg.CreateDuiWindow(hParent, _T("∆§∑Ù…Ë÷√"));
	dlg.CenterWindow();
	dlg.ShowModal();
	
}

void CCaptionPageUI::OnSetting()
{

}