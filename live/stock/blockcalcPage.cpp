#include "stdafx.h"
#include "blockcalcPage.h"
#include "blockAsk.h"
#include "../base/file.h"

void CBlockItemUI::DoEvent(TEventUI& event)
{
	if (event.Type == UIEVENT_DBLCLICK)
	{
		if (m_pManager)
		{
			m_pManager->SendNotify(this, DUI_MSGTYPE_ITEMDBCLICK);
		}
		return;
	}

	CListLabelElementUI::DoEvent(event);
}

IMPLEMENT_DUICONTROL(CBlockcalcPageUI)

CBlockcalcPageUI::CBlockcalcPageUI()
{
	m_pBlockGroupTab = nullptr;
	m_nSelectBlockIndex = -1;
	m_pStockList = nullptr;
	m_pBlockAsk = nullptr;
	m_pCalcBlockList = nullptr;
	m_pCalcBlockStock = nullptr;
}

CBlockcalcPageUI::~CBlockcalcPageUI()
{
	
}

LPCTSTR CBlockcalcPageUI::GetClass() const
{
	return DUI_CUSTOM_CTRL_CLASS_BlockcalcPage;
}

LPVOID CBlockcalcPageUI::GetInterface(LPCTSTR pstrName)
{
	if (_tcsicmp(pstrName, DUI_CUSTOM_CTRL_INTERFACE_BlockcalcPage) == 0)
	{
		return static_cast<CBlockcalcPageUI*>(this);
	}

	return _BlockCalcPageBase::GetInterface(pstrName);
}

void CBlockcalcPageUI::InitPage()
{
	m_strConfigRoot = global_funciton::GetConfigDir();
	if (!m_strConfigRoot.IsEmpty())
	{
		m_strConfigRoot += _T("block\\");
	}

	m_pBlockGroupTab = static_cast<CTabLayoutUI*>(control_utl::FindChildByName(this, _T("blockcalcPageBlockSwitch")));

	LPCTSTR ayBlockGroupName[_block_group_size] = {
		_T("blockcalcPageListBuy"),
		_T("blockcalcPageListSell"),
		_T("blockcalcPageListSelf"),
		_T("blockcalcPageListSelect")
	};

	LPCTSTR ayBlockGroupFileName[_block_group_size] = {
		_T("buy.dat"),
		_T("sell.dat"),
		_T("self.dat"),
		_T("select.dat")
	};

	for (int i = 0; i < _block_group_size; i++)
	{
		m_blockList[i].m_pList =  static_cast<CListUI*>(control_utl::FindChildByName(this, ayBlockGroupName[i]));
		if (!m_strConfigRoot.IsEmpty())
		{
			m_blockList[i].m_strFileName = m_strConfigRoot + ayBlockGroupFileName[i];
		}
	}

	m_pStockList = static_cast<CListUI*>(control_utl::FindChildByName(this, _T("blockcalcPageListStock")));

	m_pCalcBlockList = static_cast<CListUI*>(control_utl::FindChildByName(this, _T("blockcalcPageListCalcBlock")));
	m_pCalcBlockStock = static_cast<CListUI*>(control_utl::FindChildByName(this, _T("blockcalcPageListCalcStock")));

	LoadData();

	COptionUI* pOpt = static_cast<COptionUI*>(control_utl::FindChildByName(this, _T("blockcalcPageOptBuy")));
	if (pOpt)
	{
		pOpt->Selected(true);
	}

}

static LPCTSTR g_ayBlockGroupOptName[CBlockcalcPageUI::_block_group_size] = {
		_T("blockcalcPageOptBuy"),
		_T("blockcalcPageOptSell"),
		_T("blockcalcPageOptSelf"),
		_T("blockcalcPageOptSelect")
};

static int _FindBlockGroupOptIndex(LPCTSTR lpszName)
{
	for (int i = 0; i < CBlockcalcPageUI::_block_group_size; i++)
	{
		if (_tcsicmp(lpszName, g_ayBlockGroupOptName[i]) == 0)
		{
			return i;
		}
	}

	return -1;
}

