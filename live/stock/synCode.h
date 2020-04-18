#pragma once
#include "stockArray.h"



class CCodeLayoutUI : public CVerticalLayoutUI
{
	DECLARE_DUICONTROL(CCodeLayoutUI)
public:
	CCodeLayoutUI();
	~CCodeLayoutUI();

	virtual LPCTSTR GetClass() const;
	virtual LPVOID GetInterface(LPCTSTR pstrName);

};


class SynCode
{
public:
	SynCode() = default;
	~SynCode();

	int GetCodeSize();		// 获取代码个数
	int GetScrollIndex();	// 获取当前滚动索引，即当前页第一个代码索引
	int GetCurIndex();		// 获取当前选择的代码
	int GetCodePage();		// 获取页面的代码个数
	
	

	stock_wrapper::StockCode GetCodeAt(int index);

protected:
	int m_nID = 0;				// 唯一标识
	int m_nScrollPos = 0;		// 滚动点
	int m_nPage = 1;			// 一页的个数
	int m_nCurpos = -1;			// 当前选中的

};

