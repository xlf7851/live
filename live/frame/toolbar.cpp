#include "stdafx.h"
#include "toolbar.h"
#include "../Control/controlGlobalFunc.h"
#include "../global/GlobalFuncton.h"


IMPLEMENT_DUICONTROL(CToolbarPageUI)
CToolbarPageUI::CToolbarPageUI()
{
}

CToolbarPageUI::~CToolbarPageUI()
{
	
}

LPCTSTR CToolbarPageUI::GetClass() const
{
	return _T("ToolbarPageUI");
}

LPVOID CToolbarPageUI::GetInterface(LPCTSTR pstrName)
{
	if (_tcsicmp(pstrName, _T("ToolbarPage")) == 0)
	{
		return this;
	}

	return _ToolbarPageBase::GetInterface(pstrName);
}

void CToolbarPageUI::OnNotify(TNotifyUI& msg)
{
	CDuiString name = msg.pSender->GetName();
	if (msg.sType == DUI_MSGTYPE_SELECTCHANGED)
	{
		OnSelectTab(name);
	}
}

static CDuiString _GetPageResByTabName(const CDuiString& strName)
{
	static std::map<CDuiString, CDuiString> s_nameMap;
	if (s_nameMap.size() == 0)
	{
		s_nameMap[_T("toolbarBlock")] = _T("xml_block");
		s_nameMap[_T("toolbarBase64")] = _T("xml_base64");
		s_nameMap[_T("toolbarTestControl")] = _T("xml_test_control");
	}

	CDuiString strFind;
	std::map<CDuiString, CDuiString>::iterator it = s_nameMap.find(strName);
	if (it != s_nameMap.end())
	{
		strFind = it->second;
	}

	return strFind;
}

void CToolbarPageUI::OnSelectTab(const CDuiString& strName)
{
	if (m_strSelectTabName.Compare(strName) == 0)
	{
		return;
	}

	m_strSelectTabName = strName;
	if (m_strSelectTabName.IsEmpty())
	{
		return;
	}

	CDuiString strPageXml = _GetPageResByTabName(m_strSelectTabName);
	if (strPageXml.IsEmpty())
	{
		return;
	}
	global_funciton::ShowPage((LPCTSTR)strPageXml);
}
