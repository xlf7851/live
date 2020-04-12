#pragma once

class IModifyTextDlgCheckTextHandler
{
public:
	virtual int CheckModifyNewText(LPCTSTR lpszNewText, CDuiString& strError) = 0;
};

class CModifyTextDlg : public WindowImplBase
{
public:
	CModifyTextDlg();

	
	void InitInfo(LPCTSTR lpOldText, LPCTSTR lpszTitle = nullptr, IModifyTextDlgCheckTextHandler* pErrorChecker = nullptr);

	static int ShowDlg(CDuiString& strNewText, LPCTSTR lpOldText, HWND hParent = nullptr, LPCTSTR lpszTitle = nullptr, int xPos = -1, int yPox = -1, IModifyTextDlgCheckTextHandler* pErrorChecker = nullptr);

	CDuiString GetSkinFile();
	LPCTSTR GetWindowClassName() const;

	void InitWindow();
	void OnClick(TNotifyUI& msg);

	CDuiString GetNewText();

protected:
	int CheckError();
protected:
	CDuiString m_strOldText;
	CDuiString m_strNewText;
	
	CDuiString m_strTitle;

	CDuiString m_strErrorMsg;
	IModifyTextDlgCheckTextHandler* m_pErrorChecker;

	CEditUI* m_pEdit;
};