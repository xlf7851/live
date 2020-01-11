#pragma once


typedef  CHorizontalLayoutUI _CaptionPageBase;
class  CCaptionPageUI : public _CaptionPageBase, public INotifyUI
{
	DECLARE_DUICONTROL(CCaptionPageUI)
public:
	CCaptionPageUI();
	~CCaptionPageUI();

	virtual LPCTSTR GetClass() const;
	virtual LPVOID GetInterface(LPCTSTR pstrName);

	virtual void Notify(TNotifyUI& msg);
	virtual void DoInit();

protected:
	void OnSkinSetting();
	void OnSetting();
private:

};