void CBlockcalcPageUI::OnNotify(TNotifyUI& msg)
{
	CDuiString name = msg.pSender->GetName();
	if (msg.sType == DUI_MSGTYPE_CLICK)
	{
		if (name == _T("blockcalcPageBtnUpdate"))
		{
			OnUpdate();
		}
		else if (name == _T("blockcalcPageBtnModify"))
		{
			OnBlockMofify();
		}
		else if (name == _T("blockcalcPageBtnNew"))
		{
			OnNewBlock();
		}
		else if (name == _T("blockcalcPageBtnSave"))
		{
			OnSave();
		}
		else if (name == _T("blockcalcPageBtnDelete"))
		{
			OnDeleteBlock();
		}
		else if (name == _T("blockcalcPageBtnUnion"))
		{
			OnUnion();
		}
		else if (name == _T("blockcalcPageBtnInner"))
		{
			OnInner();
		}
		else if (name == _T("blockcalcPageBtnOut"))
		{
			OnSub();
		}
		else if (name == _T("blockcalcPageBtnLoadCalc"))
		{
			OnLoadCalcBlock();
		}
		else if (name == _T("blockcalcPageBtnDeleteCalc"))
		{
			OnDeleteCalcBlock();
		}
		else if (name == _T("blockcalcPageBtnCalcAddTo"))
		{
			OnAddCalcStockTo();
		}
		else if (name == _T("blockcalcPageBtnExportStock"))
		{
			OnExportStock();
		}
	}
	else if (msg.sType == DUI_MSGTYPE_SELECTCHANGED)
	{
		int nIndex = _FindBlockGroupOptIndex((LPCTSTR)name);
		OnChangeBlockGroup(nIndex);
	}
	else if (msg.sType == DUI_MSGTYPE_ITEMSELECT)
	{
		BOOL bRet = OnBlockItemSelect(msg.pSender);
		if (!bRet)
		{
			bRet = OnCalcBlockItemChange(msg.pSender);
		}
	}
	else if (msg.sType == DUI_MSGTYPE_ITEMDBCLICK)
	{
// 		int nIndex = OnDoubleClickBlockItem(msg.pSender, m_pCalcBlockList);
// 		if (nIndex != 0)
// 		{
// 			DeleteBlockItem(m_pCalcBlockList, nIndex, m_calcdata);
// 		}
	}
}

void CBlockcalcPageUI::OnUpdate()
{
	_block_list_info_t* pItem = nullptr;
	DWORD dwBlockID = GetCurrentSelectBlock(pItem);
	if (pItem == nullptr || dwBlockID == 0)
	{
		return;
	}

	std::string strName;
	std::string strQuery;
	pItem->m_data.Query(dwBlockID, strName, strQuery);
	if (strQuery.empty())
	{
		return;
	}


	if (nullptr == m_pBlockAsk)
	{
		m_pBlockAsk = new stock_wrapper::BlockAsker;
		m_pBlockAsk->Init(nullptr, this);
	}
	m_blockAskContext.m_pList = pItem;
	m_blockAskContext.m_dwBlock = dwBlockID;
	m_blockAskContext.m_dwTick = ::GetTickCount();
	m_pBlockAsk->Ask(strQuery.c_str(), (void*)m_blockAskContext.m_dwTick);
}

void CBlockcalcPageUI::OnExportStock()
{
	_block_list_info_t* pItem = nullptr;
	DWORD dwBlockID = GetCurrentSelectBlock(pItem);
	if (pItem == nullptr || dwBlockID == 0)
	{
		return;
	}

	stock_wrapper::StockArray ayCode;
	pItem->m_data.QueryStock(dwBlockID, ayCode);
	if (ayCode.GetSize() > 0)
	{
		std::string strName;
		std::string query;
		pItem->m_data.Query(dwBlockID, strName, query);
		std::string codes;
		ayCode.ToString(codes, _T(' '));
		
		CDuiString strFile;
	
		strFile.Format(_T("%s%s.txt"), global_funciton::GetRootDir(), strName.c_str());
	

		xlf::CTFile file;
		if (file.Open(strFile, xlf::CTFile::mode_create | xlf::CTFile::mode_write))
		{
			file.Write(codes.c_str(), codes.size());
		}
		
	}
}

