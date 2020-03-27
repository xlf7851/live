#include "pch.h"
#include "browserManager.h"
#include "include/cef_base.h"
#include "include/cef_app.h"
#include "browserApp.h"
#include "browserHandler.h"


CBrowerManager::CBrowerManager()
{
}


CBrowerManager::~CBrowerManager()
{
}

bool CBrowerManager::BrowserInitialize(unsigned int uOption, LPCTSTR lpResourceDirPath)
{
	CefMainArgs main_args;
	int exit_code = CefExecuteProcess(main_args, NULL, NULL);
	if (exit_code >= 0)
	{
		return false;
	}

	CefSettings settings;
	CefSettingsTraits::init(&settings);
	settings.no_sandbox = true;
	settings.multi_threaded_message_loop = true;

	if (lpResourceDirPath && lpResourceDirPath[0] != 0)
	{
		std::wstring strResPath = lpResourceDirPath;
		cef_string_from_wide(strResPath.c_str(), strResPath.size(), &settings.resources_dir_path);

		std::wstring strLocalesPath = strResPath + _T("locales\\");
		cef_string_from_wide(strLocalesPath.c_str(), strLocalesPath.size(), &settings.locales_dir_path);


		//std::wstring strCefLogPath = strResPath + _T("");
		//cef_string_from_wide(strCefLogPath.c_str(), strCefLogPath.size(), &settings.log_file);
	}
	
	//CefRefPtr<CBrowserApp> app(new CBrowserApp);
	m_app = new CBrowserApp;

	
	bool bRet = CefInitialize(main_args, settings, m_app.get(), NULL);
	if (!bRet)
	{
		delete m_app;
	}

	return bRet;
}

void CBrowerManager::BrowserMessageLoop()
{
	CefDoMessageLoopWork();
}

void CBrowerManager::BrowserUnInitialize()
{

}

CBrowserHandler* CBrowerManager::BrowserCreateBrowser(HWND hParent, const RECT& rc)
{
	CBrowserHandler* pHandler = new CBrowserHandler;
	if (!pHandler->CreateBrowser(hParent, rc))
	{
		delete pHandler;
		pHandler = NULL;
	}
	return pHandler;
}

void CBrowerManager::BrowserCloseBrowser(CBrowserHandler* pHandler)
{
	if (pHandler)
	{
		pHandler->CloseBrowser();
	
	}
}