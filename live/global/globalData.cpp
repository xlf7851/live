#include "stdafx.h"
#include "globalData.h"
#include <TlHelp32.h>
#include "functioncallBase.h"
#include "../stock/block.h"



void CInitGlobalData::InitGlobalInstance()
{
	// 初始化功能调用管理
	CFunctinCallerManager::Instance();

	// 初始化板块缓存对象
	stock_wrapper::BlockCacheManager::Instance();

	// 初始化板块组对象
	stock_wrapper::BlockGroupManager::Instance();



}

void CInitGlobalData::DoWork()
{
	// 初始化板块缓存
	stock_wrapper::BlockCacheManager::Instance()->Init();

	// 初始化板块组
	stock_wrapper::BlockGroupManager::Instance()->Init();
}

CGlobalData::CGlobalData()
{
		
}

CGlobalData::~CGlobalData()
{
		
}

CGlobalData* CGlobalData::Instance()
{
	static CGlobalData* g_instance = nullptr;
	if (g_instance == nullptr)
	{
		g_instance = new CGlobalData;
	}

	return g_instance;
}

void CGlobalData::OnTimer(UINT uEventID)
{
	if (uEventID == TIMER_ID_WRITE_FILE_CACHE)
	{
		stock_wrapper::BlockCacheManager::Instance()->Save();
		stock_wrapper::BlockGroupManager::Instance()->Save();
	}
	
}


void CGlobalData::Init()
{
		
}

void CGlobalData::DoGlobalFunction(LPCTSTR lpszFunc)
{
		
}

struct _FIND_PROCESS_RESULT
{
	DWORD m_dwProcessID;
	_tstring m_strModulePath;
};

static bool _FindProcessByName(LPCTSTR lpszName, std::vector<_FIND_PROCESS_RESULT> & result)
{
	DWORD qqPid = 0;    // 进程id
	PROCESSENTRY32 pe;  // 进程信息
	pe.dwSize = sizeof(PROCESSENTRY32);
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); // 进程快照
	if (!Process32First(hSnapshot, &pe)) // 得到第一个进程的快照
		return false;
	_tstring strPath;
	do
	{
		if (!Process32Next(hSnapshot, &pe))
			break;
		strPath = pe.szExeFile;
		if (strPath.find(lpszName) != -1)
		{
			_FIND_PROCESS_RESULT item;
			item.m_dwProcessID = pe.th32ProcessID;

			HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, item.m_dwProcessID);
			if (hProcess)
			{
				TCHAR szFilePath[MAX_PATH] = { 0 };
				DWORD dwSize = MAX_PATH;
				QueryFullProcessImageName(hProcess, 0, szFilePath, &dwSize);
				if (dwSize > 0 && dwSize < MAX_PATH - 1)
				{
					szFilePath[dwSize] = 0;
					item.m_strModulePath = szFilePath;
					result.push_back(item);
				}
			}


		}
	} while (true);

	return result.size() > 0;
}

struct _FIND_WINDOW_RESULT
{
	HWND m_hWnd;
	_tstring m_strModulePath;
	_FIND_WINDOW_RESULT()
	{
		m_hWnd = nullptr;
	}
};

struct _ENUM_WINDOW_PARAM
{
	DWORD m_dwProcessID;
	_tstring m_strModulePath;
	std::vector<_FIND_WINDOW_RESULT>& m_result;
	_ENUM_WINDOW_PARAM(std::vector<_FIND_WINDOW_RESULT>& result) : m_result(result)
	{
		m_dwProcessID = 0;
	}
};



static BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
	_ENUM_WINDOW_PARAM *pParam = (_ENUM_WINDOW_PARAM*)lParam;
	DWORD dwProp = (DWORD)::GetProp(hwnd, pParam->m_strModulePath.c_str());
	if (dwProp == 2)
	{
		DWORD dwProcessID = 0;
		GetWindowThreadProcessId(hwnd, &dwProcessID);
		if (dwProcessID == pParam->m_dwProcessID)
		{
			_FIND_WINDOW_RESULT item;
			item.m_hWnd = hwnd;
			item.m_strModulePath = pParam->m_strModulePath;
			pParam->m_result.push_back(item);
		}
	}



	return TRUE;
}

static bool _FindWindowByNameAndProp(LPCTSTR lpszName, DWORD dwProp, std::vector<_FIND_WINDOW_RESULT>& vcHwnd)
{
	std::vector<_FIND_PROCESS_RESULT>  result;
	_FindProcessByName(lpszName, result);
	if (result.size() == 0)
	{
		return false;
	}

	for (int i = 0; i < result.size(); i++)
	{
		_FIND_PROCESS_RESULT& item = result[i];
		_ENUM_WINDOW_PARAM param(vcHwnd);
		param.m_dwProcessID = item.m_dwProcessID;
		param.m_strModulePath = item.m_strModulePath;
		EnumWindows(EnumWindowsProc, (LPARAM)&param);
	}

	return vcHwnd.size() > 0;
}