bool CBlockcalcPageUI::BlockStockRevever(void* pContext, int nErrorCode, const stock_wrapper::StockArray& ayCode)
{
	if ((DWORD)pContext != m_blockAskContext.m_dwTick)
	{
		return false;
	}

	do 
	{
		if (nErrorCode != 0)
		{
			break;
		}
		_block_list_info_t* pItem = nullptr;
		DWORD dwBlockID = GetCurrentSelectBlock(pItem);
		if (pItem == nullptr || dwBlockID == 0)
		{
			break;
		}

		if (dwBlockID != m_blockAskContext.m_dwBlock || m_blockAskContext.m_pList != pItem)
		{
			break;
		}

		stock_wrapper::StockArray aySortCode = ayCode;
		aySortCode.Sort();

		pItem->m_data.UpdateStock(dwBlockID, aySortCode);
		UpdateCodeToList(m_pStockList, aySortCode);

	} while (FALSE);
	

	return false;
}

void CBlockcalcPageUI::OnUpdateAll()
{

}

void CBlockcalcPageUI::OnChangeBlockGroup(int nIndex)
{
	if (nIndex < 0 || nIndex > _block_group_size)
	{
		return;
	}

	if (m_nSelectBlockIndex == nIndex)
	{
		return;
	}

	m_nSelectBlockIndex = nIndex;
	
	if (m_pBlockGroupTab)
	{
		m_pBlockGroupTab->SelectItem(nIndex);
	}
	LoadBlockGroup();
}

void CBlockcalcPageUI::LoadBlockGroup(LPCTSTR lspzFilePath)
{
	int nIndex = m_nSelectBlockIndex;
	if (nIndex < 0 || nIndex > _block_group_size)
	{
		return;
	}

	CListUI* pList = m_blockList[nIndex].m_pList;
	if (nullptr == pList)
	{
		return;
	}

	CDuiString strLoadFilePath;
	if (lspzFilePath == nullptr || lspzFilePath[0] == 0)
	{
		if (!m_blockList[nIndex].m_bLoad)
		{
			m_blockList[nIndex].m_bLoad = true;
			strLoadFilePath = m_blockList[nIndex].m_strFileName;
		}
	}
	else
	{
		strLoadFilePath = lspzFilePath;
	}

	if (!strLoadFilePath.IsEmpty())
	{
		bool bJson = false;
		int nExtIndex = strLoadFilePath.ReverseFind(_T('.'));
		if (nExtIndex != -1)
		{
			CDuiString strExt = strLoadFilePath.Mid(nExtIndex + 1);
			if (strExt.CompareNoCase(_T("json")) == 0)
			{
				bJson = true;
			}
		}
		// 如果是json文件
		if (bJson)
		{
			m_blockList[nIndex].m_data.ImportFromJson(strLoadFilePath);
		}
		else
		{
			m_blockList[nIndex].m_data.Init(strLoadFilePath);
		}
	}

	
	
	// 生产list
	BuildBlockList(pList, m_blockList[nIndex].m_data);
	if (pList->GetCount() > 0)
	{
		pList->SelectItem(0, false);
	}

}

