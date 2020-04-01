#pragma once


class CBrowserUI;
class CBrowserWnd : public CWindowWnd
{
public:
	CBrowserWnd(CBrowserWnd** pSelfPtr = nullptr);
	virtual ~CBrowserWnd();

	void Navigate(LPCTSTR lpszUrl);

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
	CBrowserWnd** m_pSelfPtr;
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
	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

	RECT CalcBrowserPos();

protected:
	CBrowserWnd* GetBrowserWnd();
	void DestoryBrowser();
	void CreateBrowser();
	void SetBrowserPos();
	void ShowBrowser(bool bShow);

private:
	CBrowserWnd* m_pBrowserWnd;
	CDuiString m_strUrl;
};

class CWebBrowserPageUI : public CBasePageUI, public IEditPreMessageHandler, public IMessageFilterUI
{
	DECLARE_DUICONTROL(CWebBrowserPageUI)
public:
	CWebBrowserPageUI();
	~CWebBrowserPageUI();

	virtual LPCTSTR GetClass() const;
	virtual LPVOID GetInterface(LPCTSTR pstrName);
	virtual LRESULT EditMessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	virtual LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM /*lParam*/, bool& /*bHandled*/);

	virtual void OnNotify(TNotifyUI& msg);
	virtual void InitPage();

	void Navigate(LPCTSTR lpszUrl);
	void SetCanShowSearchBox(bool bEnable);
	void SetTabShowSearchBox(bool bEnable);

	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

protected:
	void OnKeyReturn();
	void AutoShowHideSearchModule();

protected:
	CBrowserUI* m_pBrowser;
	CEditUI* m_pUrlEditBox;
	CControlUI* m_pSerchBoxModule;
	CDuiString m_strUrl;
	bool m_bCanShowSearchBox;
	bool m_bTabShowSearchBox;
};

// 必须使用new 创建对象
class CWebBrowserDlg : public WindowImplBase
{
public:
	CWebBrowserDlg(CWebBrowserDlg** pSelfPtr= NULL);

	void OnFinalMessage(HWND hWnd);
	void InitWindow();

	void Navigate(LPCTSTR lpszUrl);


protected:
	LPCTSTR GetWindowClassName() const override;
	CDuiString GetSkinFile() override;

private:
	CWebBrowserDlg** m_pSelfPtr;
	CWebBrowserPageUI* m_pPage;
	CDuiString m_strUrl;
};