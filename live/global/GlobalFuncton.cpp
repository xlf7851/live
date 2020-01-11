#include "stdafx.h"
#include "GlobalFuncton.h"

extern HWND GlobalGetMainWnd();
namespace global_funciton {

	HWND GetMianHwnd()
	{
		return GlobalGetMainWnd();
	}


	void ShowPage(LPCTSTR lpszSkinFile, HWND hNotify)
	{
		HWND hWnd = hNotify ? hNotify : GetMianHwnd();
		if (hWnd)
		{
			::SendMessage(hWnd, UWM_SHOW_PAGE, (WPARAM)lpszSkinFile, 0);
		}
	}

	// ����������͸��
	void EnableMainWndLayered(bool bEnable)
	{
		HWND hWnd = GetMianHwnd();
		if (hWnd)
		{
			::SendMessage(hWnd, UWM_SKIN_CHANGED, 0, bEnable ? 1 : 0);
		}
	}
	void SetMainWndLayered(int nLayered)
	{
		HWND hWnd = GetMianHwnd();
		if (hWnd)
		{
			::SendMessage(hWnd, UWM_SKIN_CHANGED, 1, nLayered);
		}
	}

	static TCHAR g_szRoot[MAX_PATH + 1] = { 0 };
	LPCTSTR GetRootDir()
	{
		if (g_szRoot[0] == 0)
		{
			GetModuleFileName(NULL, g_szRoot, MAX_PATH);
			(_tcsrchr(g_szRoot, _T('\\')))[1] = 0;
		}

		return g_szRoot;
	}

	static TCHAR g_szConfig[MAX_PATH + 1] = { 0 };
	LPCTSTR GetConfigDir()
	{
		if (g_szConfig[0] == 0)
		{
			LPCTSTR root = GetRootDir();
			if (root[0] != 0)
			{
				_tcscpy_s(g_szConfig, root);
				_tcscat_s(g_szConfig, _T("config\\"));
			}
		}

		return g_szConfig;
	}
}
