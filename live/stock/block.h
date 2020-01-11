#pragma once
#include "stockArray.h"
namespace stock_wrapper
{
	struct _block_item_t
	{
		DWORD m_dwBlockID;
		std::string m_strName;
		std::string m_strQuery;
		StockArray m_ayCode;

		_block_item_t()
		{
			m_dwBlockID = 0;
		}

		_block_item_t& operator=(const _block_item_t& src)
		{
			m_dwBlockID = src.m_dwBlockID;
			m_strName = src.m_strName;
			m_strQuery = src.m_strQuery;
			m_ayCode = src.m_ayCode;

			return *this;
		}
	};
	typedef std::vector<_block_item_t*> _block_array_t;

	class CBlockList
	{
	public:
		CBlockList();
		CBlockList(const CBlockList& src);
		~CBlockList();

		CBlockList& operator =(const CBlockList& src);

		void Init(LPCTSTR lpszFilePath = nullptr);
		BOOL GetBlockIDList(std::vector<DWORD>& vcID);
		void SaveToFile(LPCTSTR lpszFilePath = nullptr);
		void ImportFromJson(LPCTSTR lspzFilePath);
		int GetSize() const;
		_block_item_t* GetItem(int nIndex) const;

		DWORD New(LPCTSTR lpszName, LPCTSTR lpszQury = nullptr);
		BOOL Modify(DWORD dwBlockID, LPCTSTR lpszName, LPCTSTR lpszQuery = nullptr);
		BOOL Delete(DWORD dwBlockID);
		BOOL Query(DWORD dwBlockID, std::string& strName, std::string& strQuery);

		BOOL QueryStock(DWORD dwBlockID, StockArray& ayStock);
		BOOL UpdateStock(DWORD dwBlockID, const StockArray& ayStock);

		void UnionStock(StockArray& ayStock) const;
		void InnerStock(StockArray& ayStock) const;
		void SubStock(const CBlockList& ayList, StockArray& ayStock) const;

	protected:
		void Read();
		void Write(std::string& strFilePath);
		void Clear();

	protected:
		_block_item_t* _AddItem(DWORD dwBlockID);
		_block_item_t* _FindByID(DWORD dwBlockID);
		_block_item_t* _FindByName(LPCTSTR lspzName);
		DWORD _NewID();

	protected:
		_block_array_t m_data;
		std::string m_strFilePath;

	};
}
