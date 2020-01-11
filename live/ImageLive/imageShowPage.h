#pragma once

typedef  CVerticalLayoutUI _ImageShowPageBase;
class  CImageShowPage : public _ImageShowPageBase
{
public:
	CImageShowPage();
	~CImageShowPage();

	virtual LPCTSTR GetClass() const;
	virtual LPVOID GetInterface(LPCTSTR pstrName);


private:

};