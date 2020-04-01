#include "stdafx.h"
#include "browserwindow.h"
#include "browserModule.h"

CBrowserWnd::CBrowserWnd(CBrowserWnd** pSelfPtr /* = nullptr */)
{
	m_hBrowser = NULL;
	m_pSelfPtr = pSelfPtr;
}

CBrowserWnd::~CBrowserWnd()
{
	
}

void CBrowserWnd::Navigate(LPCTSTR lpszUrl)
{
	m_strUrl = lpszUrl;
	if (m_hBrowser)
	{
		CBrowserModule::Instance()->Navigate(m_hBrowser, lpszUrl);
	}
}

LPCTSTR CBrowserWnd::GetWindowClassName() const
{
	return _T("BrowserBaseWnd");
}

void CBrowserWnd::OnFinalMessage(HWND hWnd)
{
	CWindowWnd::OnFinalMessage(hWnd);
	if (m_pSelfPtr)
	{
		*m_pSelfPtr = nullptr;
		delete this;
	}
}

LRESULT CBrowserWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_CREATE) {
		CreateBrowser();
	}
	else if (uMsg == WM_SIZE)
	{
		SetBrowserPos();
	}
	else if (uMsg == WM_SHOWWINDOW)
	{
		ShowBrowser(wParam != 0);
	}
	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

void CBrowserWnd::CreateBrowser()
{
	if (m_hBrowser)
	{
		return;
	}
	
	m_hBrowser = CBrowserModule::Instance()->CreateBrowser(GetHWND());
	if (m_hBrowser && !m_strUrl.IsEmpty())
	{
		CBrowserModule::Instance()->Navigate(m_hBrowser, m_strUrl);
	}
}

void CBrowserWnd::SetBrowserPos()
{
	if (m_hBrowser)
	{
		RECT rc;
		GetClientRect(GetHWND(), &rc);
		CBrowserModule::Instance()->MoveWindow(m_hBrowser, rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top);
	}
}

void CBrowserWnd::ShowBrowser(bool bShow)
{

}


IMPLEMENT_DUICONTROL(CBrowserUI)
CBrowserUI::CBrowserUI()
{
	m_pBrowserWnd = NULL;
}

CBrowserUI::~CBrowserUI()
{
	DestoryBrowser();
}

void CBrowserUI::DoInit()
{
	CreateBrowser();
}

void CBrowserUI::InitBrowser(LPCTSTR lpszUrl)
{
	CreateBrowser();
	if (m_pBrowserWnd)
	{
		m_pBrowserWnd->Navigate(lpszUrl);
	}
}


LPCTSTR CBrowserUI::GetClass() const
{
	return DUI_CUSTOM_CTRL_CLASS_Browser;
}

LPVOID CBrowserUI::GetInterface(LPCTSTR pstrName)
{
	if (_tcsicmp(pstrName, DUI_CUSTOM_CTRL_INTERFACE_Browser) == 0)
	{
		return this;
	}

	return CControlUI::GetInterface(pstrName);
}

CBrowserWnd* CBrowserUI::GetBrowserWnd()
{
	return m_pBrowserWnd;
}

void CBrowserUI::Navigate(LPCTSTR lpszUrl)
{
	m_strUrl = lpszUrl;
	if (m_pBrowserWnd)
	{
		m_pBrowserWnd->Navigate(lpszUrl);
	}
}

void CBrowserUI::SetPos(RECT rc, bool bNeedInvalidate /* = true */)
{
	CControlUI::SetPos(rc, bNeedInvalidate);
	SetBrowserPos();
}

void CBrowserUI::Move(SIZE szOffset, bool bNeedInvalidate /* = true */)
{
	CControlUI::Move(szOffset, bNeedInvalidate);
	SetBrowserPos();
}

void CBrowserUI::SetVisible(bool bVisible /* = true */)
{
	CControlUI::SetInternVisible(bVisible);
	ShowBrowser(bVisible);
}

void CBrowserUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	if (_tcsicmp(pstrName, _T("url")) == 0)
	{
		Navigate(pstrValue);
	}
	else
	{
		CControlUI::SetAttribute(pstrName, pstrValue);
	}
}

void CBrowserUI::ShowBrowser(bool bShow)
{
	if (m_pBrowserWnd && m_pBrowserWnd->GetHWND())
	{
		ShowWindow(m_pBrowserWnd->GetHWND(), bShow ? SW_SHOW : SW_HIDE);
	}
}

void CBrowserUI::CreateBrowser()
{
	if (m_pManager == NULL)
	{
		return;
	}

	if (m_pBrowserWnd == NULL)
	{
		m_pBrowserWnd = new CBrowserWnd(&m_pBrowserWnd);
	}

	if (m_pBrowserWnd->GetHWND() != NULL)
	{
		return;
	}

	if (!m_strUrl.IsEmpty())
	{
		m_pBrowserWnd->Navigate(m_strUrl);
	}
	
	m_pBrowserWnd->CreateDuiWindow(m_pManager->GetPaintWindow(), _T("BrowserBaseWnd"), UI_WNDSTYLE_CHILD, 0);

}

void CBrowserUI::DestoryBrowser()
{
	if (m_pBrowserWnd)
	{
		if (m_pBrowserWnd->GetHWND())
		{
			m_pBrowserWnd->SendMessage(WM_CLOSE);
		}
		else
		{
			delete m_pBrowserWnd;
			m_pBrowserWnd = nullptr;
		}
	}
}

void CBrowserUI::SetBrowserPos()
{
	if (m_pBrowserWnd && m_pBrowserWnd->GetHWND())
	{
		RECT rc = CalcBrowserPos();
		MoveWindow(m_pBrowserWnd->GetHWND(), rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, TRUE);
	}
}

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



RECT CBrowserUI::CalcBrowserPos()
{
	return m_rcItem;
}


CWebBrowserDlg::CWebBrowserDlg(CWebBrowserDlg** pSelfPtr)
{
	m_pPage = nullptr;
	m_pSelfPtr = pSelfPtr;
}



void CWebBrowserDlg::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	if (m_pSelfPtr)
	{
		*m_pSelfPtr = 0;
	}
	delete this;
}

void CWebBrowserDlg::InitWindow()
{
	m_pPage = static_cast<CWebBrowserPageUI*>(m_pm.FindControl(_T("webBrowserPageRoot")));
	if (m_pPage && !m_strUrl.IsEmpty())
	{
		m_pPage->Navigate(m_strUrl);
	}
}


LPCTSTR CWebBrowserDlg::GetWindowClassName() const
{
	return _T("WebBrowserDlg");
}

CDuiString CWebBrowserDlg::GetSkinFile()
{
	return _T("xml_webdlg");
}

void CWebBrowserDlg::Navigate(LPCTSTR lpszUrl)
{
	if (m_pPage)
	{
		m_pPage->Navigate(lpszUrl);
	}
	else
	{
		m_strUrl = lpszUrl;
	}
}