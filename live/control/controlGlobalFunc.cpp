#include "stdafx.h"
#include "controlGlobalFunc.h"

namespace control_utl
{
	bool IsChildControl(CControlUI* pParent, CControlUI* pControl)
	{
		bool ret = false;
		CControlUI* pCurent = pControl ? pControl->GetParent() : NULL;
		while (pCurent)
		{
			if (pCurent == pParent)
			{
				ret = true;
				break;
			}

			pCurent = pCurent->GetParent();
		}

		return ret;
	}

	CControlUI* FindChildByName(CControlUI* pParent, LPCTSTR lpszName)
	{
		CPaintManagerUI* pManager = pParent ? pParent->GetManager() : nullptr;
		if (pManager)
		{
			return pManager->FindSubControlByName(pParent, lpszName);
		}

		return nullptr;
	}

}