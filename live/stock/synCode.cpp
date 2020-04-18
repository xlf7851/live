#include "stdafx.h"
#include "synCode.h"


IMPLEMENT_DUICONTROL(CCodeLayoutUI)

CCodeLayoutUI::CCodeLayoutUI()
{
}

CCodeLayoutUI::~CCodeLayoutUI()
{

}

LPCTSTR CCodeLayoutUI::GetClass() const
{
	return DUI_CUSTOM_CTRL_CLASS_StockDataPage;
}

LPVOID CCodeLayoutUI::GetInterface(LPCTSTR pstrName)
{
	if (_tcsicmp(pstrName, DUI_CUSTOM_CTRL_INTERFACE_StockDataPage) == 0) return static_cast<CCodeLayoutUI*>(this);
	return CVerticalLayoutUI::GetInterface(pstrName);


}



SynCode::~SynCode()
{

}

