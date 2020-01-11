#pragma once

class CLogin: public WindowImplBase
{
public:
	CLogin();
	~CLogin();

	void InitWindow();
	void Notify(TNotifyUI& msg);
	CControlUI* CreateControl(LPCTSTR pstrClassName) override;

protected:
	LPCTSTR GetWindowClassName() const override;
	CDuiString GetSkinFile() override;

	bool Login();

private:
	CControlUI* m_pUserName;
	CControlUI* m_pPassword;

};