#include "stdafx.h"
#include "stockDataPage.h"
#include "../control/controlGlobalFunc.h"
#include <TlHelp32.h>
#include <tinyxml2.h>
#include "hqDataQuery.h"
#include "../Control/openFileDlg.h"
#include "../Control/modifyTextDlg.h"
#include "stockdata.h"
#include "stockDataTable.h"
#include "stockdataFile.h"
#include "block.h"

#define BLOCK_CONTROL_EVENT_SELECTED_BLOCK (_T("BLOCK_CONTROL_EVENT_SELECTED_BLOCK"))

IMPLEMENT_DUICONTROL(CBlockGroupTabContainerUI)
CBlockGroupTabContainerUI::CBlockGroupTabContainerUI()
{

}

CBlockGroupTabContainerUI::~CBlockGroupTabContainerUI()
{

}

LPCTSTR CBlockGroupTabContainerUI::GetClass() const
{
	return DUI_CUSTOM_CTRL_CLASS_BlockGroupTabContainer;
}

LPVOID CBlockGroupTabContainerUI::GetInterface(LPCTSTR pstrName)
{
	if (_tcsicmp(pstrName, DUI_CUSTOM_CTRL_INTERFACE_BlockGroupTabContainer) == 0)
	{
		return static_cast<CBlockGroupTabContainerUI*>(this);
	}

	return CHorizontalLayoutUI::GetInterface(pstrName);
}

void CBlockGroupTabContainerUI::BuildGroup(std::vector<uint32>& vcID, std::vector<_tstring>& vcName)
{
	for (int i = 0; i < vcID.size(); i++)
	{
		AddGroupItem(vcID[i], vcName[i]);
	}
}

void CBlockGroupTabContainerUI::AddGroupItem(uint32 id, _tstring& name)
{
	COptionUI* pItem = new COptionUI;
	CDuiString strStyle = m_pManager->GetStyle(_T("stockDataPageOptionStyle"));
	if (!strStyle.IsEmpty())
	{
		pItem->ApplyAttributeList(strStyle);
	}
	else
	{
		pItem->SetAutoCalcWidth(true);
		pItem->SetGroup(_T("stockDataPageBlockGroup"));
	}
	
	pItem->SetText(name.c_str());
	pItem->SetTag(id);
	Add(pItem);
}

void CBlockGroupTabContainerUI::SelectedGroup(uint32 id)
{
	int nCount = GetCount();
	for (int i = 0; i < nCount; i++)
	{
		COptionUI* pItem = (COptionUI*)GetItemAt(i);
		if (pItem && pItem->GetTag() == id)
		{
			pItem->Selected(true);
		}
	}
}

CBlockListItemUI::CBlockListItemUI()
{
	m_uBlockID = 0;
}

LPCTSTR CBlockListItemUI::GetClass() const
{
	return DUI_CUSTOM_CTRL_CLASS_BlockListItem;
}

LPVOID CBlockListItemUI::GetInterface(LPCTSTR pstrName)
{
	if (_tcsicmp(pstrName, DUI_CUSTOM_CTRL_INTERFACE_BlockListItem) == 0)
	{
		return static_cast<CBlockListItemUI*>(this);
	}

	return CListLabelElementUI::GetInterface(pstrName);
}


void CBlockListItemUI::DoEvent(TEventUI& event)
{
	CListLabelElementUI::DoEvent(event);
}

void CBlockListItemUI::SetBlockID(uint32 uid)
{
	m_uBlockID = uid;
}

uint32 CBlockListItemUI::GetBlockID()
{
	return m_uBlockID;
}

void CBlockListItemUI::InitControl()
{
	_tstring strName;
	stock_wrapper::BlockCacheManager::Instance()->QueryBlock(m_uBlockID, &strName);
	SetText(strName.c_str());
}


IMPLEMENT_DUICONTROL(CBlockListUI)
CBlockListUI::CBlockListUI()
{
	m_uGroupID = 0;
	m_uCurrentBlockID = 0;
}

