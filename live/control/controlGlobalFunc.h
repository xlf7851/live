#pragma once

namespace control_utl
{
	bool IsChildControl(CControlUI* pParent, CControlUI* pControl);
	CControlUI* FindChildByName(CControlUI* pParent, LPCTSTR lpszName);


}