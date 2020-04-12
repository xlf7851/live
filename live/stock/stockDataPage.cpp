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

void CBlockListItemUI::DoEvent(TEventUI& event)
{
	CListLabelElementUI::DoEvent(event);
}

void CBlockListItemUI::InitBlockInfo(const _block_draw_item_it& item)
{
	m_blockInfo = item;
	SetText(m_blockInfo.m_strName.c_str());
}


IMPLEMENT_DUICONTROL(CBlockListUI)
CBlockListUI::CBlockListUI()
{
	m_uGroupID = 0;
}

CBlockListUI::~CBlockListUI()
{

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

void CBlockListUI::ClearList()
{
	RemoveAll();
}

void CBlockListUI::BuildList(uint32 uGroupID)
{
	m_uGroupID = uGroupID;
	ClearList();
	std::vector<_block_draw_item_it*> vcDrawItem;
	stock_wrapper::BlockGroupManager::Instance()->GetBlockDrawItemListInfo(m_uGroupID, vcDrawItem);
	for (int i = 0; i < vcDrawItem.size(); i++)
	{
		_block_draw_item_it* pItem = vcDrawItem[i];
		AddBlock(pItem);
		delete pItem;
	}
}

void CBlockListUI::AddBlock(_block_draw_item_it* pItem)
{
	if (nullptr == pItem)
	{
		return;
	}
	CBlockListItemUI* pControl = new CBlockListItemUI;
	pControl->InitBlockInfo(*pItem);
	Add(pControl);
}

void CBlockListUI::AddBlock(uint32 uid, LPCTSTR lpszName, uint32 uParam, int nCnt)
{
	_block_draw_item_it drawItem;
	drawItem.m_u32BlockID = uid;
	drawItem.m_uParam = uParam;
	drawItem.m_nStockCnt = nCnt;
	drawItem.m_strName = lpszName;
	AddBlock(&drawItem);
}


IMPLEMENT_DUICONTROL(CStockDataPageUI)

CStockDataPageUI::CStockDataPageUI()
{
	m_pGroupContainerUI = nullptr;
	m_pBlockList = nullptr;
	m_uCurrentBlockGroupID = 0;
	m_pStockTable = nullptr;
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
	}
	else if (msg.sType == DUI_MSGTYPE_SELECTCHANGED)
	{
		bool bGroup = OnSelectGroup(msg.pSender);
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
	if (CModifyTextDlg::ShowDlg(strNewText, _T("ÐÂ½¨°å¿é"), GetParentHwnd()) != IDOK || strNewText.IsEmpty())
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

	if (m_pBlockList)
	{
		m_pBlockList->AddBlock(uid, lpszName);
	}

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

void CStockDataPageUI::UpdateStockList(const stock_wrapper::StockArray& ayCode)
{
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
