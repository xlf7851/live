// MainWnd.cpp

#include "StdAfx.h"
#include "MainWnd.h"
#include "../page/PageCreater.h"
#include "caption.h"
#include "toolbar.h"
#include "../base/httpHandler.h"
#include "../stock/hqDataQuery.h"
#include "../global/functioncall.h"

CMainWnd::CMainWnd()
{
	m_pRoot = nullptr;
	m_pCaption = nullptr;
	m_pToolbar = nullptr;
	m_pBody = nullptr;

	m_pPageCrater = nullptr;
}

CMainWnd::~CMainWnd()
{

}

CDuiString CMainWnd::GetSkinFile()
{
	return _T("XML_MAIN");
}

LPCTSTR CMainWnd::GetWindowClassName() const
{
	return _T("LiveMainWnd");
}

void CMainWnd::InitWindow()
{
	
	CControlUI* pControl = m_pm.GetRoot();
	if (pControl && _tcsicmp(pControl->GetClass(), _T("VerticalLayoutUI")) == 0)
	{
		m_pRoot = (CVerticalLayoutUI*)pControl;
	}

	if (nullptr == m_pRoot)
	{
		ASSERT(false);
	}

	InitCaption();
	InitToolbar();
	InitBody();
	StartPage();

	// TIMER
	::SetTimer(GetHWND(), TIMER_ID_WRITE_FILE_CACHE, 10000, nullptr);
}

void CMainWnd::Notify( TNotifyUI& msg )
{
	CDuiString name = msg.pSender->GetName();
	if (msg.sType == DUI_MSGTYPE_CLICK)
	{
		if (name == _T("closebtn"))
		{
			DoClose();
		}
	}
	WindowImplBase::Notify(msg);
}

void CMainWnd::OnFinalMessage( HWND hWnd )
{
	PostQuitMessage(0);
}

CControlUI* CMainWnd::CreateControl(LPCTSTR pstrClassName)
{
	return WindowImplBase::CreateControl(pstrClassName);
}

BOOL CMainWnd::IsInStaticControl(CControlUI *pControl)
{
	BOOL bRet = FALSE;
	if (!pControl)
	{
		return bRet;
	}

	CDuiString strClassName;
	std::vector<CDuiString> vctStaticName;

	strClassName = pControl->GetClass();
	strClassName.MakeLower();
	vctStaticName.push_back(_T("controlui"));
	vctStaticName.push_back(_T("textui"));
	vctStaticName.push_back(_T("labelui"));
	vctStaticName.push_back(_T("containerui"));
	vctStaticName.push_back(_T("horizontallayoutui"));
	vctStaticName.push_back(_T("verticallayoutui"));
	vctStaticName.push_back(_T("tablayoutui"));
	vctStaticName.push_back(_T("childlayoutui"));
	vctStaticName.push_back(_T("dialoglayoutui"));
	vctStaticName.push_back(_T("progresscontainerui"));
	vctStaticName.push_back(_T("captionpageui"));
	std::vector<CDuiString>::iterator it = std::find(vctStaticName.begin(), vctStaticName.end(), strClassName);
	if (vctStaticName.end() != it)
	{
		CControlUI* pParent = pControl->GetParent();
		while (pParent)
		{
			strClassName = pParent->GetClass();
			strClassName.MakeLower();
			it = std::find(vctStaticName.begin(), vctStaticName.end(), strClassName);
			if (vctStaticName.end() == it)
			{
				return bRet;
			}

			pParent = pParent->GetParent();
		}

		bRet = TRUE;
	}

	return bRet;
}

void CMainWnd::InitCaption()
{
	ASSERT(m_pRoot);
	CDialogBuilder builder;
	m_pCaption = builder.Create(_T("XML_CAPTION"), 0, this, &m_pm);
	if (m_pCaption)
	{
		m_pRoot->Add(m_pCaption);
	}
}

void CMainWnd::InitToolbar()
{
	ASSERT(m_pRoot);
	CDialogBuilder builder;
	m_pToolbar = builder.Create(_T("XML_TOOLBAR"), 0, this, &m_pm);
	if (m_pToolbar)
	{
		m_pRoot->Add(m_pToolbar);
	}
}

void CMainWnd::InitBody()
{
	ASSERT(m_pRoot);
	m_pBody = new CVerticalLayoutUI;
	m_pBody->SetDelayedDestroy(false);
	//m_pBody->SetBkColor(0xffffffff);
	m_pRoot->Add(m_pBody);
}

void CMainWnd::StartPage()
{
	CFunctionCallItem item;
	item.SetName(FUNCTION_CALL_CallName_SwitchPage);
	item.SetParam(FUNCTION_CALL_PARAM_xmlres, _T("xml_web_browser_page"));
	CFunctionCall::Call(item);
}

LRESULT CMainWnd::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	switch (uMsg)
	{
	case  UWM_SHOW_PAGE:
		bHandled = TRUE;
		OnShowCurrentPage(wParam, lParam);
		break;
	case UWM_SKIN_CHANGED:
		bHandled = TRUE;
		OnSkinChanged(wParam, lParam);
		break;
	case UWM_HTTP_REVEIVE:
		{
			http_utl::HttpHandlerPool* pHttpPool = http_utl::GetHttpHandlerPool();
			if (pHttpPool)
			{
				pHttpPool->OnHandlerMsg(wParam, lParam);
			}
		}
		break;
	case WM_COPYDATA:
		if (stock_wrapper::CHqDataQueryHandler::GetHqDataQueryHandler().OnCopyData(wParam, lParam))
		{

		}
		break;
	case WM_TIMER:
		{
			if (wParam == TIMER_ID_WRITE_FILE_CACHE)
			{
				bHandled = true;
				CGlobalData::OnTimer(wParam);
			}
		}
		break;
	default:
		break;
	}

	return 0;
}


void CMainWnd::OnShowCurrentPage(WPARAM wParam, LPARAM lParam)
{
	if (nullptr == m_pBody)
	{
		return;
	}

	if (0 == wParam)
	{
		return;
	}

	LPCTSTR lpszSkinFile = (LPCTSTR)wParam;
	if (NULL == lpszSkinFile || lpszSkinFile[0] == 0 || m_strCurrentPageXmlRes.CompareNoCase(lpszSkinFile) == 0)
	{
		return;
	}

	if (nullptr == m_pPageCrater)
	{
		m_pPageCrater = new CPageCreater(m_pm);
	}

	CControlUI* pControl = m_pPageCrater->CreatePage(lpszSkinFile);
	if (nullptr == pControl)
	{
		return;
	}

	m_strCurrentPageXmlRes = lpszSkinFile;
	
	m_pBody->RemoveAll();
	m_pBody->Add(pControl);

}

void CMainWnd::DoClose()
{

}

void CMainWnd::EnableTrans(bool bEnable)
{
	m_pm.SetLayered(bEnable);
	if (!bEnable)
	{
		SetTrans(0);
	}
}

void CMainWnd::SetTrans(int nTrans)
{
	m_pm.SetLayeredOpacity(255 - nTrans);
}

void CMainWnd::ChangeSkin(LPCTSTR lpszSkin)
{

}

void CMainWnd::OnSkinChanged(WPARAM wParam, LPARAM lParam)
{
	if (wParam == 0)
	{
		EnableTrans(lParam != 0);
	}
	else if (wParam == 1)
	{
		SetTrans(lParam);
	}
}