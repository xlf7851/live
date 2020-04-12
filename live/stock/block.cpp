#include "stdafx.h"
#include "block.h"
#include "../base/file.h"
#include "../base/utl.h"
#include "../base/buffer.h"
#include <json.h>
#include "../base/jsonutl.h"
#include "stockDefine.h"

namespace stock_wrapper
{

BlockCacheManager::BlockCacheManager()
{

}

BlockCacheManager::~BlockCacheManager()
{
	Release();
}

static BlockCacheManager* g_blockCacheManagerInstance = nullptr;
BlockCacheManager* BlockCacheManager::Instance()
{
	if (g_blockCacheManagerInstance == nullptr)
	{
		g_blockCacheManagerInstance = new BlockCacheManager;
	}

	return g_blockCacheManagerInstance;
}

void BlockCacheManager::Release()
{
	for (_data_container_t::iterator it = m_data.begin(); it != m_data.end(); it++)
	{
		delete it->second;
	}
	m_data.clear();
}

void BlockCacheManager::Init(LPCTSTR lpszFilePath /* = nullptr */)
{
	if (lpszFilePath && lpszFilePath[0] != 0)
	{
		m_strFilePath = lpszFilePath;
	}

	if (m_strFilePath.empty())
	{
		m_strFilePath = global_funciton::GetConfigDir();
		m_strFilePath += STOCK_CONFIG_BLOCK_CACHE_PATH;
	}

	Build();
}

void BlockCacheManager::Build()
{
	if (m_strFilePath.empty())
	{
		return;
	}

	Release();

	m_data.rehash(100);

	xlf::CBuffer buf;
	xlf::ReadBufferFromFile(buf, m_strFilePath.c_str());
	if (buf.GetSize() == 0)
	{
		return;
	}

	ReadFromBuffer((char*)buf.GetBuffer(), buf.GetSize());
}


int BlockCacheManager::ReadFromBuffer(char* buf, int len)
{
	int nHeadSize = sizeof(_xlf_common_binary_file_header_t);
	if (len < nHeadSize)
	{
		return 0;
	}
	_xlf_common_binary_file_header_t* header = (_xlf_common_binary_file_header_t*)buf;
	
	if (xlf::IsErrorXlfBinaryFileHeader(header, GET_XLF_BINARY_FILE_HEAD_FLAG(g_szBlockCacheFileFlag), len))
	{
		return 0;
	}

	len -= nHeadSize;
	buf += nHeadSize;

	int nTotal = nHeadSize;
	
	while (len > 0)
	{
		BlockCacheItem* pNewItem = new BlockCacheItem;
		int nRead = pNewItem->ReadFromBuffer(buf, len);
		if (nRead == 0)
		{
			delete pNewItem;
			break;
		}

		m_data[pNewItem->GetBlockID()] = pNewItem;

		nTotal += nRead;
	}

	return nTotal;
}

int BlockCacheManager::WriteToBuffer(xlf::CBuffer & buf)
{
	_xlf_common_binary_file_header_t header;
	xlf::InitXlfBinaryFileHeader(&header, GET_XLF_BINARY_FILE_HEAD_FLAG(g_szBlockCacheFileFlag));
	_xlf_common_binary_file_header_t* pHeaderInBuf = (_xlf_common_binary_file_header_t*)buf.Append(&header, sizeof(_xlf_common_binary_file_header_t));
	int nOldSize = buf.GetSize();
	for (_data_container_t::iterator it = m_data.begin(); it != m_data.end(); it++)
	{
		BlockCacheItem* pItem = it->second;
		if (pItem == nullptr || pItem->GetBlockID() == 0)
		{
			continue;
		}

		pItem->WriteToBuf(buf);
	}

	pHeaderInBuf->m_ntotal += buf.GetSize() - nOldSize;

	return pHeaderInBuf->m_ntotal;
}

uint32 BlockCacheManager::NewBlockID()
{
	static uint32 s_firstid = BLOCK_ID_SELF_BEGIN;
	uint32 dwID = s_firstid;
	bool bFirst = true;
	do
	{
		if (dwID >= BLOCK_ID_SELF_END)
		{
			if (!bFirst)
			{
				dwID = 0;
				break;
			}
			dwID = BLOCK_ID_SELF_BEGIN;
			bFirst = false;
		}

		if (FindBlockItem(dwID) == nullptr)
		{
			s_firstid = dwID + 1;
			break;
		}
		dwID++;
	} while (true);

	return dwID;
}

BlockCacheItem* BlockCacheManager::FindBlockItem(uint32 uid)
{
	return _FindBlockCacheItem(uid, false);
}

BlockCacheItem* BlockCacheManager::_FindBlockCacheItem(uint32 uid, bool bNew)
{
	BlockCacheItem* pFind = nullptr;
	_data_container_t::iterator it = m_data.find(uid);
	if (it != m_data.end())
	{
		pFind = it->second;
	}

	if (pFind == nullptr && bNew)
	{
		pFind = _AddBlockCacheItem(uid);
	}

	return pFind;
}

BlockCacheItem* BlockCacheManager::_AddBlockCacheItem(uint32 uid)
{
	BlockCacheItem* pFind = new BlockCacheItem;
	pFind->SetBlockID(uid);
	m_data[uid] = pFind;
	return pFind;
}

uint32 BlockCacheManager::NewBlock(const _tstring& name, const _tstring& query, uint32 uParam)
{
	uint32 u32BlockID = NewBlockID();
	if (u32BlockID == 0)
	{
		return u32BlockID;
	}

	BlockCacheItem* pNew = _AddBlockCacheItem(u32BlockID);
	pNew->SetBlockName(name);
	if (!query.empty())
	{
		pNew->SetQuery(query);
	}
	pNew->SetBlockParam(uParam);
	
	return u32BlockID;
}

bool BlockCacheManager::ModifyBlock(uint32 dwBlockID, const _tstring& name, const _tstring& query)
{
	BlockCacheItem* pFind = _FindBlockCacheItem(dwBlockID);
	if (pFind == nullptr)
	{
		return false;
	}

	if (!name.empty())
	{
		pFind->SetBlockName(name);
	}
	
	if (!query.empty())
	{
		pFind->SetQuery(query);
	}

	return true;
}

bool BlockCacheManager::DeleteBlock(uint32 dwBlockID)
{
	_data_container_t::iterator it = m_data.find(dwBlockID);
	if (it != m_data.end())
	{
		m_data.erase(it);

		return true;
	}
	return false;
}

bool BlockCacheManager::QueryBlock(uint32 dwBlockID, _tstring* strName, _tstring* strQuery, uint32* pParam, StockArray* ayCode, int* codesize)
{
	BlockCacheItem* pFind = _FindBlockCacheItem(dwBlockID);
	if (pFind)
	{
		if (strName)
		{
			pFind->GetBlockName(*strName);
		}
		
		if (strQuery)
		{
			pFind->GetQuery(*strQuery);
		}
		
		if (pParam)
		{
			*pParam = pFind->GetBlockParam();
		}

		if (ayCode)
		{
			pFind->GetStockArray(*ayCode);
		}

		if (codesize)
		{
			*codesize = pFind->GetStockArraySize();
		}

		return true;
	}

	return false;
}

bool BlockCacheManager::QueryStock(uint32 dwBlockID, stock_wrapper::StockArray& ayStock)
{
	BlockCacheItem* pFind = _FindBlockCacheItem(dwBlockID);
	if (pFind)
	{
		pFind->GetStockArray(ayStock);
		return true;
	}

	return false;
}

bool BlockCacheManager::UpdateStock(uint32 dwBlockID, const StockArray& ayStock)
{
	BlockCacheItem* pFind = _FindBlockCacheItem(dwBlockID);
	if (pFind)
	{
		pFind->SetStockArray(ayStock);
		return TRUE;
	}

	return FALSE;
}



