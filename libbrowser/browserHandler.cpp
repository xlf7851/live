#include "pch.h"
#include "browserHandler.h"
#include "browserClient.h"
#include "browserApp.h"


CBrowserHandler::CBrowserHandler()
{
	memset(&m_rcMove, 0, sizeof(RECT));
}


CBrowserHandler::~CBrowserHandler()
{

}


CefRefPtr<CBrowserClientHandler> CBrowserHandler::GetClient()
{
	return m_client;
}

bool CBrowserHandler::CreateBrowser(HWND hParent, const RECT& rc)
{
	//m_app = new CBrowserApp;
	m_client = new CBrowserClientHandler(this);

	CefWindowInfo window_info;
	window_info.SetAsChild(hParent,rc);
	CefBrowserSettings browser_settings;

	CefString url;// = _T("http://www.baidu.com");
	
	return m_client->CreateBrowser(window_info, browser_settings, url);
}

void CBrowserHandler::CloseBrowser()
{
	if (m_browser)
	{
		m_browser->GetHost()->CloseBrowser(false);
	}
}

HWND CBrowserHandler::GetBrowserHwnd()
{
	if (m_browser)
	{
		return m_browser->GetHost()->GetWindowHandle();
	}

	return NULL;
}

void CBrowserHandler::Navigate(LPCTSTR lpszUrl)
{
	if (m_browser == nullptr)
	{
		m_strUrl = lpszUrl;
		return;
	}

	CefString url(lpszUrl);

	m_browser->GetMainFrame()->LoadURL(url);
}

void CBrowserHandler::OnBrowserCreated(CefRefPtr<CefBrowser> browser)
{
	m_browser = browser;
	if (!m_strUrl.empty())
	{
		m_browser->GetMainFrame()->LoadURL(m_strUrl);
	}

	if (m_rcMove.right > m_rcMove.left && m_rcMove.bottom > m_rcMove.top)
	{
		MoveBrowserWindow(m_rcMove.left, m_rcMove.top, m_rcMove.right - m_rcMove.left, m_rcMove.bottom - m_rcMove.top);
	}
}

void CBrowserHandler::OnBrowserClosing(CefRefPtr<CefBrowser> browser)
{
}

void CBrowserHandler::OnBrowserClosed(CefRefPtr<CefBrowser> browser)
{
	if (m_browser != nullptr &&
		m_browser->GetIdentifier() == browser->GetIdentifier())
	{
		m_browser = nullptr;
		if (m_client != nullptr)
		{
			m_client->DetachDelegate();
		}
	}
}

void CBrowserHandler::OnSetAddress(LPCTSTR url)
{
	
}

void CBrowserHandler::OnSetTitle(LPCTSTR title)
{
	
}

void CBrowserHandler::OnSetFullscreen(bool const fullscreen)
{
}

void CBrowserHandler::OnSetLoadingState(bool const isLoading,bool const canGoBack,bool const canGoForward)
{
}

void CBrowserHandler::MoveBrowserWindow(int left, int top, int width, int height)
{
	HWND hWnd = GetBrowserHwnd();
	if (hWnd)
	{
		::MoveWindow(hWnd, left, top, width, height, FALSE);
	}
	else
	{
		m_rcMove.left = left;
		m_rcMove.top = top;
		m_rcMove.right = m_rcMove.left + width;
		m_rcMove.bottom = m_rcMove.top + height;
	}
}