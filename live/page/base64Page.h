#pragma once
#include "basePage.h"

typedef  CBasePageUI _Base64PageUIBase;
class CBase64PageUI : public _Base64PageUIBase
{
	DECLARE_DUICONTROL(CBase64PageUI)
public:
	CBase64PageUI();
	~CBase64PageUI();

	virtual LPCTSTR GetClass() const;
	virtual LPVOID GetInterface(LPCTSTR pstrName);

	virtual void OnNotify(TNotifyUI& msg);
	virtual void InitPage();

protected:
	void OnEncode();
	void OnDecode();
	void ReadEditControl();

private:
	CRichEditUI* m_pSrcEdit;
	CRichEditUI* m_pDstEdit;
};