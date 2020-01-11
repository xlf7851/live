#include "stdafx.h"
#include "block.h"
#include "../base/file.h"
#include "../base/utl.h"
#include "../base/buffer.h"
#include <json.h>
#include "../base/jsonutl.h"

namespace stock_wrapper
{
	CBlockList::CBlockList()
	{

	}

	CBlockList::CBlockList(const CBlockList& src)
	{
		*this = src;
	}

	CBlockList::~CBlockList()
	{
		Clear();
	}

	CBlockList& CBlockList::operator=(const CBlockList& src)
	{
		Clear();
		for (int i = 0; i < src.m_data.size(); i++)
		{
			_block_item_t* pItem = src.m_data[i];
			if (nullptr == pItem)
			{
				continue;
			}

			_block_item_t* pNew = new _block_item_t;
			*pNew = *pItem;
			m_data.push_back(pNew);
		}

		return *this;
	}

	void CBlockList::Init(LPCTSTR lpszFilePath /* = nullptr */)
	{
		if (lpszFilePath && lpszFilePath[0] != 0)
		{
			m_strFilePath = lpszFilePath;
			Read();
		}
	}

	void CBlockList::SaveToFile(LPCTSTR lpszFilePath)
	{
		std::string strFilePath;
		if (lpszFilePath && lpszFilePath[0] != 0)
		{
			strFilePath = lpszFilePath;
		}
		else
		{
			strFilePath = m_strFilePath;
		}

		if (m_strFilePath.empty())
		{
			return;
		}

		Write(strFilePath);
	}

	void CBlockList::Clear()
	{
		for (int i = 0; i < m_data.size(); i++)
		{
			delete m_data[i];
		}
		m_data.clear();
	}

	
	BOOL CBlockList::GetBlockIDList(std::vector<DWORD>& vcID)
	{
		vcID.clear();
		for (int i = 0; i < m_data.size(); i++)
		{
			if (m_data[i])
			{
				vcID.push_back(m_data[i]->m_dwBlockID);
			}
		}

		return vcID.size() > 0;
	}

	int CBlockList::GetSize() const
	{
		return m_data.size();
	}

	_block_item_t* CBlockList::GetItem(int nIndex) const
	{
		if (nIndex >= 0 && nIndex < m_data.size())
		{
			return m_data[nIndex];
		}

		return nullptr;
	}

#define BLOCK_CACHE_BUF_SIZE 1024*1024

#define BLOCK_FLAG_SIZE 6
	static const char g_szFlag[BLOCK_FLAG_SIZE] = { 'b','l','o','c','k',0 };

	static bool _matchFlag(char* pData)
	{
		for (int i = 0; i < BLOCK_FLAG_SIZE; i++)
		{
			if (g_szFlag[i] != pData[i])
			{
				return false;
			}
		}

		return true;
	}

	static bool _ParseBlockItem(char* data, int nLen, _block_item_t& item, int& nReadLen)
	{
		int nSLen = nLen;
		nReadLen = 0;
		int nSize = sizeof(DWORD);
		if (nLen < nSize)
		{
			nReadLen = nSLen;
			return false;
		}
		nReadLen += nSize;
		item.m_dwBlockID = *((DWORD*)data);
		nLen -= nSize;
		char* p = data + nSize;

		// name
		nSize = sizeof(unsigned char);
		if (nLen < nSize)
		{
			nReadLen = nSLen;
			return false;
		}
		nLen -= nSize;
		nReadLen += nSize;
		int nDataSize = *((unsigned char*)p);
		p += nSize;
		nSize = nDataSize;
		if (nLen < nSize)
		{
			nReadLen = nSLen;
			return false;
		}
		if (nSize > 0)
		{
			TCHAR* name = (TCHAR*)p;
			item.m_strName.assign(name, nSize / sizeof(TCHAR));

			nReadLen += nSize;
			nLen -= nSize;
			p += nSize;
		}
		

		//query
		nSize = sizeof(unsigned short);
		if (nLen < nSize)
		{
			nReadLen = nSLen;
			return false;
		}
		nLen -= nSize;
		nReadLen += nSize;
		nDataSize = *((unsigned short*)p);
		p += nSize;
		nSize = nDataSize;
		if (nLen < nSize)
		{
			nReadLen = nSLen;
			return false;
		}
		if (nSize > 0)
		{
			TCHAR* query = (TCHAR*)p;
			item.m_strQuery.assign(query, nSize / sizeof(TCHAR));

			nReadLen += nSize;
			nLen -= nSize;
			p += nSize;
		}

		//codes
		int nReadCodeLen = item.m_ayCode.ReadFromBuf(p, nLen);
		nReadLen += nReadCodeLen;

		return true;
	}

