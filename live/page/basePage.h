#pragma once

typedef  CVerticalLayoutUI _BasePageUIBase;
class CBasePageUI : public _BasePageUIBase, public INotifyUI
{
	DECLARE_DUICONTROL(CBasePageUI)
public:
	CBasePageUI();
	virtual ~CBasePageUI();

	virtual LPCTSTR GetClass() const;
	virtual LPVOID GetInterface(LPCTSTR pstrName);
	virtual void DoInit();
	virtual void Notify(TNotifyUI& msg);
	virtual bool IsThisNotify(TNotifyUI& msg);
	virtual void OnNotify(TNotifyUI& msg);
	virtual void InitPage(){}

public:
	HWND GetParentHwnd();
};