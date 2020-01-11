// MainWnd.cpp

#include "StdAfx.h"
#include "popupSliderWnd.h"


CPopupSliderWnd::CPopupSliderWnd()
{
	m_pSlider = NULL;
	m_msgPM = NULL;
	m_dwUserData = 0;
	m_nValue = 0;
}

CPopupSliderWnd::~CPopupSliderWnd()
{

}

CDuiString CPopupSliderWnd::GetSkinFile()
{
	return m_strXMLPath;
}

LPCTSTR CPopupSliderWnd::GetWindowClassName() const
{
	return _T("PopupSliderMainWnd");
}

void CPopupSliderWnd::PopupWnd(LPCTSTR lpszXml, int nValue, const POINT & point, HWND hParent, CPaintManagerUI* msgPM, DWORD dwUserData)
{
	m_strXMLPath = lpszXml;
	m_msgPM = msgPM;
	m_dwUserData = dwUserData;
	m_nValue = nValue;

	Create(hParent, _T("PopupSliderWnd"), UI_WNDSTYLE_DIALOG, WS_EX_TOOLWINDOW);
	POINT pt = point;
	::ClientToScreen(hParent, &pt);
	//这里会计算应该显示的位置，好让自己出现在组合框上方
	::SetWindowPos(*this, NULL, pt.x, pt.y - m_pm.GetClientSize().cy, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);

}

void CPopupSliderWnd::InitWindow()
{
	m_pSlider = static_cast<CSliderUI*>(m_pm.FindControl(_T("sliderCtrl")));
	if (m_pSlider)
	{
		m_pSlider->SetValue(m_nValue);
	}

}

void CPopupSliderWnd::Notify(TNotifyUI& msg)
{
	CDuiString name = msg.pSender->GetName();
	if (msg.sType == DUI_MSGTYPE_VALUECHANGED || msg.sType == DUI_MSGTYPE_VALUECHANGED_MOVE)
	{
		if (msg.pSender == m_pSlider)
		{
			if (m_msgPM)
			{
				m_msgPM->SendNotify(m_pSlider, DUI_UMSGTYPE_POPUP_SLIDER_VALUECHANGED, m_dwUserData, m_pSlider->GetValue());
			}
			
		}
	}
	

	WindowImplBase::Notify(msg);
}

void CPopupSliderWnd::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	delete this;
}

LRESULT CPopupSliderWnd::OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (::IsIconic(*this)) bHandled = FALSE;
	return (wParam == 0) ? TRUE : FALSE;
}

LRESULT CPopupSliderWnd::OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	Close();
	return 0;
}

LRESULT CPopupSliderWnd::OnMouseLeave(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	Close();
	return 0;
}