	void CBlockList::Read()
	{
		if (m_strFilePath.empty())
		{
			return;
		}

		xlf::CTFile file;
		if (!file.Open(m_strFilePath.c_str()))
		{
			return;
		}

		int nLen = file.GetFileLength();
		if (nLen < BLOCK_FLAG_SIZE)
		{
			return;
		}

		char* pData = new char[nLen];
		file.Read(pData, nLen);

		if (!_matchFlag(pData))
		{
			delete pData;
			return;
		}

		nLen -= BLOCK_FLAG_SIZE;
		char* pCur = pData + BLOCK_FLAG_SIZE;
		while (nLen > 0)
		{
			int nReadLen = 0;
			_block_item_t* pNew = new _block_item_t;
			bool bRead = _ParseBlockItem(pCur, nLen, *pNew, nReadLen);
			pCur += nReadLen;
			nLen -= nReadLen;

			if (bRead)
			{
				m_data.push_back(pNew);
			}
			else
			{
				delete pNew;
			}
		}
		
		delete pData;
	}

	void CBlockList::Write(std::string& strFilePath)
	{
		if (strFilePath.empty())
		{
			return;
		}

		if (m_data.size() == 0)
		{
			return;
		}

		xlf::CBuffer buf;
		buf.Reserve(BLOCK_CACHE_BUF_SIZE);

		// 前6个字节是标志
		buf.Append((unsigned char*)g_szFlag, sizeof(char)*6);

		for (int i = 0; i < m_data.size(); i++)
		{
			_block_item_t* pItem = m_data[i];
			if (nullptr == pItem || pItem->m_dwBlockID == 0 || pItem->m_strName.empty())
			{
				continue;
			}

			// 前4个字节是id
			buf.Append((unsigned char*)&pItem->m_dwBlockID, sizeof(DWORD));

			// 名称，用字节表示长度
			unsigned char cLen = pItem->m_strName.size()* sizeof(TCHAR);
			buf.Append((unsigned char*)&cLen, sizeof(unsigned char));
			if (cLen > 0)
			{
				buf.Append((unsigned char*)pItem->m_strName.c_str(), cLen);
			}
			
			// 问句，2个字节表示长度
			unsigned short sLen = pItem->m_strQuery.size() * sizeof(TCHAR);
			buf.Append((unsigned char*)&sLen, sizeof(unsigned short));
			if (sLen > 0)
			{
				buf.Append((unsigned char*)pItem->m_strQuery.c_str(), sLen);
			}
			
			// 代码
			pItem->m_ayCode.WriteToBuf(buf);
		}

		xlf::CTFile file;
		if (!file.Open(strFilePath.c_str(), xlf::CTFile::mode_write | xlf::CTFile::mode_create))
		{
			return;
		}
 		file.Write((void*)buf.GetBuffer(), buf.GetSize());
	}

	_block_item_t* CBlockList::_AddItem(DWORD dwBlockID)
	{
		_block_item_t* pFind = _FindByID(dwBlockID);
		if (pFind == nullptr)
		{
			pFind = new _block_item_t;
			pFind->m_dwBlockID = dwBlockID;
			m_data.push_back(pFind);
		}

		return pFind;
	}

	_block_item_t* CBlockList::_FindByID(DWORD dwBlockID)
	{
		for (int i = 0; i < m_data.size(); i++)
		{
			if (m_data[i] && m_data[i]->m_dwBlockID == dwBlockID)
			{
				return m_data[i];
			}
		}

		return nullptr;
	}

	_block_item_t* CBlockList::_FindByName(LPCTSTR lspzName)
	{
		for (int i = 0; i < m_data.size(); i++)
		{
			if (m_data[i] && m_data[i]->m_strName == lspzName)
			{
				return m_data[i];
			}
		}

		return nullptr;
	}

	static int _FindInArray(DWORD value, std::vector<DWORD>& ay)
	{
		for (int i = 0; i < ay.size(); i++)
		{
			if (value == ay[i])
			{
				return i;
			}
		}

		return -1;
	}

	DWORD CBlockList::_NewID()
	{
		std::vector<DWORD> vcID;
		GetBlockIDList(vcID);

		static DWORD s_firstid = 0x23;
		DWORD dwID = s_firstid;
		do 
		{
			if (_FindInArray(dwID, vcID) < 0)
			{
				s_firstid = dwID + 1;
				break;
			}
			dwID++;
		} while (true);

		return dwID;
	}

