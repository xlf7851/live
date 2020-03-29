#pragma once

class CBrowserUI;
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