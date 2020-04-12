#pragma once
#include "stockArray.h"
#include <unordered_map>

struct _block_draw_item_it;

namespace stock_wrapper {
//#define BLOCK_PROPERTY_USE_BUFFER 1

#ifdef BLOCK_PROPERTY_USE_BUFFER
class BlockProperty
{
private:
#pragma pack(1)
	struct _BlockPropertyHeader
	{
		uint8 m_nSize;		// 本结构大小
		uint32 m_blockid;	// 板块id
		uint32 m_param;		// param
		uint8 m_namesize;	// 名称的大小
		uint8 m_querysize;   // 问句大小
		uint16 m_totalsize; // 总的大小
	};
#pragma pack()
#define BlockPropertyHeaderSize (sizeof(_BlockPropertyHeader))
#define BlockPropertyDataMinSize 32

public:
	BlockProperty()
	{
		m_dataBuf = nullptr;
		Init();
	}

	BlockProperty(const BlockProperty& src)
	{
		m_dataBuf = nullptr;
		*this = src;
	}

	BlockProperty& operator=(const BlockProperty& src)
	{
		Clear();
		int nSize = src.GetDataSize();
		Alloc(nSize);
		memcpy(m_dataBuf, src.m_dataBuf, nSize + BlockPropertyHeaderSize);
		return *this;
	}

	virtual ~BlockProperty()
	{
		if (m_dataBuf)
		{
			delete []m_dataBuf;
		}
	}

	void Clear()
	{
		_BlockPropertyHeader* header = GetHeader();
		if (header)
		{
			header->m_blockid = 0;
			header->m_param = 0;
			header->m_namesize = header->m_querysize = 1;
		}
	}
	
	int ReadFromBuffer(const char* buf, int len)
	{
		Clear();
		if (len < BlockPropertyHeaderSize +BlockPropertyDataMinSize)
		{
			return 0;
		}

		_BlockPropertyHeader* pHeader = (_BlockPropertyHeader*)buf;
		if (pHeader->m_nSize != BlockPropertyHeaderSize || pHeader->m_totalsize > len)
		{
			return 0;
		}

		int nReadSize = pHeader->m_totalsize;
		Alloc(nReadSize - BlockPropertyHeaderSize);
		memcpy(m_dataBuf, buf, nReadSize);

		return nReadSize;
	}

	int WriteToBuf(xlf::CBuffer& buf)
	{
		int size = GetHeader()->m_totalsize;
		buf.Append(m_dataBuf, size);
		return size;
	}

	uint32 GetBlockID() const
	{
		return GetHeader()->m_blockid;
	}
	void SetBlockID(uint32 uid)
	{
		GetHeader()->m_blockid = uid;
	}

	uint32 GetBlockParam() const
	{
		return GetHeader()->m_param;
	}
	void  SetBlockParam(uint32 param)
	{
		GetHeader()->m_param = param;
	}

	void GetBlockName(_tstring& strName) const
	{
		strName = GetBlockName();
	}

	LPCTSTR GetBlockName() const
	{
		return (TCHAR*)GetDataPtr();
	}

	void SetBlockName(const _tstring& name)
	{
		_BlockPropertyHeader* header = GetHeader();
		int OldSize = header->m_namesize;
		int nNameSize = (name.size() + 1);
		int nNeed = (nNameSize + header->m_querysize);
		Alloc(nNeed * sizeof(TCHAR));
		
		TCHAR* p = (TCHAR*)GetDataPtr();
		if (nNameSize > OldSize)
		{
			memmove(p + nNameSize, p + OldSize, header->m_querysize * sizeof(TCHAR));
			memcpy(p, name.c_str(), name.size()*sizeof(TCHAR));
			p[nNameSize-1] = 0;
		}
		else
		{
			if (name.size() > 0)
			{
			    memcpy(p, name.c_str(), name.size() * sizeof(TCHAR));
			}
			
			p[nNameSize-1] = 0;

			memmove(p + nNameSize, p + OldSize, header->m_querysize* sizeof(TCHAR));
		}
		header->m_namesize = nNameSize;
	}

	void GetQuery(_tstring& strQuery)const
	{
		strQuery = GetQuery();
	}

