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

RECT CBrowserUI::CalcBrowserPos()
{
	return m_rcItem;
}


CWebBrowserDlg::CWebBrowserDlg()
{
	m_pBrowser = NULL;
	m_pUrlEditBox = NULL;
	m_bDeleteSelf = false;
}

void CWebBrowserDlg::SetDeleteSelf(bool bDelete)
{
	m_bDeleteSelf = true;
}

void CWebBrowserDlg::OnFinalMessage(HWND hWnd)
{
	if (m_bDeleteSelf)
	{
		delete this;
	}
}

void CWebBrowserDlg::InitWindow()
{
	m_pBrowser = static_cast<CBrowserUI*>(m_pm.FindControl(_T("webBrowser")));
	m_pUrlEditBox = static_cast<CEditUI*>(m_pm.FindControl(_T("urlBox")));
	if (m_pBrowser && !m_strUrl.IsEmpty())
	{
		m_pBrowser->Navigate(m_strUrl);
	}
}

void CWebBrowserDlg::OnClick(TNotifyUI& msg)
{
	CDuiString strName = msg.pSender->GetName();
	if (strName.CompareNoCase(_T("btnSearch")) == 0)
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
		return;
	}
	WindowImplBase::OnClick(msg);
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
	if (m_pBrowser)
	{
		m_pBrowser->Navigate(lpszUrl);
	}
	else
	{
		m_strUrl = lpszUrl;
	}
}