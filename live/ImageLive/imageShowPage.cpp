#include "stdafx.h"
#include "imageShowPage.h"


CImageShowPage::CImageShowPage()
{
}

CImageShowPage::~CImageShowPage()
{

}

LPCTSTR CImageShowPage::GetClass() const
{
	return _T("ImageShowPageUI");
}

LPVOID CImageShowPage::GetInterface(LPCTSTR pstrName)
{
	if (_tcsicmp(pstrName, _T("ImageShowPage")) == 0)
	{
		return this;
	}

	return _ImageShowPageBase::GetInterface(pstrName);
}
