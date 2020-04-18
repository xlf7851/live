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

	int GetCodeSize();		// ��ȡ�������
	int GetScrollIndex();	// ��ȡ��ǰ��������������ǰҳ��һ����������
	int GetCurIndex();		// ��ȡ��ǰѡ��Ĵ���
	int GetCodePage();		// ��ȡҳ��Ĵ������
	
	

	stock_wrapper::StockCode GetCodeAt(int index);

protected:
	int m_nID = 0;				// Ψһ��ʶ
	int m_nScrollPos = 0;		// ������
	int m_nPage = 1;			// һҳ�ĸ���
	int m_nCurpos = -1;			// ��ǰѡ�е�

};

