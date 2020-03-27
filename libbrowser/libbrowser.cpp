// libbrowser.cpp : 定义 DLL 应用程序的导出函数。
//

#include "libbrowser.h"
#include "browserManager.h"
#include "browserHandler.h"

inline CBrowerManager* GetBrowserManager()
{
	static CBrowerManager g_sBrowserManager;

	return &g_sBrowserManager;
}

bool BrowserInitialize(unsigned int uOption, LPCTSTR lpResourceDirPath)
{
	return GetBrowserManager()->BrowserInitialize(uOption, lpResourceDirPath);
}

void BrowserMessageLoop()
{
	GetBrowserManager()->BrowserMessageLoop();
}

void BrowserUnInitialize()
{
	GetBrowserManager()->BrowserUnInitialize();
}

BROWSER_HANDLE BrowserCreateBrowser(HWND hParent, const RECT& rc)
{
	return GetBrowserManager()->BrowserCreateBrowser(hParent, rc);
}

void BrowserCloseBrowser(BROWSER_HANDLE hBrowser)
{
	GetBrowserManager()->BrowserCloseBrowser((CBrowserHandler*)hBrowser);
}

void BrowserNavigate(BROWSER_HANDLE hBrowser, LPCTSTR lpszUrl)
{
	CBrowserHandler* pHandler = (CBrowserHandler*)hBrowser;
	if (pHandler)
	{
		pHandler->Navigate(lpszUrl);
	}

}

void BrowserMoveWindow(BROWSER_HANDLE hBrowser, const RECT& rc)
{
	CBrowserHandler* pHandler = (CBrowserHandler*)hBrowser;
	if (pHandler)
	{
		HWND hWnd = pHandler->GetBrowserHwnd();
		if (hWnd)
		{
			::MoveWindow(hWnd, rc.left, rc.top, rc.right, rc.bottom, FALSE);
		}
	}
}