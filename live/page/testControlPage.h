#pragma once

#include "basePage.h"

typedef  CBasePageUI _TestControlPageUIBase;
class CTestControlPageUI : public _TestControlPageUIBase
{
	DECLARE_DUICONTROL(CTestControlPageUI)
public:
	CTestControlPageUI();
	~CTestControlPageUI();

	virtual LPCTSTR GetClass() const;
	virtual LPVOID GetInterface(LPCTSTR pstrName);

	virtual void InitPage();

	virtual void OnNotify(TNotifyUI& msg);
protected:
	void OnScreenshot();
	void OnTestVirtualList();

};
