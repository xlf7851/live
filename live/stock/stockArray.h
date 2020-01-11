#pragma once

#include "../base/fmtStringArray.h"
#include "../base/buffer.h"

#define STOCK_CODE_LEN	8
namespace stock_wrapper {

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
			return m_szCode;
		}

		bool Empty()
		{
			return m_szCode[0] == 0;
		}
	};

	class StockArray : public xlf::FmtStringArray
	{
	public:
		StockArray();

		void FromString(LPCTSTR lpszCodes, int nLen, TCHAR ch);
		void ToString(std::string& strCodes, char c = ',');
		void WriteToBuf(xlf::CBuffer& buf);
		int ReadFromBuf(const char* data, int nLen);

		void Sort();
		void Union(const StockArray& src);
		void Sub(const StockArray& src);
		void Inner(const StockArray& src);
	};

	
}