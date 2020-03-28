#pragma once


class CBrowserUI;
class CBrowserWnd : public CWindowWnd
{
public:
	CBrowserWnd();

	void Navigate(LPCTSTR lpszUrl);

	void Init(CBrowserUI* pOwner);
	RECT CalPos();

	LPCTSTR GetWindowClassName() const;
	void OnFinalMessage(HWND hWnd);

	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
protected:
	void CreateBrowser();
	void SetBrowserPos();
	void ShowBrowser(bool bShow);

protected:
	CBrowserUI* m_pOwner;
	BROWSER_HANDLE m_hBrowser;
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

	void InitWindow();
	void OnClick(TNotifyUI& msg);

	void Navigate(LPCTSTR lpszUrl);

	

protected:
	LPCTSTR GetWindowClassName() const override;
	CDuiString GetSkinFile() override;

private:
	CBrowserUI* m_pBrowser;
	CEditUI* m_pUrlEditBox;
	CDuiString m_strUrl;
};