#pragma once

#include "include/cef_base.h"
class CefBrowser;

class IBrowserDelegate
{
public:
	virtual void OnBrowserCreated(CefRefPtr<CefBrowser> browser) = 0;

	virtual void OnBrowserClosing(CefRefPtr<CefBrowser> browser) = 0;

	virtual void OnBrowserClosed(CefRefPtr<CefBrowser> browser) = 0;

	virtual void OnSetAddress(LPCTSTR url) = 0;

	virtual void OnSetTitle(LPCTSTR title) = 0;

	virtual void OnSetFullscreen(bool const fullscreen) = 0;

	virtual void OnSetLoadingState(bool const isLoading, bool const canGoBack, bool const canGoForward) = 0;

protected:
	virtual ~IBrowserDelegate() {}
};