	int BlockGroup::ReadFromBuffer(const char* buf, int len)
	{
		int nTotal = 0;
		int nRead = xlf::ReadUInt32FromBuffer(buf, len, m_uGroupID);
		if (nRead == 0)
		{
			return 0;
		}
		nTotal += nRead;
		nRead = xlf::ReadUInt32FromBuffer(buf, len, m_uOption);
		if (nRead == 0)
		{
			return 0;
		}
		nTotal += nRead;
		nRead = m_ayBlockID.ReadFromBuf(buf, len);
		if (nRead == 0)
		{
			return 0;
		}

		nTotal += nRead;

		return nTotal;
	}

	int BlockGroup::WriteToBuffer(xlf::CBuffer & buf)
	{
		int old = buf.GetSize();
		buf.AppendUInt32(m_uGroupID);
		buf.AppendUInt32(m_uOption);
		m_ayBlockID.WriteToBuf(buf);

		return buf.GetSize() - old;
	}

	BlockGroupManager::BlockGroupManager()
	{

	}

	BlockGroupManager::~BlockGroupManager()
	{
		Release();
	}

	void BlockGroupManager::Release()
	{
		for (_dataIterator_t it = m_data.begin(); it != m_data.end(); it++)
		{
			delete *it;
		}

		m_data.clear();
	}

