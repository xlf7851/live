#include "pch.h"
#include "browserClient.h"
#include "browserDelegate.h"

#include "include/base/cef_bind.h"
#include "include/cef_app.h"
#include "include/wrapper/cef_closure_task.h"
#include "include/wrapper/cef_helpers.h"

CBrowserClientHandler::CBrowserClientHandler(IBrowserDelegate* delegate)
	: m_delegate(delegate)
{
}

CBrowserClientHandler::~CBrowserClientHandler()
{
}

bool CBrowserClientHandler::CreateBrowser(CefWindowInfo const & info, CefBrowserSettings const & settings, CefString const & url)
{
	return CefBrowserHost::CreateBrowser(info, this, url, settings, nullptr);
}

void CBrowserClientHandler::OnAddressChange(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, const CefString& url)
{
	CEF_REQUIRE_UI_THREAD();

	// Only update the address for the main (top-level) frame.
	if (frame->IsMain())
	{
		if (m_delegate != nullptr)
			m_delegate->OnSetAddress(url.c_str());
	}
}

void CBrowserClientHandler::OnTitleChange(CefRefPtr<CefBrowser> browser, const CefString& title)
{
	CEF_REQUIRE_UI_THREAD();

	if (m_delegate != nullptr)
		m_delegate->OnSetTitle(title.c_str());
}

void CBrowserClientHandler::OnFullscreenModeChange(CefRefPtr<CefBrowser> browser, bool fullscreen)
{
	CEF_REQUIRE_UI_THREAD();

	if (m_delegate != nullptr)
		m_delegate->OnSetFullscreen(fullscreen);
}

void CBrowserClientHandler::OnAfterCreated(CefRefPtr<CefBrowser> browser)
{
	CEF_REQUIRE_UI_THREAD();

	if (m_delegate != nullptr)
		m_delegate->OnBrowserCreated(browser);
}

bool CBrowserClientHandler::DoClose(CefRefPtr<CefBrowser> browser)
{
	CEF_REQUIRE_UI_THREAD();

	if (m_delegate != nullptr)
		m_delegate->OnBrowserClosing(browser);

	return false;
}

void CBrowserClientHandler::OnBeforeClose(CefRefPtr<CefBrowser> browser)
{
	CEF_REQUIRE_UI_THREAD();

	if (m_delegate != nullptr)
		m_delegate->OnBrowserClosed(browser);
}

void CBrowserClientHandler::OnLoadError(CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	ErrorCode errorCode,
	const CefString& errorText,
	const CefString& failedUrl)
{
	CEF_REQUIRE_UI_THREAD();

	// Don't display an error for downloaded files.
	if (errorCode == ERR_ABORTED)
		return;

	// Display a load error message.
	std::stringstream ss;
	ss << "<html><body bgcolor=\"white\">"
		"<h2>Failed to load URL " << std::string(failedUrl) <<
		" with error " << std::string(errorText) << " (" << errorCode <<
		").</h2></body></html>";
	frame->LoadString(ss.str(), failedUrl);
}

void CBrowserClientHandler::OnLoadingStateChange(CefRefPtr<CefBrowser> browser, bool isLoading, bool canGoBack, bool canGoForward)
{
	CEF_REQUIRE_UI_THREAD();

	if (m_delegate != nullptr)
		m_delegate->OnSetLoadingState(isLoading, canGoBack, canGoForward);
}

void CBrowserClientHandler::DetachDelegate()
{
	m_delegate = nullptr;
}
