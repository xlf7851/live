// MainWnd.cpp

#include "StdAfx.h"
#include "skinSetWnd.h"
#include "../control/popupSliderWnd.h"
#include "../global/GlobalFuncton.h"


CSkinSetWnd::CSkinSetWnd()
{
	m_pSkinTab = NULL;
	m_pSkinTransBtn = NULL;
	m_nTrans = 0;
}

CSkinSetWnd::~CSkinSetWnd()
{

}

CDuiString CSkinSetWnd::GetSkinFile()
{
	return _T("XML_SKIN_SET");
}

LPCTSTR CSkinSetWnd::GetWindowClassName() const
{
	return _T("SkinMainWnd");
}

void CSkinSetWnd::InitWindow()
{
	m_pSkinTab = static_cast<CAnimationTabLayoutUI*>(m_pm.FindControl(_T("skinTab")));
	m_pSkinTransBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("BtnSkinTrans")));

}

void CSkinSetWnd::Notify( TNotifyUI& msg )
{
	CDuiString name = msg.pSender->GetName();
	if( msg.sType == DUI_MSGTYPE_CLICK)
	{
		if (name == _T("BtnSkinTrans"))
		{
			OnBtnSkinTrans();
		}
		else if (name == _T("BtnPattet"))
		{
			OnBtnPattet();
		}
		else if (name == _T("BtnCustom"))
		{
			OnBtnCustom();
		}
		else if (name == _T("BtnDefault"))
		{
			OnBtnDefault();
		}
	}
	else if (msg.sType == DUI_MSGTYPE_SELECTCHANGED)
	{
		if (name == _T("OptOfficial"))
		{
			if (m_pSkinTab)
			{
				m_pSkinTab->SelectItem(0);
			}
		}
		else if (name == _T("OptMy"))
		{
			if (m_pSkinTab)
			{
				m_pSkinTab->SelectItem(1);
			}
		}
		else if (name == _T("OptSkinTrans"))
		{
			COptionUI* pOpt = static_cast<COptionUI*>(msg.pSender);
			if (pOpt)
			{
				OnEnableSkinTrans(pOpt->IsSelected());
			}
		}
	}
	else if (msg.sType == DUI_UMSGTYPE_POPUP_SLIDER_VALUECHANGED)
	{
		if (m_pSkinTransBtn && (DWORD)m_pSkinTransBtn == msg.wParam)
		{
			m_nTrans = msg.lParam;
			UpdateTransText();
			global_funciton::SetMainWndLayered(m_nTrans);
		}
	}

	WindowImplBase::Notify(msg);
}

void CSkinSetWnd::OnFinalMessage( HWND hWnd )
{
	WindowImplBase::OnFinalMessage(hWnd);
}

CControlUI* CSkinSetWnd::CreateControl(LPCTSTR pstrClassName)
{
	return WindowImplBase::CreateControl(pstrClassName);
}

LRESULT CSkinSetWnd::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{

	return 0;
}

void CSkinSetWnd::OnEnableSkinTrans(bool bEnable)
{
	if (bEnable)
	{
		if (m_pSkinTransBtn)
		{
			m_pSkinTransBtn->SetEnabled(true);
		}

		global_funciton::EnableMainWndLayered(bEnable);
	}
	else
	{
		m_nTrans = 0;
		if (m_pSkinTransBtn)
		{
			m_pSkinTransBtn->SetEnabled(false);
			UpdateTransText();
		}

		global_funciton::EnableMainWndLayered(false);
	}
}

void CSkinSetWnd::OnBtnSkinTrans()
{
	if (NULL == m_pSkinTransBtn)
	{
		return;
	}

	POINT point;
	RECT rc = m_pSkinTransBtn->GetPos();
	point.x = rc.left;
	point.y = rc.top;

	CPopupSliderWnd* pWnd = new CPopupSliderWnd;
	pWnd->PopupWnd(_T("XML_SKIN_TRANS_SET_POP"), m_nTrans,  point, GetHWND(), &m_pm, (DWORD)m_pSkinTransBtn);
}

void CSkinSetWnd::OnBtnPattet()
{

}

void CSkinSetWnd::OnBtnCustom()
{

}

void CSkinSetWnd::OnBtnDefault()
{

}

void CSkinSetWnd::UpdateTransText()
{
	if (m_pSkinTransBtn)
	{
		CDuiString strText;
		strText.SmallFormat(_T("%d%%"), m_nTrans * 100 / 255);
		m_pSkinTransBtn->SetText(strText);
	}
}