#pragma once


class CBrowserUI;
class CBrowserWnd : public CWindowWnd
{
public:
	CBrowserWnd();
	virtual ~CBrowserWnd();

	void Navigate(LPCTSTR lpszUrl);
	void EnableDeleteSelf(bool bEnable);
	bool IsDeleteSelf();

	LPCTSTR GetWindowClassName() const;
	void OnFinalMessage(HWND hWnd);

	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
protected:
	void CreateBrowser();
	void SetBrowserPos();
	void ShowBrowser(bool bShow);

protected:
	BROWSER_HANDLE m_hBrowser;
	CDuiString m_strUrl;
	bool m_bDeleteSelf;
};

class CBrowserUI : public CControlUI
{
	DECLARE_DUICONTROL(CBrowserUI)
public:
	CBrowserUI();
	~CBrowserUI();

	virtual void DoInit();

	virtual LPCTSTR GetClass() const;
	virtual LPVOID GetInterface(LPCTSTR pstrName);

	void InitBrowser(LPCTSTR lpszUrl);


	void Navigate(LPCTSTR lpszUrl);

	void SetPos(RECT rc, bool bNeedInvalidate = true);
	void Move(SIZE szOffset, bool bNeedInvalidate = true);
	void SetVisible(bool bVisible = true);

	RECT CalcBrowserPos();

protected:
	CBrowserWnd* GetBrowserWnd();
	void DestoryBrowser();
	void CreateBrowser();
	void SetBrowserPos();
	void ShowBrowser(bool bShow);

private:
	CBrowserWnd* m_pBrowserWnd;
};

class CWebBrowserDlg : public WindowImplBase
{
public:
	CWebBrowserDlg();

	void OnFinalMessage(HWND hWnd);
	void InitWindow();
	void OnClick(TNotifyUI& msg);

	void Navigate(LPCTSTR lpszUrl);

	void SetDeleteSelf(bool bDelete);

protected:
	LPCTSTR GetWindowClassName() const override;
	CDuiString GetSkinFile() override;

private:
	CBrowserUI* m_pBrowser;
	CEditUI* m_pUrlEditBox;
	CDuiString m_strUrl;
	bool m_bDeleteSelf;
};