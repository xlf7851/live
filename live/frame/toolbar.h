#pragma once
#include "../page/basePage.h"

class CToolbarItemUI : public CButtonUI
{
	DECLARE_DUICONTROL(CToolbarItemUI)
public:
	CToolbarItemUI();
	~CToolbarItemUI();

	virtual LPCTSTR GetClass() const;
	virtual LPVOID GetInterface(LPCTSTR pstrName);

	CFunctionCallItem& GetFunctionCall();

	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

protected:
	CFunctionCallItem m_functionCall;
};


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
	void OnClickToolBarItem(CToolbarItemUI* pItem);
private:
	CDuiString m_strSelectTabName;
};

