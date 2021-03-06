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

	virtual LPCTSTR GetClass() const;
	virtual LPVOID GetInterface(LPCTSTR pstrName);

	void DoEvent(TEventUI& event);

	void SetBlockID(uint32 uid);
	uint32 GetBlockID();
	void InitControl();

protected:
	uint32 m_uBlockID;
};


class CBlockListUI : public CListUI, public INotifyUI
{
	DECLARE_DUICONTROL(CBlockListUI)
public:
	CBlockListUI();
	~CBlockListUI();

	virtual LPCTSTR GetClass() const;
	virtual LPVOID GetInterface(LPCTSTR pstrName);

	virtual void DoInit();
	void Notify(TNotifyUI& msg);

	void BuildList(uint32 uGroupID);
	void ClearList();

	void AddBlock(uint32 uid);

	uint32 GetCurrentBlockID();
	void SelectEdDefault();

	CBlockListItemUI* GetSelectedBlockItem();
	CBlockListItemUI* GetBlockItemAt(int index);

protected:
	void OnSelectEdBlock();

protected:
	uint32 m_uGroupID;
	uint32 m_uCurrentBlockID;
};


namespace stock_wrapper
{
	class MarketDayData;
}

class CStockDataTableUI;
class CStockDataPageUI : public CStockPageBaseUI
{
	DECLARE_DUICONTROL(CStockDataPageUI)
public:
	CStockDataPageUI();
	~CStockDataPageUI();

	virtual LPCTSTR GetClass() const;
	virtual LPVOID GetInterface(LPCTSTR pstrName);

	virtual void OnNotify(TNotifyUI& msg);
	virtual void InitPage();
	virtual void OnStockChangeEvent(HSynCode hSynCode, uint32 uCodeListID, uint64 uEvent);
protected:
	void OnLoadData();
	bool OnSelectGroup(CControlUI* pControl);

	// block
	void OnNewBlock();
	void OnSelectedBlock(uint32 blockid);


	// stock
	void OnImport();
	void OnSaveStock();

protected:
	void InitSynCode();
	void LoadPoolData(LPCTSTR lpszFile);

	// list group
	void BuildGroup();
	void BuildBlockList();

	// block
	void DoNewBlock(LPCTSTR lpszName);

	// stock
	void UpdateStockList(stock_wrapper::StockArray& ayStock);
	void GetStockList(stock_wrapper::StockArray& ayCode);
	void UpdateStockCountText();

	
	uint32 GetCurrentBlock(uint32& groupid);

	
	
protected:
	// block group container ctrl
	CBlockGroupTabContainerUI* m_pGroupContainerUI;
	// blocklist
	CBlockListUI* m_pBlockList;
	uint32 m_uCurrentBlockGroupID;

	// stocklist
	CStockDataTableUI* m_pStockTable;
	uint32 m_uCodeList = 0;

	// 
	stock_wrapper::StockArray m_aySrcStockCode;

	CLabelUI* m_pStockCountUI;
};