void CBlockcalcPageUI::OnBlockMofify()
{
	_block_list_info_t* pItem = GetCurrentList();
	if (nullptr == pItem)
	{
		return;
	}

	CListUI* pList = pItem->m_pList;
	if (pItem == nullptr)
	{
		return;
	}

	DWORD dwBlockID = 0;
	CControlUI* pControl = nullptr;
	int nSel = pList->GetCurSel();
	if (nSel != -1)
	{
		pControl = pList->GetItemAt(nSel);
		if (pControl)
		{
			dwBlockID = pControl->GetTag();
		}
	}

	if (dwBlockID == 0)
	{
		return;
	}

	CModifyBlockDlg dlg(FALSE);
	std::string name;
	std::string query;
	pItem->m_data.Query(dwBlockID, name, query);
	dlg.SetBlockInfo(name.c_str(), query.c_str());
	if (dlg.CreateDuiWindow(GetParentHwnd(), _T("BlockModify"), UI_WNDSTYLE_DIALOG, 0))
	{
		dlg.CenterWindow();
		if (dlg.ShowModal() == IDOK)
		{
			
			CDuiString strName;
			CDuiString strQuery;
			dlg.GetResult(strName, strQuery);
			if (strName.IsEmpty())
			{
				return;
			}

			pItem->m_data.Modify(dwBlockID, strName, strQuery);
			pControl->SetText(strName);
		}
	}

}

void CBlockcalcPageUI::LoadData()
{
	for (int i = 0; i < _block_group_size; i++)
	{
		if (!m_blockList[i].m_bLoad)
		{
			m_blockList[i].m_bLoad = true;
			m_blockList[i].m_data.Init(m_blockList[i].m_strFileName);
		}
	}
}

void CBlockcalcPageUI::BuildBlockList(CListUI* pList, const stock_wrapper::CBlockList& data)
{
	if (nullptr == pList)
	{
		return;
	}
	pList->SetDelayedDestroy(false);
	pList->RemoveAll();

	int nSize = data.GetSize();
	if (nSize < 0)
	{
		return;
	}

	for (int i = 0; i < nSize; i++)
	{
		stock_wrapper::_block_item_t* pItem = data.GetItem(i);
		if (pItem == nullptr)
		{
			continue;
		}
		
		AddBlockItem(pList, pItem->m_dwBlockID, pItem->m_strName.c_str());
	}

}

void CBlockcalcPageUI::AddBlockItem(CListUI* pList, DWORD dwBlockID, LPCTSTR lpszName)
{
	CListLabelElementUI* pNew = new CBlockItemUI;
	pNew->SetOwner(pList);
	pList->Add(pNew);
	pNew->SetTag(dwBlockID);
	pNew->SetFixedHeight(24);
	pNew->SetText(lpszName);

}

void CBlockcalcPageUI::DeleteBlockItem(CListUI* pList, int nIndex, stock_wrapper::CBlockList& data)
{
	if (nullptr == pList || pList->GetCount() <= 0)
	{
		return;
	}

	CControlUI* pControl = pList->GetItemAt(nIndex);
	if (pControl == nullptr)
	{
		return;
	}
	DWORD dwBlockID = pControl->GetTag();
	pList->RemoveAt(nIndex);
	data.Delete(dwBlockID);
}

void CBlockcalcPageUI::OnNewBlock()
{
	CModifyBlockDlg dlg(TRUE);
	if (dlg.CreateDuiWindow(GetParentHwnd(), _T("BlockModify"), UI_WNDSTYLE_DIALOG, 0))
	{
		dlg.CenterWindow();
		if (dlg.ShowModal() == IDOK)
		{
			CDuiString strName;
			CDuiString strQuery;
			dlg.GetResult(strName, strQuery);
			if (strName.IsEmpty())
			{
				return;
			}

			_block_list_info_t* pList = GetCurrentList();
			if (nullptr == pList)
			{
				return;
			}
			
			DWORD dwBlock = pList->m_data.New(strName, strQuery);
			if (dwBlock != 0 && pList->m_pList)
			{
				AddBlockItem(pList->m_pList, dwBlock, strName);
			}
		}
	}
}