CBlockListUI::~CBlockListUI()
{
	if (m_pManager)
	{
		m_pManager->RemoveNotifier(this);
	}
}

LPCTSTR CBlockListUI::GetClass() const
{
	return DUI_CUSTOM_CTRL_CLASS_BlockList;
}

LPVOID CBlockListUI::GetInterface(LPCTSTR pstrName)
{
	if (_tcsicmp(pstrName, DUI_CUSTOM_CTRL_INTERFACE_BlockList) == 0)
	{
		return static_cast<CBlockListUI*>(this);
	}

	return CListUI::GetInterface(pstrName);
}

void CBlockListUI::DoInit()
{
	CListUI::DoInit();
	if (m_pManager)
	{
		m_pManager->AddNotifier(this);
	}
}

void CBlockListUI::Notify(TNotifyUI& msg)
{
	if (!(msg.pSender == this || control_utl::IsChildControl(this, msg.pSender)))
	{
		return;
	}

	CDuiString strName = msg.pSender->GetName();
	if (msg.sType.CompareNoCase(DUI_MSGTYPE_ITEMSELECT) == 0)
	{
		if (msg.pSender == this)
		{
			OnSelectEdBlock();
		}
	}
}

void CBlockListUI::ClearList()
{
	RemoveAll();
}

void CBlockListUI::BuildList(uint32 uGroupID)
{
	m_uGroupID = uGroupID;
	m_uCurrentBlockID = 0;
	ClearList();
	BlockIDArray ayBlock;
	stock_wrapper::BlockGroupManager::Instance()->GetBlockArray(m_uGroupID, ayBlock);
	for (int i = 0; i < ayBlock.GetSize(); i++)
	{
		AddBlock(ayBlock[i]);
	}

	SelectEdDefault();
}

void CBlockListUI::AddBlock(uint32 uid)
{
	CBlockListItemUI* pControl = new CBlockListItemUI;
	pControl->SetBlockID(uid);
	pControl->InitControl();
	Add(pControl);
}

uint32 CBlockListUI::GetCurrentBlockID()
{
	return m_uCurrentBlockID;
}

void CBlockListUI::SelectEdDefault()
{
	if (GetCount() > 0)
	{
		SelectItem(0);
	}
}

CBlockListItemUI* CBlockListUI::GetBlockItemAt(int index)
{
	int size = GetCount();
	if (size == 0 || index < 0 || index >= size)
	{
		return nullptr;
	}

	CControlUI* pControl = GetItemAt(index);
	return pControl ? static_cast<CBlockListItemUI*>(pControl->GetInterface(DUI_CUSTOM_CTRL_INTERFACE_BlockListItem)) : nullptr;
}

CBlockListItemUI* CBlockListUI::GetSelectedBlockItem()
{
	return GetBlockItemAt(GetCurSel());
}

void CBlockListUI::OnSelectEdBlock()
{
	CBlockListItemUI* pItem = GetSelectedBlockItem();
	if (pItem)
	{
		uint32 uBlockID = pItem->GetBlockID();
		if (uBlockID != m_uCurrentBlockID)
		{
			m_uCurrentBlockID = uBlockID;
			if (m_pManager)
			{
				m_pManager->SendNotify(this, BLOCK_CONTROL_EVENT_SELECTED_BLOCK, uBlockID, 0, true);
			}
		}
	}
}

IMPLEMENT_DUICONTROL(CStockDataPageUI)

CStockDataPageUI::CStockDataPageUI()
{
	m_pGroupContainerUI = nullptr;
	m_pBlockList = nullptr;
	m_uCurrentBlockGroupID = 0;
	m_pStockTable = nullptr;
	m_pStockCountUI = nullptr;
}

CStockDataPageUI::~CStockDataPageUI()
{

}

LPCTSTR CStockDataPageUI::GetClass() const
{
	return DUI_CUSTOM_CTRL_CLASS_StockDataPage;
}