	LPCTSTR GetQuery()const
	{
		TCHAR* p = (TCHAR*)GetDataPtr();
		return p + GetHeader()->m_namesize;
	}

	void SetQuery(const _tstring& query)
	{
		_BlockPropertyHeader* header = GetHeader();
		int OldSize = header->m_querysize;
		int nNewSize = (query.size() + 1);
		int nNeed = (nNewSize + header->m_namesize);
		Alloc(nNeed * sizeof(TCHAR));

		TCHAR* p = (TCHAR*)GetDataPtr();
		p += header->m_namesize;

		if (query.size() > 0)
		{
			memcpy(p, query.c_str(), query.size() * sizeof(TCHAR));
		}
		p[nNewSize - 1] = 0;
		
		header->m_querysize = nNewSize;
	}
private:
	_BlockPropertyHeader* GetHeader() const
	{
		return (_BlockPropertyHeader*)m_dataBuf;
	}
	int GetDataSize() const
	{
		_BlockPropertyHeader* header = GetHeader();

		return header ? header->m_totalsize - BlockPropertyHeaderSize : 0;
	}

	char* GetDataPtr() const
	{
		return m_dataBuf ? m_dataBuf + BlockPropertyHeaderSize : nullptr;
	}

	void Alloc(int nAlloc)
	{
		if (nAlloc < BlockPropertyDataMinSize)
		{
			nAlloc = BlockPropertyDataMinSize;
		}

	
		int nOld = GetDataSize();
		if (nAlloc <= nOld)
		{
			return;
		}

		int nNeed = nAlloc + BlockPropertyHeaderSize;
		nNeed = (nNeed + 7) / 8 * 8; 


		char* pBuf = new char[nNeed];
		if (m_dataBuf)
		{
			memcpy(pBuf, m_dataBuf, nOld+BlockPropertyHeaderSize);
			delete[]m_dataBuf;
		}
		else
		{
			_BlockPropertyHeader* header = (_BlockPropertyHeader*)pBuf;
			header->m_nSize = BlockPropertyHeaderSize;
			header->m_blockid = header->m_param = 0;
			header->m_namesize = header->m_querysize = 1;
		}
		m_dataBuf = pBuf;
		GetHeader()->m_totalsize = nNeed;
	}

	void Init()
	{
		Alloc(32);
	}

	
private:
	char* m_dataBuf; 
};

#else
class BlockProperty
{
public:
	BlockProperty()
	{
		m_u32BlockID = 0;
		m_uParam = 0;
	}

	BlockProperty(const BlockProperty& src)
	{
		*this = src;
	}

	BlockProperty& operator=(const BlockProperty& src)
	{
		m_u32BlockID = src.m_u32BlockID;
		m_uParam = src.m_uParam;
		m_strQuery = src.m_strQuery;
		m_strName = src.m_strName;

		return *this;
	}

	virtual ~BlockProperty()
	{
		
	}

	void Clear()
	{
		m_u32BlockID = 0;
		m_uParam = 0;
		m_strName.clear();
		m_strQuery.clear();
	}

	int ReadFromBuffer(const char* buf, int len)
	{
		Clear();
		int nReadLen = xlf::ReadUInt32FromBuffer(buf, len, m_u32BlockID);
		if (nReadLen == 0)
		{
			return 0;
		}
	
		int nRead = xlf::ReadUInt32FromBuffer(buf, len, m_uParam);
		if (nRead == 0)
		{
			return 0;
		}
		nReadLen += nRead;

		// name
		nRead = xlf::ReadStringFromBufferEx(buf, len, m_strName);
		if (nRead == 0)
		{
			return 0;
		}
		nReadLen += nRead;


		//query
		nRead = xlf::ReadStringFromBufferEx(buf, len, m_strQuery);
		if (nRead == 0)
		{
			return 0;
		}
		nReadLen += nRead;
	
		return nReadLen;
	}

	void WriteToBuf(xlf::CBuffer& buf)
	{
		// 前4个字节是id
		buf.AppendUInt32(m_u32BlockID);

		// wapram
		buf.AppendUInt32(m_uParam);

		// 名称
		buf.AppendStringEx(m_strName);

		// 问句
		buf.AppendStringEx(m_strQuery);
	}