	static BlockGroupManager* g_blockGroupManagerInstance = nullptr;
	BlockGroupManager*  BlockGroupManager::Instance()
	{
		if (g_blockGroupManagerInstance == nullptr)
		{
			g_blockGroupManagerInstance = new BlockGroupManager;
			g_blockGroupManagerInstance->Init();
		}

		return g_blockGroupManagerInstance;
	}

	void BlockGroupManager::Init(LPCTSTR lpszFilePath)
	{
		if (lpszFilePath && lpszFilePath[0] != 0)
		{
			m_strFilePath = lpszFilePath;
		}

		if (m_strFilePath.empty())
		{
			m_strFilePath = global_funciton::GetConfigDir();
			m_strFilePath += STOCK_CONFIG_BLOCK_GROUP_PATH;
		}

		xlf::MakeDir(m_strFilePath.c_str());
	
		Load();
	}

	void BlockGroupManager::Load(LPCTSTR lpszFilePath)
	{
		_tstring strPath;
		if (lpszFilePath && lpszFilePath[0] != 0)
		{
			strPath = lpszFilePath;
		}
		else
		{
			strPath = m_strFilePath;
		}

		if (strPath.empty())
		{
			return;
		}

		Release();

		xlf::CBuffer buf;
		xlf::ReadBufferFromFile(buf, strPath.c_str());
		
		ReadFromBuffer((const char*)buf.GetBuffer(), buf.GetSize());

		if (m_data.size() == 0)
		{
			CreateDefault();
		}
	}

	void BlockGroupManager::Save(LPCTSTR lpszFilePath/* =nullptr */)
	{
		_tstring strPath;
		if (lpszFilePath && lpszFilePath[0] != 0)
		{
			strPath = lpszFilePath;
		}
		else
		{
			strPath = m_strFilePath;
		}

		if (strPath.empty())
		{
			return;
		}

		xlf::CBuffer buf;
		WriteToBuffer(buf);

		if (buf.GetSize() > 0)
		{
			xlf::WriteBufferToFile(buf, strPath.c_str());
		}
	}

	int BlockGroupManager::ReadFromBuffer(const char* buf, int len)
	{
		int nHeadSize = sizeof(_xlf_common_binary_file_header_t);
		if (len < nHeadSize)
		{
			return 0;
		}
		_xlf_common_binary_file_header_t* header = (_xlf_common_binary_file_header_t*)buf;

		if (xlf::IsErrorXlfBinaryFileHeader(header, GET_XLF_BINARY_FILE_HEAD_FLAG(g_szBlockGroupFileFlag), len))
		{
			return 0;
		}

		len -= nHeadSize;
		buf += nHeadSize;

		int nTotal = nHeadSize;

		while (len > 0)
		{
			BlockGroup* pNewItem = new BlockGroup;
			int nRead = pNewItem->ReadFromBuffer(buf, len);
			if (nRead == 0)
			{
				delete pNewItem;
				break;
			}

			m_data.push_back(pNewItem);

			nTotal += nRead;
		}

		return nTotal;
	}


	int BlockGroupManager::WriteToBuffer(xlf::CBuffer & buf)
	{
		_xlf_common_binary_file_header_t header;
		xlf::InitXlfBinaryFileHeader(&header, GET_XLF_BINARY_FILE_HEAD_FLAG(g_szBlockGroupFileFlag));
		_xlf_common_binary_file_header_t* pHeaderInBuf = (_xlf_common_binary_file_header_t*)buf.Append(&header, sizeof(_xlf_common_binary_file_header_t));
		int nOldSize = buf.GetSize();
		for (_dataIterator_t it = m_data.begin(); it != m_data.end(); it++)
		{
			BlockGroup *pItem = *it;
			if (pItem == nullptr || pItem->GetGroupID() == 0)
			{
				continue;
			}

			pItem->WriteToBuffer(buf);
		}

		pHeaderInBuf->m_ntotal += buf.GetSize() - nOldSize;

		return pHeaderInBuf->m_ntotal;
	}