LPVOID CStockDataPageUI::GetInterface(LPCTSTR pstrName)
{
	if (_tcsicmp(pstrName, DUI_CUSTOM_CTRL_INTERFACE_StockDataPage) == 0) return static_cast<CStockDataPageUI*>(this);
	return _StockDataPageUIBase::GetInterface(pstrName);

	
}

void CStockDataPageUI::InitPage()
{
	m_pGroupContainerUI = static_cast<CBlockGroupTabContainerUI*>(control_utl::FindChildByName(this, _T("stockDataPageGroupContainer")));
	BuildGroup();

	m_pBlockList = static_cast<CBlockListUI*>(control_utl::FindChildByName(this, _T("stockDataPageBlocklist")));

	m_pStockTable = static_cast<CListUI*>(control_utl::FindChildByName(this, _T("stockDataPageStockTable")));
	m_pStockCountUI = static_cast<CLabelUI*>(control_utl::FindChildByName(this, _T("stockDataPageBtnLabelStockCnt")));

	if (m_pGroupContainerUI)
	{
		m_pGroupContainerUI->SelectedGroup(BLOCK_GROUP_ID_SELF);
	}
}

void CStockDataPageUI::OnNotify(TNotifyUI& msg)
{
	CDuiString name = msg.pSender->GetName();
	if (msg.sType == DUI_MSGTYPE_CLICK)
	{
		if (name.CompareNoCase(_T("btnStockhisdataPageLoad")) == 0)
		{
			OnLoadData();
		}
		else if (name.CompareNoCase(_T("stockDataPageBtnNewBlock")) == 0)
		{
			OnNewBlock();
		}
		else if (name.CompareNoCase(_T("stockDataPageBtnImport")) == 0)
		{
			OnImport();
		}
		else if (name.CompareNoCase(_T("stockDataPageBtnSaveStock")) == 0)
		{
			OnSaveStock();
		}
	}
	else if (msg.sType == DUI_MSGTYPE_SELECTCHANGED)
	{
		bool bGroup = OnSelectGroup(msg.pSender);
	}
	else if (msg.sType == BLOCK_CONTROL_EVENT_SELECTED_BLOCK)
	{
		OnSelectedBlock((uint32)msg.wParam);
	}
 }



void CStockDataPageUI::OnLoadData()
{
	COpenFileHelp openFileHelp;
	if (!openFileHelp.BrowseForFolder())
	{
		return;
	}

	CDuiString strPath = openFileHelp.GetFilePath();
	if (strPath.IsEmpty())
	{
		return;
	}

	LoadPoolData(strPath);
}


void CStockDataPageUI::LoadPoolData(LPCTSTR lpszFile)
{
	stock_wrapper::CLoadThsStockFile::Load(lpszFile);
}


void CStockDataPageUI::BuildGroup()
{
	if (nullptr == m_pGroupContainerUI)
	{
		return;
	}

	std::vector<uint32> groupID;
	std::vector<_tstring> groupName;
	stock_wrapper::BlockGroupManager::Instance()->GetAllBlockGroupIDAndName(groupID, groupName);
	if (groupName.size() != groupID.size())
	{
		return;
	}

	m_pGroupContainerUI->BuildGroup(groupID, groupName);
}

bool CStockDataPageUI::OnSelectGroup(CControlUI* pControl)
{
	if (m_pGroupContainerUI == nullptr || !control_utl::IsChildControl(m_pGroupContainerUI, pControl))
	{
		return false;
	}

	uint32 uGroupID = pControl->GetTag();
	if (m_uCurrentBlockGroupID != uGroupID)
	{
		m_uCurrentBlockGroupID = uGroupID;
		BuildBlockList();
	}

	return true;
}


void CStockDataPageUI::BuildBlockList()
{
	if (m_uCurrentBlockGroupID == 0 || m_pBlockList == nullptr)
	{
		return;
	}

	m_pBlockList->BuildList(m_uCurrentBlockGroupID);
}

