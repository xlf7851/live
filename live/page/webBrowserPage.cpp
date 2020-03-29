#include "stdafx.h"
#include "webBrowserPage.h"
#include "../browser/browserwindow.h"


IMPLEMENT_DUICONTROL(CWebBrowserPageUI)

CWebBrowserPageUI::CWebBrowserPageUI()
{
	m_pBrowser = NULL;
	m_pUrlEditBox = NULL;
	m_bCanShowSearchBox = false;
	m_bTabShowSearchBox = false;
	m_pSerchBoxModule = nullptr;
}

CWebBrowserPageUI::~CWebBrowserPageUI()
{
	if (m_pManager)
	{
		m_pManager->RemovePreMessageFilter(this);
	}
}

LPCTSTR CWebBrowserPageUI::GetClass() const
{
	return DUI_CUSTOM_CTRL_CLASS_WebBrowserPage;
}

LPVOID CWebBrowserPageUI::GetInterface(LPCTSTR pstrName)
{
	if (_tcsicmp(pstrName, DUI_CUSTOM_CTRL_INTERFACE_WebBrowserPage) == 0) return static_cast<CWebBrowserPageUI*>(this);
	return CBasePageUI::GetInterface(pstrName);
}

void CWebBrowserPageUI::InitPage()
{
	m_pBrowser = static_cast<CBrowserUI*>(control_utl::FindChildByName(this, _T("webBrowser")));
	m_pUrlEditBox = static_cast<CEditUI*>(control_utl::FindChildByName(this, _T("WebBrowserPageurlBox")));
	m_pSerchBoxModule = control_utl::FindChildByName(this, _T("WebBrowserPageSearchModule"));
	if (m_pBrowser && !m_strUrl.IsEmpty())
	{
		m_pBrowser->Navigate(m_strUrl);
	}
	if (m_pUrlEditBox)
	{
		m_pUrlEditBox->SetPreMessageHandler(this);
	}

	if (m_pSerchBoxModule && !m_bCanShowSearchBox && m_pSerchBoxModule->IsVisible())
	{
		m_pSerchBoxModule->SetVisible(false);
	}

	if (m_pManager)
	{
		m_pManager->AddPreMessageFilter(this);
	}
}

void CWebBrowserPageUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	if (_tcsicmp(pstrName, _T("canshowsearchbox")) == 0)
	{
		SetCanShowSearchBox(_tcsicmp(pstrValue, _T("true")) == 0);
	}
	else if (_tcsicmp(pstrName, _T("tabshowsearchbox")) == 0)
	{
		SetTabShowSearchBox(_tcsicmp(pstrValue, _T("true")) == 0);
	}
	else
	{
		CBasePageUI::SetAttribute(pstrName, pstrValue);
	}
}

void CWebBrowserPageUI::SetCanShowSearchBox(bool bEnable)
{
	if (m_bCanShowSearchBox != bEnable)
	{
		m_bCanShowSearchBox = bEnable;
		if (m_pSerchBoxModule && !m_bCanShowSearchBox && m_pSerchBoxModule->IsVisible())
		{
			m_pSerchBoxModule->SetVisible(false);
		}
	}
}

void CWebBrowserPageUI::SetTabShowSearchBox(bool bEnable)
{
	m_bTabShowSearchBox = bEnable;
}

void CWebBrowserPageUI::OnNotify(TNotifyUI& msg)
{
}

void CWebBrowserPageUI::Navigate(LPCTSTR lpszUrl)
{
	if (m_pBrowser)
	{
		m_pBrowser->Navigate(lpszUrl);
	}
	else
	{
		m_strUrl = lpszUrl;
	}
}

LRESULT CWebBrowserPageUI::EditMessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
	if (uMsg == WM_KEYDOWN)
	{
		if (wParam == VK_RETURN)
		{
			bHandled = true;
			OnKeyReturn();
			return 0;
		}
		else if (wParam == VK_TAB)
		{
			bHandled = true;
			return 1;
		}
	}
	return 0;
}

void CWebBrowserPageUI::OnKeyReturn()
{
	if (m_pUrlEditBox)
	{
		CDuiString strUrl = m_pUrlEditBox->GetText();
		if (!strUrl.IsEmpty())
		{
			m_strUrl = strUrl;
			Navigate(m_strUrl);
		}
	}
}

LRESULT CWebBrowserPageUI::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM, bool& bHandled)
{
	if (uMsg == WM_KEYDOWN && IsVisible())
	{
		if (wParam == VK_TAB && m_bCanShowSearchBox && m_bTabShowSearchBox && (m_pUrlEditBox == nullptr || !m_pUrlEditBox->IsFocused()))
		{
			bHandled = true;
			AutoShowHideSearchModule();
			return 0;
		}
	}

	return 0;
}

void CWebBrowserPageUI::AutoShowHideSearchModule()
{
	if (m_pSerchBoxModule)
	{
		m_pSerchBoxModule->SetVisible(!m_pSerchBoxModule->IsVisible());
	}
}
