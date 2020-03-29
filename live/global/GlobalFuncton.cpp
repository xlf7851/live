#include "stdafx.h"
#include "GlobalFuncton.h"
#include <TlHelp32.h>

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

	// 设置主窗口透明
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

	static TCHAR g_szProgramePath[MAX_PATH +1] = { 0 };
	static TCHAR g_szModuleFilePath[MAX_PATH + 1] = { 0 };

	LPCTSTR _GetModulePath(bool bFileName = false)
	{
		if (g_szModuleFilePath[0] == 0)
		{
			GetModuleFileName(NULL, g_szModuleFilePath, MAX_PATH);
			_tcscpy_s(g_szProgramePath, g_szModuleFilePath);
			(_tcsrchr(g_szProgramePath, _T('\\')))[1] = 0;
		}

		return bFileName ? g_szModuleFilePath : g_szProgramePath;
	}

	LPCTSTR GetProgramPath()
	{
		return _GetModulePath(false);
	}

	LPCTSTR GetModuleFilePath()
	{
		return _GetModulePath(true);
	}

	LPCTSTR GetRootDir()
	{
		return GetProgramPath();
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

	void RestartProgram()
	{
		HWND hWmd = GlobalGetMainWnd();
		if (hWmd == nullptr)
		{
			return;
		}
		::PostMessage(hWmd, WM_SYSCOMMAND, SC_CLOSE, NULL);
		//获取exe程序当前路径
	
		_tstring strFile = GetModuleFilePath();
		//重启程序
		STARTUPINFO StartInfo;
		PROCESS_INFORMATION procStruct;
		memset(&StartInfo, 0, sizeof(STARTUPINFO));
		StartInfo.cb = sizeof(STARTUPINFO);
		::CreateProcess(
			strFile.c_str(),
			NULL,
			NULL,
			NULL,
			FALSE,
			NORMAL_PRIORITY_CLASS,
			NULL,
			NULL,
			&StartInfo,
			&procStruct);
	}
}
