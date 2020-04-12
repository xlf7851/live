#pragma once
#include "stockDataTable.h"
#include "stockDefine.h"
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
	void DoEvent(TEventUI& event);

	void InitBlockInfo(const _block_draw_item_it& item);

protected:
	_block_draw_item_it m_blockInfo;
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

	void AddBlock(_block_draw_item_it* pItem);
	void AddBlock(uint32 uid, LPCTSTR lpszName, uint32 uParam = 0, int nCnt = 0);

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