void CStockDataPageUI::OnNewBlock()
{
	CDuiString strNewText;
	if (CModifyTextDlg::ShowDlg(strNewText, _T("新建板块"), GetParentHwnd()) != IDOK || strNewText.IsEmpty())
	{
		return;
	}

	DoNewBlock(strNewText);
}

void CStockDataPageUI::DoNewBlock(LPCTSTR lpszName)
{
	_tstring strName = lpszName;
	uint32  uid = stock_wrapper::BlockCacheManager::Instance()->NewBlock(strName);
	if (uid == 0)
	{
		return;
	}

	stock_wrapper::BlockGroupManager::Instance()->AddBlock(m_uCurrentBlockGroupID, uid);

	if (m_pBlockList)
	{
		m_pBlockList->AddBlock(uid);
	}

}

void CStockDataPageUI::OnSelectedBlock(uint32 blockid)
{
	m_aySrcStockCode.ClearStockCode();
	stock_wrapper::BlockCacheManager::Instance()->QueryStock(blockid, m_aySrcStockCode);
	m_ayResultCode.ClearStockCode();
	UpdateStockList(m_aySrcStockCode);
	
}

void CStockDataPageUI::OnImport()
{
	COpenFileHelp fileHelp;
	if (!fileHelp.OpenFile())
	{
		return;
	}

	_tstring strFile = fileHelp.GetFilePath();
	if (strFile.empty())
	{
		return;
	}

	xlf::CBuffer buf;
	xlf::ReadBufferFromFile(buf, strFile.c_str());
	if (buf.GetSize() == 0)
	{
		return;
	}

	stock_wrapper::StockArray ayCode;
	ayCode.FromString((char*)buf.GetBuffer(), buf.GetSize());
	if (ayCode.GetStockCodeSize() == 0)
	{
		return;
	}

	UpdateStockList(ayCode);
	
}

void CStockDataPageUI::OnSaveStock()
{
	stock_wrapper::StockArray& ayCode = GetCurrentCodeList();
	if (ayCode.GetStockCodeSize() == 0)
	{
		return;
	}

	uint32 groupid = 0;
	uint32 blockid = GetCurrentBlock(groupid);
	if (blockid == 0)
	{
		return;
	}

	stock_wrapper::BlockCacheManager::Instance()->UpdateStock(blockid, ayCode);
}

void CStockDataPageUI::UpdateStockList(const stock_wrapper::StockArray& ayCode)
{
	m_ayResultCode = ayCode;
	UpdateStockCountText();
	if (m_pStockTable == nullptr)
	{
		return;
	}

	m_pStockTable->RemoveAll();

	int nSize = ayCode.GetStockCodeSize();
	for (int i = 0; i < nSize; i++)
	{
		const stock_wrapper::StockCode* code = ayCode.GetStockCodeAt(i);
		
		if (code->Empty())
		{
			continue;
		}

		stock_wrapper::Stock stock = code->GetStock();

		CListLabelElementUI* pNew = new CListLabelElementUI;
		pNew->SetOwner(m_pStockTable);
		m_pStockTable->Add(pNew);
		pNew->SetFixedHeight(24);
		pNew->SetText((LPCTSTR)stock);
	}
}

void CStockDataPageUI::UpdateStockCountText()
{
	int nCnt = m_ayResultCode.GetStockCodeSize();
	CDuiString strText;
	strText.Format(_T("个股(%d)"), nCnt);
	if (m_pStockCountUI)
	{
		m_pStockCountUI->SetText(strText);
	}

}

stock_wrapper::StockArray& CStockDataPageUI::GetCurrentCodeList()
{
	return m_ayResultCode;
}

uint32 CStockDataPageUI::GetCurrentBlock(uint32& groupid)
{
	groupid = m_uCurrentBlockGroupID;
	if (groupid == 0)
	{
		return 0;
	}

	if (m_pBlockList == nullptr)
	{
		return 0;
	}

	return m_pBlockList->GetCurrentBlockID();
}