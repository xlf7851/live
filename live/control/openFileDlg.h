#pragma once
#include <commdlg.h>

class COpenFileHelp
{
public:
	bool OpenFile(bool bOpenFileDialog = true, LPCTSTR lpszDefExt = NULL, LPCTSTR lpszFileName = NULL, DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, LPCTSTR lpszFilter = NULL, HWND hParent = NULL);
	bool BrowseForFolder(DWORD dwFlags = BIF_DONTGOBELOWDOMAIN | BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE | BIF_EDITBOX, HWND hParent = NULL);
	CDuiString GetFilePath();
protected:
	void Clear();
protected:
	CDuiString m_strFilePath;
};