	void CBlockList::ImportFromJson(LPCTSTR lspzFilePath)
	{
		xlf::CTFile file;
		if (!file.Open(lspzFilePath, xlf::CTFile::mode_read))
		{
			return;
		}

		int nLen = file.GetFileLength();
		std::string strData;
		strData.reserve(nLen + 1);
		file.Read((void*)strData.c_str(), nLen);
		strData.resize(nLen);
		
		Json::Value jsRoot;
		Json::Reader reader;
		if (!reader.parse(strData, jsRoot) || jsRoot.isNull() || !jsRoot.isArray())
		{
			return;
		}

		string strTmp;
		xlf::CBuffer buf;
		int nSize = jsRoot.size();
		for (int i = 0; i < nSize; i++)
		{
			Json::Value& item = jsRoot[i];
			if (!item.isObject())
			{
				continue;
			}
			// name
			strTmp = json_utl::json_to_string(item["name"]);
			if (strTmp.empty())
			{
				continue;
			}
			buf.Clear();
			if (!xlf::Base64Decode(strTmp.c_str(), strTmp.size(), buf))
			{
				continue;
			}
			strTmp.assign((char*)buf.GetBuffer(), buf.GetSize());
			_block_item_t* pItem = _FindByName(strTmp.c_str());
			if (pItem == nullptr)
			{
				DWORD dwNewID = _NewID();
				pItem = new _block_item_t;
				pItem->m_dwBlockID = dwNewID;
				m_data.push_back(pItem);
			}
			pItem->m_strName = strTmp;

			// query
			strTmp = json_utl::json_to_string(item["query"]);
			if (!strTmp.empty())
			{
				buf.Clear();
				if (xlf::Base64Decode(strTmp.c_str(), strTmp.size(), buf))
				{
					strTmp.assign((char*)buf.GetBuffer(), buf.GetSize());
				}
			}
			pItem->m_strQuery = strTmp;

			// stock
			strTmp = json_utl::json_to_string(item["codes"]);
			pItem->m_ayCode.Clear();
			pItem->m_ayCode.FromString(strTmp.c_str(), strTmp.size(), ',');
		}


	}

	DWORD CBlockList::New(LPCTSTR lpszName, LPCTSTR lpszQury /* = nullptr */)
	{
		DWORD dwBlockID = _NewID();
		if (dwBlockID == 0)
		{
			return dwBlockID;
		}

		_block_item_t* pNew = new _block_item_t;
		m_data.push_back(pNew);
		pNew->m_dwBlockID = dwBlockID;
		pNew->m_strName = lpszName;
		if (lpszQury)
		{
			pNew->m_strQuery = lpszQury;
		}

		return dwBlockID;
	}

	BOOL CBlockList::Modify(DWORD dwBlockID, LPCTSTR lpszName, LPCTSTR lpszQuery)
	{
		_block_item_t* pFind = _FindByID(dwBlockID);
		if (pFind == nullptr)
		{
			return FALSE;
		}

		pFind->m_strName = lpszName;
		if (lpszQuery)
		{
			pFind->m_strQuery = lpszQuery;
		}

		return TRUE;
	}

	BOOL CBlockList::Delete(DWORD dwBlockID)
	{
		for (_block_array_t::iterator it = m_data.begin(); it != m_data.end(); it++)
		{
			_block_item_t* pItem = *it;
			if (pItem && pItem->m_dwBlockID == dwBlockID)
			{
				m_data.erase(it);
				return TRUE;
			}
		}

		return FALSE;

	}

	BOOL CBlockList::Query(DWORD dwBlockID, std::string& strName, std::string& strQuery)
	{
		_block_item_t* pFind = _FindByID(dwBlockID);
		if (pFind == nullptr)
		{
			return FALSE;
		}

		strName = pFind->m_strName;
		strQuery = pFind->m_strQuery;

		return TRUE;
	}

	BOOL CBlockList::QueryStock(DWORD dwBlockID, StockArray& ayStock)
	{
		_block_item_t* pFind = _FindByID(dwBlockID);
		if (pFind)
		{
			ayStock = pFind->m_ayCode;
			return TRUE;
		}

		return FALSE;
	}

	BOOL CBlockList::UpdateStock(DWORD dwBlockID, const StockArray& ayStock)
	{
		_block_item_t* pFind = _FindByID(dwBlockID);
		if (pFind)
		{
			pFind->m_ayCode = ayStock;
			return TRUE;
		}

		return FALSE;
	}

	void CBlockList::UnionStock(StockArray& ayStock) const
	{
		if (m_data.size() == 0)
		{
			return;
		}

		for (int i = 0; i < m_data.size(); i++)
		{
			_block_item_t* pItem = m_data[i];
			if (nullptr == pItem || pItem->m_ayCode.GetSize() == 0)
			{
				continue;
			}

			ayStock.Union(pItem->m_ayCode);
		}
	}

	void CBlockList::InnerStock(StockArray& ayStock) const
	{
		if (m_data.size() == 0)
		{
			return;
		}

		bool bFirst = false;
		for (int i = 0; i < m_data.size(); i++)
		{
			_block_item_t* pItem = m_data[i];
			if (nullptr == pItem || pItem->m_ayCode.GetSize() == 0)
			{
				continue;
			}

			if (!bFirst)
			{
				bFirst = true;
				ayStock = pItem->m_ayCode;
				continue;
			}

			ayStock.Inner(pItem->m_ayCode);
		}
	}

	void CBlockList::SubStock(const CBlockList& ayList, StockArray& ayStock) const
	{
		UnionStock(ayStock);

		StockArray aySrc;
		ayList.UnionStock(aySrc);

		ayStock.Sub(aySrc);

	}
}
