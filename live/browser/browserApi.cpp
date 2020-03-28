#include "stdafx.h"
#include "browserApi.h"
#include "../global/GlobalFuncton.h"


CBrowserApi::CBrowserApi()
{
}

CBrowserApi::~CBrowserApi()
{
}

CBrowserApi* CBrowserApi::Instance()
{
	static CBrowserApi s_obj;
	return &s_obj;
}


static std::wstring _GetCefResPath()
{
  	_tstring strPath = global_funciton::GetProgramPath();
 	strPath += _T("cefres\\");

	return xlf::AutoToWString(strPath.c_str());
}

bool CBrowserApi::Initialize()
{
	std::wstring strCefRespath = _GetCefResPath();
	if (BrowserInitialize(0, (LPCTSTR)strCefRespath.c_str()))
	{
		return true;
	}

	return false;
}

void CBrowserApi::UnInitialize()
{
	BrowserUnInitialize();
}

BROWSER_HANDLE CBrowserApi::CreateBrowser(HWND hParent, int left, int top, int width, int height)
{
	return BrowserCreateBrowser(hParent, left, top, width, height);
}

void CBrowserApi::CloseBrowser(BROWSER_HANDLE hBrowser)
{
	BrowserCloseBrowser(hBrowser);
}

void CBrowserApi::Navigate(BROWSER_HANDLE hBrowser, LPCTSTR lpszUrl)
{	std::wstring strUrl = xlf::AutoToWString(lpszUrl);	BrowserNavigate(hBrowser, (LPCTSTR)strUrl.c_str());
}

void CBrowserApi::MoveWindow(BROWSER_HANDLE hBrowser, int left, int top, int width, int height)
{
	BrowserMoveWindow(hBrowser, left, top, width, height);
}