	uint32 GetBlockID() const
	{
		return m_u32BlockID;
	}
	void SetBlockID(uint32 uid)
	{
		m_u32BlockID = uid;
	}

	uint32 GetBlockParam() const
	{
		return m_uParam;
	}
	void  SetBlockParam(uint32 param)
	{
		m_uParam = param;
	}

	void GetBlockName(_tstring& strName) const
	{
		strName = m_strName;
	}

	LPCTSTR GetBlockName() const
	{
		return m_strName.c_str();
	}

	void SetBlockName(const _tstring& name)
	{
		m_strName = name;
	}

	void GetQuery(_tstring& strQuery)const
	{
		strQuery = m_strQuery;
	}

	LPCTSTR GetQuery()const
	{
		return m_strQuery.c_str();
	}

	void SetQuery(const _tstring& query)
	{
		m_strQuery = query;
	}
private:
	uint32 m_u32BlockID;
	uint32 m_uParam;
	std::string m_strName;
	std::string m_strQuery;
};

#endif

class BlockCacheItem
{
public:
	BlockCacheItem(){}
	

	void Clear()
	{
		m_property.Clear();
		m_ayCode.ClearStockCode();
	}

	int ReadFromBuffer(const char* buf, int len)
	{
		Clear();
		int nReadLen = m_property.ReadFromBuffer(buf, len);
		if (nReadLen == 0)
		{
			return 0;
		}

		buf += nReadLen;
		len -= nReadLen;
		nReadLen += m_ayCode.ReadFromBuf(buf, len);
		
		return nReadLen;
	}

	void WriteToBuf(xlf::CBuffer& buf)
	{
		if (m_property.GetBlockID() == 0)
		{
			return;
		}

		m_property.WriteToBuf(buf);
		m_ayCode.WriteToBuf(buf);
	}

	uint32 GetBlockID() const
	{
		return m_property.GetBlockID();
	}
	void SetBlockID(uint32 uid)
	{
		m_property.SetBlockID(uid);
	}

	uint32 GetBlockParam() const
	{
		return m_property.GetBlockParam();
	}
	void  SetBlockParam(uint32 param)
	{
		m_property.SetBlockParam(param);
	}

	void GetBlockName(_tstring& strName) const
	{
		m_property.GetBlockName(strName);
	}

	LPCTSTR GetBlockName() const
	{
		return m_property.GetBlockName();
	}

	void SetBlockName(const _tstring& name)
	{
		m_property.SetBlockName(name);
	}

	void GetQuery(_tstring& strQuery)const
	{
		m_property.GetQuery(strQuery);
	}

	LPCTSTR GetQuery()const
	{
		return m_property.GetQuery();
	}

	void SetQuery(const _tstring& query)
	{
		m_property.SetQuery(query);
	}

	void SetStockArray(const StockArray& ayCode)
	{
		m_ayCode = ayCode;
	}

	const StockArray& GetStockArray() const
	{
		return m_ayCode;
	}

	StockArray& GetStockArray()
	{
		return m_ayCode;
	}

	void GetStockArray(StockArray& ayCode) const
	{
		ayCode = m_ayCode;
	}

	int GetStockArraySize() const
	{
		return m_ayCode.GetStockCodeSize();
	}

protected:
	BlockProperty m_property;
	StockArray m_ayCode;
};

class BlockCacheManager
{
public:
	typedef std::unordered_map<uint32, BlockCacheItem*> _data_container_t;
public:
	BlockCacheManager();
	 ~BlockCacheManager();

	static BlockCacheManager* Instance();

	

	void Release();
	void Init(LPCTSTR lpszFilePath = nullptr);
	void Build();

	
	BlockCacheItem* FindBlockItem(uint32 uid);

	uint32 NewBlock(const _tstring& name, const _tstring& query = _tstring(_T("")), uint32 uParam = 0);
	bool ModifyBlock(uint32 dwBlockID, const _tstring& name, const _tstring& query = _tstring(_T("")));
	bool DeleteBlock(uint32 dwBlockID);
	bool QueryBlock(uint32 dwBlockID, _tstring* strName, _tstring* strQuery = nullptr, uint32* pParam = nullptr, StockArray* ayCode = nullptr, int* codesize = nullptr);

