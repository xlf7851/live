#pragma once


class CPageClassNameMgr
{
public:
	void Init();

	LPCTSTR GetClassName(LPCTSTR lpszClass);

protected:
	typedef std::map<LPCTSTR, LPCTSTR> _PageClassMap_t;

	_PageClassMap_t m_data;
};

class CPageCreater : public IDialogBuilderCallback
{
public:
	CPageCreater(CPaintManagerUI& pm);
	~CPageCreater();

	CControlUI* CreatePage(LPCTSTR lpszSinkFile);

protected:
	CControlUI* CreateControl(LPCTSTR pstrClassName);

private:
	CPageClassNameMgr m_classNameMgr;

	CPaintManagerUI& m_pm;
};

