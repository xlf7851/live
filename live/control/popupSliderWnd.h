#pragma once

class CPopupSliderWnd : public WindowImplBase
{
public:
	CPopupSliderWnd();
	~CPopupSliderWnd();

	CDuiString GetSkinFile();
	LPCTSTR GetWindowClassName() const;

	void PopupWnd(LPCTSTR lpszXml, int nValue, const POINT & point, HWND hParent, CPaintManagerUI* msgPM, DWORD dwUserData);
	void InitWindow();
	void Notify(TNotifyUI& msg);

	void OnFinalMessage(HWND hWnd);
	LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnMouseLeave(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);


protected:
	

protected:
	CDuiString		m_strXMLPath;
	CSliderUI		*m_pSlider;
	CPaintManagerUI* m_msgPM;
	DWORD			m_dwUserData;
	int				m_nValue;
};