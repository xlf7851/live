#include "stdafx.h"
#include "functioncallBase.h"

CFunctionCallItem::CFunctionCallItem()
{

}

CFunctionCallItem::~CFunctionCallItem()
{
	Release();
}

void CFunctionCallItem::Release()
{
	for (_param_container_t::iterator it = m_param.begin(); it != m_param.end(); it++)
	{
		delete (*it);
	}

	m_param.clear();
}

_tstring CFunctionCallItem::GetName() const
{
	return m_strName;
}

_tstring CFunctionCallItem::GetParamValue(LPCTSTR lpszKey) const
{
	_tstring strValue;
	FunctionParamItem* pItem = FindParamItem(lpszKey);
	if (pItem)
	{
		strValue = pItem->m_strValue;
	}

	return strValue;
}

bool CFunctionCallItem::IsAttributeTrue(LPCTSTR lpszKey) const
{
	_tstring strValue = GetParamValue(lpszKey);
	return !strValue.empty() && _tcsicmp(strValue.c_str(), _T("true")) == 0;
}

CFunctionCallItem::FunctionParamItem* CFunctionCallItem::FindParamItem(LPCTSTR lpKey) const
{
	for (_param_container_t::const_iterator it = m_param.begin(); it != m_param.end(); it++)
	{
		FunctionParamItem* pItem = *it;
		if (pItem && _tcsicmp(pItem->m_strKey.c_str(), lpKey) == 0)
		{
			return pItem;
		}
	}

	return nullptr;
}

void CFunctionCallItem::SetName(LPCTSTR lpszName)
{
	m_strName = lpszName;
}


void CFunctionCallItem::ParseParam(LPCTSTR lpszParam)
{
	std::vector<_tstring> vcValue;
	xlf::SplitString(vcValue, lpszParam, -1, _T(','), FALSE);
	for (int i = 0; i < vcValue.size(); i++)
	{
		_tstring key;
		_tstring value;
		xlf::ParseKeyValue(vcValue[i], key, value, _T(':'));
		xlf::TrimStringLeft(key, _T(' '));
		xlf::TrimStringRight(key, _T(' '));
		xlf::TrimStringLeft(value, _T(' '));
		xlf::TrimStringRight(value, _T(' '));
		if (!key.empty() && !value.empty())
		{
			FunctionParamItem* pItem = new FunctionParamItem;
			pItem->m_strKey = key;
			pItem->m_strValue = value;
			m_param.push_back(pItem);
		}
	}

}

