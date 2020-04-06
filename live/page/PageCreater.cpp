#include "stdafx.h"
#include "PageCreater.h"




CPageCreater::CPageCreater(CPaintManagerUI& pm)
	:m_pm(pm)
{
	
}


CPageCreater::~CPageCreater()
{
}


CControlUI* CPageCreater::CreateControl(LPCTSTR pstrClass)
{
	return nullptr;
}

CControlUI* CPageCreater::CreatePage(LPCTSTR lpszSinkFile)
{
	CDialogBuilder builder;
	return builder.Create(lpszSinkFile, 0, this, &m_pm);
}