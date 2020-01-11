#include "stdafx.h"
#include "login.h"

CLogin::CLogin(  )
{
	m_pUserName = NULL;
	m_pPassword = NULL;
}

CLogin::~CLogin()
{}

LPCTSTR CLogin::GetWindowClassName() const
{
	return _T("DesktopMainWnd");
}


CDuiString CLogin::GetSkinFile()
{
	return _T("XML_LOGIN");
}

void CLogin::InitWindow()
{
	m_pUserName = m_pm.FindControl(_T("username"));
	m_pPassword = m_pm.FindControl(_T("password"));
	CenterWindow();

}


void CLogin::Notify( TNotifyUI& msg )
{
	CDuiString strControlName = msg.pSender->GetName();
	if (msg.sType == _T("click"))
	{
		if (strControlName == _T("ok"))
		{
			if (Login())
			{
				Close(IDOK);
			}
			return;
		}
		else if (strControlName == _T("cancel") || strControlName == _T("closebtn"))
		{
			Close(IDCANCEL);
			return;
		}
	}

	WindowImplBase::Notify(msg);
}

CControlUI* CLogin::CreateControl( LPCTSTR pstrClassName )
{

	return NULL;
}

bool CLogin::Login()
{
	CDuiString strUserName;
	CDuiString strPassword;
	if (m_pUserName)
	{
		strUserName = m_pUserName->GetText();
	}

	if (m_pPassword)
	{
		strPassword = m_pPassword->GetText();
	}

	if (strUserName == _T("love") && strPassword == _T("you"))
	{
		return true;
	}

	MessageBox(GetHWND(), _T("用户名或密码不正确"), _T("提示"), MB_OKCANCEL);

	return false;
}