#pragma once
#include "block.h"
#include "blockAsk.h"

class CBlockItemUI : public CListLabelElementUI
{
public:
	void DoEvent(TEventUI& event);
};

typedef  CBasePageUI _BlockCalcPageBase;
class CBlockcalcPageUI : public _BlockCalcPageBase, public stock_wrapper::IBlockAskerRecever
{
public:
	struct _block_list_info_t
	{
		stock_wrapper::CBlockList m_data;
		CListUI* m_pList;
		CDuiString m_strFileName;
		bool m_bLoad;

		_block_list_info_t()
		{
			m_pList = nullptr;
			m_bLoad = false;
		}
	};

	enum _block_group_type_t
	{
		_block_group_buy = 0,
		_block_group_sell = 1,
		_block_group_self = 2,
		_block_group_select = 3,
		_block_group_size
	};

	struct _block_ask_context 
	{
		_block_list_info_t* m_pList;
		DWORD m_dwBlock;
		DWORD m_dwTick;
		_block_ask_context()
		{
			m_dwBlock = 0;
			m_dwTick = 0;
		}
	};

	DECLARE_DUICONTROL(CBlockcalcPageUI)
public:
	CBlockcalcPageUI();
	~CBlockcalcPageUI();

	virtual LPCTSTR GetClass() const;
	virtual LPVOID GetInterface(LPCTSTR pstrName);
	virtual void OnNotify(TNotifyUI& msg);
	virtual void InitPage();

	virtual bool BlockStockRevever(void* pContext, int nErrorCode, const stock_wrapper::StockArray& ayCode);

protected:
	void OnUpdate();
	void OnUpdateAll();
	void OnExportStock();
	

	void OnNewBlock();
	void OnDeleteBlock();
	void OnBlockMofify();
	void OnSave();

	void OnUnion();
	void OnInner();
	void OnSub();
	void OnLoadCalcBlock();
	void OnDeleteCalcBlock();
	void OnAddCalcStockTo();

	BOOL OnCalcBlockItemChange(CControlUI* pList);
	BOOL OnBlockItemSelect(CControlUI* pList);
	int OnDoubleClickBlockItem(CControlUI* pItem, CListUI* pList);
	void OnChangeBlockGroup(int nIndex);
	void LoadBlockGroup(LPCTSTR lspzFilePath = nullptr);
	void LoadData();
	_block_list_info_t* GetCurrentList();
	DWORD GetCurrentSelectBlock(_block_list_info_t*& pList);
	void BuildBlockList(CListUI* pList, const stock_wrapper::CBlockList& data);
	void DeleteBlockItem(CListUI* pList, int nIndex, stock_wrapper::CBlockList& data);
	void AddBlockItem(CListUI* pList, DWORD dwBlockID, LPCTSTR lpszName);
	void UpdateCodeToList(CListUI* pList, const stock_wrapper::StockArray& ayCode);

private:
	_block_list_info_t m_blockList[_block_group_size];
	int m_nSelectBlockIndex;
	CTabLayoutUI* m_pBlockGroupTab;
	CListUI* m_pStockList;
	

	stock_wrapper::StockArray m_stock;		// 选出来的成分股

	CDuiString m_strConfigRoot;
	stock_wrapper::BlockAsker* m_pBlockAsk;
	_block_ask_context m_blockAskContext;

	// 计算板块
	stock_wrapper::CBlockList m_calcdata;
	CListUI* m_pCalcBlockList;
	CListUI* m_pCalcBlockStock;
	stock_wrapper::StockArray m_calcStock;
};



class CModifyBlockDlg : public WindowImplBase
{
public:
	CModifyBlockDlg(BOOL bNew = FALSE);
	~CModifyBlockDlg();

	CDuiString GetSkinFile();
	LPCTSTR GetWindowClassName() const;

	void SetBlockInfo(LPCTSTR lpszName, LPCTSTR lpszQuery = nullptr);
	void InitWindow();
	void Notify(TNotifyUI& msg);
	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	void GetResult(CDuiString& strName, CDuiString& strAsk);

protected:
	void OnSearch();
	void OnBlockAskReturn(WPARAM wParam, LPARAM lParam);
	void OnClickOK();
	
protected:
	BOOL m_bNew;
	CEditUI* m_pName;
	CRichEditUI* m_pQuery;
	CRichEditUI* m_pResult;

	CDuiString m_strOriName;
	CDuiString m_strOriQuery;

	CDuiString m_strName;
	CDuiString m_strQuery;

	stock_wrapper::BlockAsker* m_pBlockAsk;
};