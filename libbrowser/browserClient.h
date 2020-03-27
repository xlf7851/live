#pragma once

#include "include/cef_client.h"
#include "include/cef_base.h"
#include "include/base/cef_lock.h"

class IBrowserDelegate;
class CBrowserClientHandler : public CefClient, public CefLifeSpanHandler, public CefDisplayHandler, public CefLoadHandler
{
public:
	CBrowserClientHandler(IBrowserDelegate* delegate);
	virtual ~CBrowserClientHandler();

	bool CreateBrowser(CefWindowInfo const & info, CefBrowserSettings const & settings, CefString const & url);

	// CefClient methods:
	virtual CefRefPtr<CefDisplayHandler> GetDisplayHandler() override { return this; }
	virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override { return this; }
	virtual CefRefPtr<CefLoadHandler> GetLoadHandler() override { return this; }

	// CefDisplayHandler methods:
	virtual void OnAddressChange(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, const CefString& url) override;
	virtual void OnTitleChange(CefRefPtr<CefBrowser> browser, const CefString& title) override;
	virtual void OnFullscreenModeChange(CefRefPtr<CefBrowser> browser, bool fullscreen);

	// CefLifeSpanHandler methods:
	virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) override;
	virtual bool DoClose(CefRefPtr<CefBrowser> browser) override;
	virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) override;

	// CefLoadHandler methods:
	virtual void OnLoadingStateChange(CefRefPtr<CefBrowser> browser,
		bool isLoading,
		bool canGoBack,
		bool canGoForward) override;

	virtual void OnLoadError(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		ErrorCode errorCode,
		const CefString& errorText,
		const CefString& failedUrl) override;

	// This object may outlive the Delegate object so it's necessary for the
	// Delegate to detach itself before destruction.
	void DetachDelegate();

private:

	// Include the default reference counting implementation.
	IMPLEMENT_REFCOUNTING(CBrowserClientHandler);
	// Include the default locking implementation.
	IMPLEMENT_LOCKING(CBrowserClientHandler);

private:
	IBrowserDelegate* m_delegate;
};