	void BlockGroupManager::CreateDefault()
	{
		static uint32 ids[] = { BLOCK_GROUP_ID_SELF, BLOCK_GROUP_ID_STRATEGY };
		static LPCTSTR names[] = { _T("��ѡ���"), _T("���԰��") };
		int nCount = _countof(ids);
		for (int i = 0; i < nCount; i++)
		{
			BlockGroup* pList = NewGroup(ids[i]);
			if (pList)
			{
				pList->SetGroupName(names[i]);
			}
		}
	}

	BlockGroup* BlockGroupManager::NewGroup(uint32 groupid)
	{
		BlockGroup* pList = new BlockGroup;
		if (pList)
		{
			m_data.push_back(pList);
			pList->SetGroupID(groupid);
		}

		return pList;
	}

	uint32 BlockGroupManager::NewGroupID()
	{
		static uint32 s_firstid = BLOCK_GROUP_BEGIN;
		uint32 uGroupid = s_firstid;
		bool bFirst = true;
		do
		{
			if (uGroupid >= BLOCK_GROUP_END)
			{
				if (!bFirst)
				{
					uGroupid = 0;
					break;
				}
				uGroupid = BLOCK_GROUP_BEGIN;
				bFirst = false;
			}

			if (FindGroup(uGroupid, false) == nullptr)
			{
				s_firstid = uGroupid + 1;
				break;
			}
			uGroupid++;
		} while (true);

		return uGroupid;
	}

	BlockGroup* BlockGroupManager::FindGroup(uint32 groupid, bool bNew)
	{
		BlockGroup* pFind = nullptr;
		for (_dataIterator_t it = m_data.begin(); it != m_data.end(); it++)
		{
			BlockGroup* p = *it;
			if (p && p->GetGroupID() == groupid)
			{
				pFind = p;
				break;
			}
		}

		if (pFind == nullptr && bNew)
		{
			pFind = NewGroup(groupid);
		}

		return pFind;
	}

	void BlockGroupManager::GetAllBlockGroupIDAndName(std::vector<uint32>& vcGroup, std::vector<_tstring>& vcName)
	{
		for (_dataIterator_t it = m_data.begin(); it != m_data.end(); it++)
		{
			BlockGroup* pList = *it;
			if (pList)
			{
				vcGroup.push_back(pList->GetGroupID());
				vcName.push_back(pList->GetGroupName());
			}
		}
	}


	void BlockGroupManager::GetBlockDrawItemListInfo(uint32 groupid, std::vector<_block_draw_item_it *>& vcDrawInfo)
	{
		
	}

	uint32 BlockGroupManager::AddGroup(const _tstring& strName, uint32 option)
	{
		uint32 groupid = NewGroupID();
		if (groupid == 0)
		{
			return 0;
		}

		BlockGroup* pNew = NewGroup(groupid);
		if (pNew == nullptr)
		{
			return 0;
		}

		pNew->SetGroupOption(option);
		pNew->SetGroupName(strName.c_str());

		return groupid;
	}

	bool BlockGroupManager::QueryGroup(uint32 groupid, _tstring* name, BlockIDArray* ayid , uint32* option)
	{
		BlockGroup* pGroup = FindGroup(groupid);
		if (pGroup)
		{
			if (name)
			{
				*name = pGroup->GetGroupName();
			}

			if (ayid)
			{
				pGroup->GetBlockArray(*ayid);
			}

			if (option)
			{
				*option = pGroup->GetGroupOption();
			}

			return true;
		}
		return false;
	}
	
	void BlockGroupManager::GetBlockArray(uint32 groupid, BlockIDArray& ayBlock)
	{
		QueryGroup(groupid, nullptr, &ayBlock);
	}

	void BlockGroupManager::SetBlockArray(uint32 groupid, const BlockIDArray& ayBlock)
	{
		BlockGroup* pGroup = FindGroup(groupid);
		if (pGroup)
		{
			pGroup->SetBlockArray(ayBlock);
		}
	}

	void BlockGroupManager::AddBlock(uint32 groupid, uint32 uID)
	{
		BlockGroup* pGroup = FindGroup(groupid);
		if (pGroup)
		{
			pGroup->AddBlock(uID);
		}
	}

	void BlockGroupManager::DeleteBlock(uint32 groupid, uint32 uID)
	{
		BlockGroup* pGroup = FindGroup(groupid);
		if (pGroup)
		{
			pGroup->DeleteBlock(uID);
		}
	}

}
