#pragma once




class CPageCreater : public IDialogBuilderCallback
{
public:
	CPageCreater(CPaintManagerUI& pm);
	~CPageCreater();

	CControlUI* CreatePage(LPCTSTR lpszSinkFile);

protected:
	CControlUI* CreateControl(LPCTSTR pstrClassName);

private:
	CPaintManagerUI& m_pm;
};