void CBlockcalcPageUI::OnDeleteBlock()
{
	_block_list_info_t* pItem = GetCurrentList();
	if (nullptr == pItem || pItem->m_pList == nullptr)
	{
		return ;
	}

	DWORD dwBlockID = 0;
	int nSelitem = pItem->m_pList->GetCurSel();
	if (nSelitem != -1)
	{
		CControlUI* pControl = pItem->m_pList->GetItemAt(nSelitem);
		if (pControl)
		{
			dwBlockID = pControl->GetTag();
		}

		pItem->m_pList->RemoveAt(nSelitem);
	}

	if (dwBlockID != 0)
	{
		pItem->m_data.Delete(dwBlockID);
	}



}

void CBlockcalcPageUI::OnSave()
{
	_block_list_info_t* pList = GetCurrentList();
	if (nullptr == pList)
	{
		return;
	}
	pList->m_data.SaveToFile(pList->m_strFileName);
}

CBlockcalcPageUI::_block_list_info_t* CBlockcalcPageUI::GetCurrentList()
{
	if (m_nSelectBlockIndex < 0 || m_nSelectBlockIndex >= _block_group_size)
	{
		return nullptr;
	}

	return &m_blockList[m_nSelectBlockIndex];
}

DWORD CBlockcalcPageUI::GetCurrentSelectBlock(_block_list_info_t*& pItem)
{
	pItem = GetCurrentList();
	if (nullptr == pItem || pItem->m_pList == nullptr)
	{
		return 0;
	}

	DWORD dwBlockID = 0;
	int nSelitem = pItem->m_pList->GetCurSel();
	if (nSelitem != -1)
	{
		CControlUI* pControl = pItem->m_pList->GetItemAt(nSelitem);
		if (pControl)
		{
			dwBlockID = pControl->GetTag();
		}
	}

	return dwBlockID;
}

BOOL CBlockcalcPageUI::OnBlockItemSelect(CControlUI* pList)
{
	_block_list_info_t* pItem = GetCurrentList();
	if (nullptr == pItem)
	{
		return FALSE;
	}

	if (pItem->m_pList == nullptr || pItem->m_pList != pList)
	{
		return FALSE;
	}

	DWORD dwBlockID = 0;
	int nSelitem = pItem->m_pList->GetCurSel();
	if (nSelitem != -1)
	{
		CControlUI* pControl = pItem->m_pList->GetItemAt(nSelitem);
		if (pControl)
		{
			dwBlockID = pControl->GetTag();
		}
	}

	if (dwBlockID != 0)
	{
		stock_wrapper::StockArray ayCode;
		pItem->m_data.QueryStock(dwBlockID, ayCode);
		UpdateCodeToList(m_pStockList, ayCode);
	}

	return TRUE;
}

BOOL CBlockcalcPageUI::OnCalcBlockItemChange(CControlUI* pList)
{
	if (nullptr == m_pCalcBlockList || m_pCalcBlockList != pList)
	{
		return FALSE;
	}

	

	DWORD dwBlockID = 0;
	int nSelitem = m_pCalcBlockList->GetCurSel();
	if (nSelitem != -1)
	{
		CControlUI* pControl = m_pCalcBlockList->GetItemAt(nSelitem);
		if (pControl)
		{
			dwBlockID = pControl->GetTag();
		}
	}

	if (dwBlockID != 0)
	{
		m_calcStock.Clear();
		m_calcdata.QueryStock(dwBlockID, m_calcStock);
		UpdateCodeToList(m_pCalcBlockStock, m_calcStock);
	}

	return TRUE;
}

int CBlockcalcPageUI::OnDoubleClickBlockItem(CControlUI* pItem, CListUI* pList)
{
	if (pList == nullptr || pItem == nullptr)
	{
		return FALSE;
	}
	
	int nFind = -1;
	int nCount = pList->GetCount();
	for (int i = 0; i < nCount; i++)
	{
		CControlUI* pChild = pList->GetItemAt(i);
		if (pChild == pItem)
		{
			nFind = i;
			break;
		}
	}

	return nFind;
	
}

