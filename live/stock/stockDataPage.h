#pragma once
#include "stockDataTable.h"
#include "stockArray.h"

class CBlockGroupTabContainerUI : public CHorizontalLayoutUI
{
	DECLARE_DUICONTROL(CBlockGroupTabContainerUI)
public:
	CBlockGroupTabContainerUI();
	~CBlockGroupTabContainerUI();

	virtual LPCTSTR GetClass() const;
	virtual LPVOID GetInterface(LPCTSTR pstrName);

	void BuildGroup(std::vector<uint32>& vcID, std::vector<_tstring>& vcName);
	void AddGroupItem(uint32 id, _tstring& name);
	void SelectedGroup(uint32 id);

protected:
};


class CBlockListItemUI : public CListLabelElementUI
{
public:
	CBlockListItemUI();

	void DoEvent(TEventUI& event);

	void SetBlockID(uint32 uid);
	void InitControl();

protected:
	uint32 m_uBlockID;
};


class CBlockListUI : public CListUI
{
	DECLARE_DUICONTROL(CBlockListUI)
public:
	CBlockListUI();
	~CBlockListUI();

	virtual LPCTSTR GetClass() const;
	virtual LPVOID GetInterface(LPCTSTR pstrName);

	void BuildList(uint32 uGroupID);
	void ClearList();

	void AddBlock(uint32 uid);

protected:
	uint32 m_uGroupID;
};

typedef  CBasePageUI _StockDataPageUIBase;
namespace stock_wrapper
{
	class MarketDayData;
}

class CStockDataTableUI;
class CStockDataPageUI : public _StockDataPageUIBase
{
	DECLARE_DUICONTROL(CStockDataPageUI)
public:
	CStockDataPageUI();
	~CStockDataPageUI();

	virtual LPCTSTR GetClass() const;
	virtual LPVOID GetInterface(LPCTSTR pstrName);

	virtual void OnNotify(TNotifyUI& msg);
	virtual void InitPage();
protected:
	void OnLoadData();
	bool OnSelectGroup(CControlUI* pControl);

	// block
	void OnNewBlock();


	// stock
	void OnImport();

protected:
	void LoadPoolData(LPCTSTR lpszFile);

	// list group
	void BuildGroup();
	void BuildBlockList();

	// block
	void DoNewBlock(LPCTSTR lpszName);

	// stock
	void UpdateStockList(const stock_wrapper::StockArray& ayStock);
	
protected:
	// block group container ctrl
	CBlockGroupTabContainerUI* m_pGroupContainerUI;
	// blocklist
	CBlockListUI* m_pBlockList;
	uint32 m_uCurrentBlockGroupID;

	// stocklist
	CListUI* m_pStockTable;
};