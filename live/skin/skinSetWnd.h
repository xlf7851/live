#pragma once


class CSkinSetWnd : public WindowImplBase
{
public:
	CSkinSetWnd();
	~CSkinSetWnd();

	CDuiString GetSkinFile();
	LPCTSTR GetWindowClassName() const ;
	
	void InitWindow();
	void Notify(TNotifyUI& msg);
	void OnFinalMessage(HWND hWnd);

	CControlUI* CreateControl(LPCTSTR pstrClassName);
	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

protected:
	void OnEnableSkinTrans(bool bEnable);
	void OnBtnPattet();
	void OnBtnSkinTrans();
	void OnBtnCustom();
	void OnBtnDefault();

	void UpdateTransText();

protected:
	CAnimationTabLayoutUI *m_pSkinTab;
	CButtonUI* m_pSkinTransBtn;

	int m_nTrans;
};