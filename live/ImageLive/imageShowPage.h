#pragma once

class  CImageShowPageUI : public CBasePageUI
{
public:
	CImageShowPageUI();
	~CImageShowPageUI();

	virtual LPCTSTR GetClass() const;
	virtual LPVOID GetInterface(LPCTSTR pstrName);


private:

};