void CBlockcalcPageUI::UpdateCodeToList(CListUI* pList, const stock_wrapper::StockArray& ayCode)
{
	if (nullptr == pList)
	{
		return;
	}

	pList->SetDelayedDestroy(false);
	pList->RemoveAll();

	int nSize = ayCode.GetSize();
	for (int i = 0; i < nSize; i++)
	{
		LPCTSTR lspzCode = ayCode.GetAt(i);
		if (lspzCode == nullptr || lspzCode[0] == 0)
		{
			continue;
		}

		CListLabelElementUI* pNew = new CListLabelElementUI;
		pNew->SetOwner(pList);
		pList->Add(pNew);
		pNew->SetFixedHeight(24);
		pNew->SetText(lspzCode);
	}
}

void CBlockcalcPageUI::OnUnion()
{
	m_calcStock.Clear();
	m_calcdata.UnionStock(m_calcStock);

	UpdateCodeToList(m_pCalcBlockStock, m_calcStock);
}

void CBlockcalcPageUI::OnInner()
{
	m_calcStock.Clear();
	m_calcdata.InnerStock(m_calcStock);

	UpdateCodeToList(m_pCalcBlockStock, m_calcStock);
}

void CBlockcalcPageUI::OnSub()
{
 	if (m_calcdata.GetSize() > 1)
 	{
		m_calcStock.Clear();
		stock_wrapper::StockArray ayDst;
		stock_wrapper::_block_item_t* first = m_calcdata.GetItem(0);
		if (first)
		{
			m_calcStock = first->m_ayCode;
		}

		stock_wrapper::_block_item_t* second = m_calcdata.GetItem(1);
		if (second)
		{
			ayDst = second->m_ayCode;
		}
		m_calcStock.Sub(ayDst);

		UpdateCodeToList(m_pCalcBlockStock, m_calcStock);
 	}
 	
}

void CBlockcalcPageUI::OnLoadCalcBlock()
{
	_block_list_info_t* pItem = GetCurrentList();
	if (nullptr == pItem)
	{
		return;
	}

	m_calcdata = pItem->m_data;
	BuildBlockList(m_pCalcBlockList, m_calcdata);
}

void CBlockcalcPageUI::OnDeleteCalcBlock()
{
	if (m_pCalcBlockList)
	{
		DeleteBlockItem(m_pCalcBlockList, m_pCalcBlockList->GetCurSel(), m_calcdata);
	}
}

void CBlockcalcPageUI::OnAddCalcStockTo()
{
	if (m_calcStock.GetSize() == 0)
	{
		return;
	}

	CModifyBlockDlg dlg(TRUE);
	if (dlg.CreateDuiWindow(GetParentHwnd(), _T("BlockModify"), UI_WNDSTYLE_DIALOG, 0))
	{
		dlg.CenterWindow();
		if (dlg.ShowModal() == IDOK)
		{
			CDuiString strName;
			CDuiString strQuery;
			dlg.GetResult(strName, strQuery);
			if (strName.IsEmpty())
			{
				return;
			}



			_block_list_info_t& listItem = m_blockList[2];
			

			DWORD dwBlock = listItem.m_data.New(strName);
			if (dwBlock != 0)
			{
				listItem.m_data.UpdateStock(dwBlock, m_calcStock);
				if (listItem.m_pList)
				{
					AddBlockItem(listItem.m_pList, dwBlock, strName);
				}
				
			}
		}
	}
}

CModifyBlockDlg::CModifyBlockDlg(BOOL bNew)
{
	m_bNew = bNew;
	m_pName = nullptr;
	m_pQuery = nullptr;
	m_pResult = nullptr;
	m_pBlockAsk = nullptr;
}

CModifyBlockDlg::~CModifyBlockDlg()
{
	if (m_pBlockAsk)
	{
		delete m_pBlockAsk;
	}
}

CDuiString CModifyBlockDlg::GetSkinFile()
{
	return _T("xml_block_modify");
}

LPCTSTR CModifyBlockDlg::GetWindowClassName() const
{
	return _T("BlockModifyDlg");
}

