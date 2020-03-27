#pragma once

#include "include/cef_app.h"
#include "include/cef_base.h"


class CBrowserApp : public CefApp, public CefBrowserProcessHandler
{
public:
	virtual ~CBrowserApp() {}

	// CefApp methods:
	virtual CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() override { return this; }

	// CefBrowserProcessHandler methods:
	virtual void OnContextInitialized() override;

private:
	// Include the default reference counting implementation.
	IMPLEMENT_REFCOUNTING(CBrowserApp);
};
