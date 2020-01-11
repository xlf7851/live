#include "stdafx.h"
#include "PageCreater.h"
#include "blockcalcPage.h"
#include "base64Page.h"


void CPageClassNameMgr::Init()
{
	m_data[_T("ImageShowPage")] = _T("CImageShowPage");
}

LPCTSTR CPageClassNameMgr::GetClassName(LPCTSTR lpszClass)
{
	_PageClassMap_t::iterator it = m_data.find(lpszClass);
	if (it != m_data.end())
	{
		return it->second;
	}

	return _T("");
}

CPageCreater::CPageCreater(CPaintManagerUI& pm)
	:m_pm(pm)
{
	m_classNameMgr.Init();
}


CPageCreater::~CPageCreater()
{
}





CControlUI* CPageCreater::CreateControl(LPCTSTR pstrClass)
{
// 	if (_tcsicmp(pstrClass, _T("Base64Page")) == 0)
// 	{
// 		return new CBase64PageUI;
// 	}
// 	else if (_tcsicmp(pstrClass, _T("BlockcalcPage")) == 0)
// 	{
// 		return new CBlockcalcPageUI;
// 	}
	return nullptr;
}

CControlUI* CPageCreater::CreatePage(LPCTSTR lpszSinkFile)
{
	CDialogBuilder builder;
	return builder.Create(lpszSinkFile, 0, this, &m_pm);
}