void CModifyBlockDlg::SetBlockInfo(LPCTSTR lpszName, LPCTSTR lpszQuery /* = nullptr */)
{
	if (lpszName)
	{
		m_strOriName = lpszName;
	}

	if (lpszQuery)
	{
		m_strQuery = lpszQuery;
	}
}
void CModifyBlockDlg::InitWindow()
{
	m_pName = static_cast<CEditUI*>(m_pm.FindControl(_T("newName")));
	m_pQuery = static_cast<CRichEditUI*>(m_pm.FindControl(_T("askName")));
	m_pResult = static_cast<CRichEditUI*>(m_pm.FindControl(_T("rst")));

	if (m_pName && !m_strOriName.IsEmpty())
	{
		m_pName->SetText(m_strOriName);
	}

	if (m_pQuery && !m_strQuery.IsEmpty())
	{
		m_pQuery->SetText(m_strQuery);

	}

}

void CModifyBlockDlg::Notify(TNotifyUI& msg)
{
	CDuiString name = msg.pSender->GetName();
	if (msg.sType == DUI_MSGTYPE_CLICK)
	{
		if (name == _T("ask"))
		{
			OnSearch();
		}
		else if (name == _T("confirmbtn"))
		{
			OnClickOK();
			Close(IDOK);
			return;
		}
		else if (name == _T("cancelbtn"))
		{
			Close(IDCANCEL);
			return;
		}
	}
	

	WindowImplBase::Notify(msg);
}


LRESULT CModifyBlockDlg::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (uMsg == UWM_BLOCK_ASK_RET)
	{
		OnBlockAskReturn(wParam, lParam);
	}
	return 0;
}

void CModifyBlockDlg::OnSearch()
{
	if (m_pQuery == nullptr)
	{
		return;
	}
	std::string strQuery = m_pQuery->GetText();
	if (strQuery.empty())
	{
		return;
	}

	if (nullptr == m_pBlockAsk)
	{
		m_pBlockAsk = new stock_wrapper::BlockAsker;
		m_pBlockAsk->Init( GetHWND());
	}

	
	m_pBlockAsk->Ask(strQuery.c_str(), 0);

}

static void _FormatCodeShowString(stock_wrapper::StockArray& ayCode, CDuiString& strCodes, int nLineCodes = 7)
{
	int nSize = ayCode.GetSize();
	int nAdd = 0;
	for (int i = 0; i < ayCode.GetSize(); i++)
	{
		LPCTSTR lpszCode = ayCode.GetAt(i);
		if (lpszCode && lpszCode[0] != 0)
		{
			strCodes += lpszCode;
			nAdd++;
			if (nAdd == 7)
			{
				nAdd = 0;
				strCodes += _T("\r\n");
			}
			else
			{
				strCodes += _T(" ");
			}
		}
	}
}

void CModifyBlockDlg::OnBlockAskReturn(WPARAM wParam, LPARAM lParam)
{
	if (wParam == 0 || lParam== 0 || wParam != (WPARAM)m_pBlockAsk)
	{
		return;
	}

	CDuiString strCodes;
	stock_wrapper::BLOCK_ASKER_RET_LPARAM* param = (stock_wrapper::BLOCK_ASKER_RET_LPARAM*)lParam;

	if (param->m_nErrorCode == 0)
	{
		_FormatCodeShowString(param->m_ayCode, strCodes);
	}

	if (m_pResult)
	{
		m_pResult->SetText(strCodes);
	}

	delete param;
	delete m_pBlockAsk;
	m_pBlockAsk = nullptr;
}

void CModifyBlockDlg::GetResult(CDuiString& strName, CDuiString& strAsk)
{
	strName = m_strName;
	strAsk = m_strQuery;
}

void CModifyBlockDlg::OnClickOK()
{
	if (m_pName)
	{
		m_strName = m_pName->GetText();
	}

	if (m_pQuery)
	{
		m_strQuery = m_pQuery->GetText();
	}
}