	bool QueryStock(uint32 dwBlockID, StockArray& ayStock);
	bool UpdateStock(uint32 dwBlockID, const StockArray& ayStock);

protected:
	int ReadFromBuffer(char* buf, int len);
	int WriteToBuffer(xlf::CBuffer & buf);
	BlockCacheItem* _FindBlockCacheItem(uint32 uid, bool bNew = false);
	BlockCacheItem* _AddBlockCacheItem(uint32 uid);
	uint32 NewBlockID();



protected:
	_data_container_t m_data;
	_tstring m_strFilePath;
};


class BlockGroup
{
public:
	BlockGroup()
	{
		m_uGroupID = 0;
		m_uOption = 0;
	}

	BlockGroup(const BlockGroup& src)
	{
		*this = src;
	}
	

	BlockGroup& operator =(const BlockGroup& src)
	{
		m_uGroupID = src.m_uGroupID;
		m_uOption = src.m_uOption;
		m_strGroupName = src.m_strGroupName;
		m_ayBlockID = src.m_ayBlockID;
	}

	virtual ~BlockGroup()
	{

	}

	int ReadFromBuffer(const char* buf, int len);
	int WriteToBuffer(xlf::CBuffer & buf);

	uint32 GetGroupID() const
	{
		return m_uGroupID;
	}

	void SetGroupID(uint32 uID)
	{
		m_uGroupID = uID;
	}

	uint32 GetGroupOption() const
	{
		return m_uOption;
	}

	void SetGroupOption(uint32 uOption)
	{
		m_uOption = uOption;
	}

	_tstring GetGroupName() const
	{
		return m_strGroupName;
	}

	void SetGroupName(LPCTSTR lpszName)
	{
		m_strGroupName = lpszName;
	}

	void GetBlockArray(BlockIDArray& ayBlock) const
	{
		ayBlock = m_ayBlockID;
	}

	BlockIDArray& GetBlockArray()
	{
		return m_ayBlockID;
	}

	void SetBlockArray(const BlockIDArray& ayBlock)
	{
		m_ayBlockID = ayBlock;
	}

	void AddBlock(uint32 uID)
	{
		if (m_ayBlockID.FindValue(uID) == -1)
		{
			m_ayBlockID.Add(uID);
		}
	}

	void DeleteBlock(uint32 uID)
	{
		m_ayBlockID.RemoveValue(uID);
	}

protected:
	uint32 m_uGroupID;
	uint32 m_uOption;
	_tstring m_strGroupName;
	BlockIDArray m_ayBlockID;
};


	class BlockGroupManager
	{
	public:
		typedef std::vector<BlockGroup*> _dataContainer_t;
		typedef _dataContainer_t::iterator _dataIterator_t;
	public:
		BlockGroupManager();
		~BlockGroupManager();

		void Load(LPCTSTR lpszFilePath = nullptr);
		void Save(LPCTSTR lpszFilePath=nullptr);
		int ReadFromBuffer(const char* buf, int len);
		int WriteToBuffer(xlf::CBuffer & buf);

		static BlockGroupManager* Instance();

		void Init(LPCTSTR lpszFilePath = nullptr);
		void Release();

		void GetAllBlockGroupIDAndName(std::vector<uint32>& vcGroup, std::vector<_tstring>& vcName);
		void GetBlockDrawItemListInfo(uint32 groupid, std::vector<_block_draw_item_it*>& vcDrawInfo);

		uint32 AddGroup(const _tstring& strName, uint32 option = 0);
		bool QueryGroup(uint32 groupid, _tstring* name, BlockIDArray* ayid = nullptr, uint32* option = nullptr);
		void GetBlockArray(uint32 groupid, BlockIDArray& ayBlock);
		void SetBlockArray(uint32 groupid, const BlockIDArray& ayBlock);
		void AddBlock(uint32 groupid, uint32 uID);
		void DeleteBlock(uint32 groupid, uint32 uID);
		
	protected:
		
		void CreateDefault();
		BlockGroup* FindGroup(uint32 groupid, bool bNew = false);
		BlockGroup* NewGroup(uint32 groupid);
		uint32 NewGroupID();

	protected:
		std::string m_strFilePath;
		_dataContainer_t m_data;

	};
}
