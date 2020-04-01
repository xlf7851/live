// MainWnd.h

#pragma once


class CPageCreater;
class CMainWnd : public WindowImplBase
{
public:
	CMainWnd();
	~CMainWnd();

	CDuiString GetSkinFile();
	LPCTSTR GetWindowClassName() const ;
	
	void InitWindow();
	void Notify(TNotifyUI& msg);
	void OnFinalMessage(HWND hWnd);

	CControlUI* CreateControl(LPCTSTR pstrClassName);
	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual BOOL IsInStaticControl(CControlUI *pControl);

	void EnableTrans(bool bEnable);
	void SetTrans(int nTrans);
	void ChangeSkin(LPCTSTR lpszSkin);

protected:
	void InitCaption();
	void InitToolbar();
	void InitBody();
	void StartPage();

	void OnShowCurrentPage(WPARAM wParam, LPARAM lParam);
	void DoClose();
	void OnSkinChanged(WPARAM wParam, LPARAM lParam);

protected:
	CVerticalLayoutUI*	m_pRoot;
	CControlUI*			m_pCaption;
	CControlUI*			m_pToolbar;
	CVerticalLayoutUI*	m_pBody;

private:
	CPageCreater* m_pPageCrater;
	CDuiString m_strCurrentPageXmlRes;
};