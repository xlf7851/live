#pragma once
#include "stockDataTable.h"
#include "stockArray.h"


class CStockPageBaseUI : public CBasePageUI, public IStockChangeEventHandler
{
	DECLARE_DUICONTROL(CStockPageBaseUI)
public:
	CStockPageBaseUI();
	~CStockPageBaseUI();

	virtual LPCTSTR GetClass() const;
	virtual LPVOID GetInterface(LPCTSTR pstrName);

	virtual void OnStockChangeEvent(HSynCode hSynCode, uint32 uCodeListID, uint64 uEvent){}

	HSynCode AddSynCode(uint32& codelistid);
	HSynCode FindSynCode(uint32 codelistid);

protected:
	void ReleaseSynCode();
	uint32 NewSynCodeID();

protected:
	CPtrVector m_vcSyncode;
};