#pragma once
#include "../page/basePage.h"


typedef  CBasePageUI _ToolbarPageBase;
class  CToolbarPageUI : public CBasePageUI
{
	DECLARE_DUICONTROL(CToolbarPageUI)
public:
	CToolbarPageUI();
	~CToolbarPageUI();

	virtual LPCTSTR GetClass() const;
	virtual LPVOID GetInterface(LPCTSTR pstrName);

	virtual void OnNotify(TNotifyUI& msg);

protected:
	void OnSelectTab(const CDuiString& strName);
private:
	CDuiString m_strSelectTabName;
};

