#include "stdafx.h"
#include "openFileDlg.h"



bool COpenFileHelp::OpenFile(bool bOpenFileDialog, LPCTSTR lpszDefExt, LPCTSTR lpszFileName, DWORD dwFlags , LPCTSTR lpszFilter, HWND hParent)
{
	Clear();
	OPENFILENAME ofn;
	TCHAR szFile[MAX_PATH] = { 0 };

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hParent;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = lpszFilter;
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = dwFlags;

	if (!GetOpenFileName(&ofn))
	{
		return false;
	}	m_strFilePath = szFile;
	return true;
}

// 此回调函数为全局函数或静态函数;
static int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	switch (uMsg)
	{
	case BFFM_INITIALIZED:
	{
		::SendMessage(hwnd, BFFM_SETSELECTION, TRUE, (LPARAM)lpData);
	}
	break;
	default:
		break;
	}
	return 0;
}

bool COpenFileHelp::BrowseForFolder(DWORD dwFlags, HWND hParent)
{
	Clear();
// 	TCHAR szPathName[MAX_PATH] = { 0 };
// 
// 	BROWSEINFO bInfo = { 0 };
// 	static CDuiString strDefaultPath = _T("C:\\");
// 	bInfo.hwndOwner = hParent; 
// 	bInfo.ulFlags = dwFlags;
// 	bInfo.lpfn = BrowseCallbackProc;
// 	bInfo.lParam = (LPARAM)(LPCTSTR)(strDefaultPath);
// 
// 	LPITEMIDLIST lpDlist = NULL;
// 	lpDlist = SHBrowseForFolder(&bInfo);
// 	if (lpDlist)
// 	{
// 		SHGetPathFromIDList(lpDlist, szPathName);
// 
// 		strDefaultPath = szPathName;
// 		m_strFilePath = szPathName;
// 
// 		return true;
// 	}

	return false;
}

void COpenFileHelp::Clear()
{
	m_strFilePath.Empty();
}

CDuiString COpenFileHelp::GetFilePath()
{
	return m_strFilePath;
}