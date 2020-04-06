#include "stdafx.h"
#include "imageShowPage.h"


CImageShowPageUI::CImageShowPageUI()
{
}

CImageShowPageUI::~CImageShowPageUI()
{

}

LPCTSTR CImageShowPageUI::GetClass() const
{
	return _T("ImageShowPageUI");
}

LPVOID CImageShowPageUI::GetInterface(LPCTSTR pstrName)
{
	if (_tcsicmp(pstrName, _T("ImageShowPage")) == 0)
	{
		return this;
	}

	return CBasePageUI::GetInterface(pstrName);
}
