#include "stdafx.h"
#include "browserModule.h"


CBrowserModule::CBrowserModule()
{
}

CBrowserModule::~CBrowserModule()
{
}

CBrowserModule* CBrowserModule::Instance()
{
	static CBrowserModule s_obj;
	return &s_obj;
}


static std::wstring _GetCefResPath()
{
  	_tstring strPath = global_funciton::GetProgramPath();
 	strPath += _T("cefres\\");

	return xlf::AutoToWString(strPath.c_str());
}

bool CBrowserModule::Initialize()
{
	std::wstring strCefRespath = _GetCefResPath();
	if (BrowserInitialize(0, strCefRespath.c_str()))
	{
		return true;
	}

	return false;
}

void CBrowserModule::UnInitialize()
{
	BrowserUnInitialize();
}

BROWSER_HANDLE CBrowserModule::CreateBrowser(HWND hParent, int left, int top, int width, int height)
{
	return BrowserCreateBrowser(hParent, left, top, width, height);
}

void CBrowserModule::CloseBrowser(BROWSER_HANDLE hBrowser)
{
	BrowserCloseBrowser(hBrowser);
}

void CBrowserModule::Navigate(BROWSER_HANDLE hBrowser, LPCTSTR lpszUrl)
{
}

void CBrowserModule::MoveWindow(BROWSER_HANDLE hBrowser, int left, int top, int width, int height)
{
	BrowserMoveWindow(hBrowser, left, top, width, height);
}