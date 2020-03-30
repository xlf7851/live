#pragma once
#include "browserDelegate.h"

class CBrowserApp;
class CBrowserClientHandler;

class CBrowserHandler : public IBrowserDelegate
{
public:
	CBrowserHandler();
	~CBrowserHandler();

	CefRefPtr<CBrowserClientHandler> GetClient();

	bool CreateBrowser(HWND hParent, const RECT& rc);
	void CloseBrowser();
	HWND GetBrowserHwnd();
	void MoveBrowserWindow(int left, int top, int width, int height);
	void Navigate(LPCTSTR lpszUrl);

public:
	virtual void OnBrowserCreated(CefRefPtr<CefBrowser> browser);

	virtual void OnBrowserClosing(CefRefPtr<CefBrowser> browser);

	virtual void OnBrowserClosed(CefRefPtr<CefBrowser> browser);

	virtual void OnSetAddress(LPCTSTR url);

	virtual void OnSetTitle(LPCTSTR title);

	virtual void OnSetFullscreen(bool const fullscreen);

	virtual void OnSetLoadingState(bool const isLoading, bool const canGoBack, bool const canGoForward);

protected:
	CefRefPtr<CBrowserClientHandler>	m_client;
	CefRefPtr<CefBrowser>				m_browser;

	CefString m_strUrl;
	RECT m_rcMove;
};

