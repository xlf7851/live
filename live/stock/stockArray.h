#pragma once

#include "../base/fmtStringArray.h"
#include "../base/buffer.h"

#define STOCK_CODE_LEN	16
namespace stock_wrapper {

	// ÊÐ³¡×ª»»
	_tstring MarketToString(uint32 uMarket);
	uint32 StringToMarket(LPCTSTR lpszMarket);

#pragma pack(1)
	struct Stock
	{
		TCHAR m_szCode[STOCK_CODE_LEN];
		Stock()
		{
			m_szCode[0] = 0;
		}

		Stock(const Stock& src)
		{
			*this = src;
		}

		Stock(LPCTSTR src, int nLen = -1)
		{
			Assign(src, nLen);
		}

		Stock& operator=(LPCTSTR src)
		{
			Assign(src, -1);
			return *this;
		}

		Stock& Assign(LPCTSTR src, int nLen = -1)
		{
			m_szCode[0] = 0;
			if (src)
			{
				if (nLen >= STOCK_CODE_LEN)
				{
					nLen = STOCK_CODE_LEN - 1;
				}
				int i = 0;
				while (nLen == -1 || i < nLen)
				{
					if (src[i] == 0)
					{
						break;
					}
					m_szCode[i] = src[i];
					i++;
				}
				m_szCode[i] = 0;
			}

			return *this;
		}

		Stock& operator=(const Stock& src)
		{
			memcpy(m_szCode, src.m_szCode, sizeof(m_szCode));

			return *this;
		}

		int Compare(const Stock& src) const
		{
			return _tcscmp(m_szCode, src.m_szCode);
		}

		bool operator==(const Stock& src) const
		{
			return Compare(src) == 0;
		}

		bool operator>(const Stock& src) const
		{
			return Compare(src) > 0;
		}

		bool operator<(const Stock& src) const
		{
			return Compare(src) < 0;
		}

		std::string ToString() const
		{
			return std::string(m_szCode);
		}

		LPCTSTR ToCString() const
		{
			return m_szCode;
		}

		bool Empty() const
		{
			return m_szCode[0] == 0;
		}

		void Clear()
		{
			memset(this, 0, sizeof(Stock));
		}

		operator LPCTSTR() const
		{
			return m_szCode;
		}

	protected:


	};


	class StockCode
	{
	public:
		StockCode()
		{
			memset(this, 0, sizeof(StockCode));
		}

		StockCode(const StockCode& src)
		{
			*this = src;
		}


		StockCode& operator=(const StockCode& src)
		{
			memcpy(this, &src, sizeof(StockCode));

			return *this;
		}


		StockCode& Assign(uint32 uMarket, const Stock& stock)
		{
			m_uMarket = uMarket;
			m_stock = stock;

			return *this;
		}


		int Compare(const StockCode& src) const
		{
			if (m_uMarket == src.m_uMarket)
			{
				return m_stock.Compare(src.m_stock);
			}
			else
			{
				return m_uMarket - src.m_uMarket;
			}
		}

		bool operator==(const StockCode& src) const
		{
			return Compare(src) == 0;
		}

		bool operator>(const StockCode& src) const
		{
			return Compare(src) > 0;
		}

		bool operator<(const StockCode& src) const
		{
			return Compare(src) < 0;
		}

		uint32 GetMarket() const
		{
			return m_uMarket;
		}

		const Stock& GetStock() const
		{
			return m_stock;
		}

		Stock& GetStock()
		{
			return m_stock;
		}

		void Clear()
		{
			memset(this, 0, sizeof(StockCode));
		}

		bool Empty() const
		{
			return m_stock.Empty();
		}

		bool FromString(LPCTSTR str);
		_tstring ToString();
	protected:
		const void* GetPtr() const{ return this; }
		friend class StockArray;
	private:
		uint32 m_uMarket;
		Stock m_stock;

	};
#pragma pack()

	class StockArray : protected xlf::FmtStringArray
	{
	public:
		StockArray();
		StockArray(const StockArray& src);

		StockArray& operator=(const StockArray& src);

		void FromString(LPCTSTR lpszCodes, int nLen, TCHAR ch = _T(','));
		void ToString(_tstring& strCodes, TCHAR c = _T(','));
		void WriteToBuffer(xlf::CBuffer& buf);
		bool ReadFromBuffer(const char* &data, int nLen);

		void Sort();
		void Union(const StockArray& src);
		void Sub(const StockArray& src);
		void Inner(const StockArray& src);

		int GetStockCodeSize() const
		{
			return GetSize();
		}
		void ClearStockCode()
		{
			Clear();
		}
		bool EmptyStockCode()
		{
			Empty();
		}

		void SetStockCodeCapacity(int nSize)
		{
			SetCapacity(nSize);
		}

		StockCode* AddStockCode(const StockCode& stockCode);
		StockCode* GetStockCodeAt(int index);
		const StockCode* GetStockCodeAt(int index) const;
		int FindStockCodeIndex(const StockCode& stockCode) const;
		int RemoveStockCode(const StockCode& stockCode);

		bool IsSorted() const;

	protected:
		int _FindStockCodeByOrder(const StockCode& stockCode) const;
		int _FindStockCodeBySort(const StockCode& stockCode) const;


		bool m_bSortEd;
	};

	
	
}