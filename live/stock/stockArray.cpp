#include "stdafx.h"
#include "stockArray.h"

namespace stock_wrapper
{
#define NEW_MARKET_FLAG (_T('U'))
	_tstring MarketToString(uint32 uMarket)
	{
		TCHAR sz[5] = { 0 };
		char* p = (char*)&uMarket;
		if (*p == NEW_MARKET_FLAG)
		{
			for (int i = 0; i < 4; i++)
			{
				sz[i] = p[i];
			}
		}
		else if (uMarket <= 0xff)
		{
			_stprintf_s(sz, _T("%d"), uMarket);
		}
		return _tstring(sz);
	}

	uint32 StringToMarket(LPCTSTR lpszMarket)
	{
		uint32 uMarket = 0;
		char szMarket[4] = { 0 };
		if (lpszMarket && lpszMarket[0] != 0)
		{
			if (lpszMarket[0] == NEW_MARKET_FLAG)
			{
				szMarket[0] = lpszMarket[0];
				int i = 1;
				while (lpszMarket[i] != 0 && i < 4)
				{
					szMarket[i] = lpszMarket[i];
				}

				if (i == 4)
				{
					uMarket = *((uint32*)szMarket);
				}
			}
			else
			{
				long lRet = _ttol(lpszMarket);
				if (lRet < 0 && lRet > -128)
				{
					uMarket = (BYTE)(char)lRet;
				}
				else if (lRet > 0 && lRet <= 0xff)
				{
					uMarket = lRet;
				}
			}
		}

		return uMarket;
	}

	bool StockCode::FromString(LPCTSTR str)
	{
		Clear();
		if (str == NULL || str[0] == 0)
		{
			return false;
		}

		int i = 0;
		while (str[i] != 0 && str[i] != _T(':'))
		{
			i++;
		}

		if (str[i] == 0) // 没有市场
		{
			m_stock.Assign(str);
		}
		else if (i > 0 && str[i+1] != 0)
		{
			_tstring strMarket(str,i);
			m_uMarket = StringToMarket(strMarket.c_str());
			if (m_uMarket != 0)
			{
				m_stock.Assign(str + i + 1);
			}
		}

		return !Empty();

	}

	_tstring StockCode::ToString()
	{
		_tstring str;
		if (!m_stock.Empty())
		{
			if (m_uMarket != 0)
			{
				str = MarketToString(m_uMarket);
			}

			str += (LPCTSTR)m_stock;
		}

		return str;
	}



	StockArray::StockArray()
		:xlf::FmtStringArray(sizeof(StockCode), 64)
	{
		m_bSortEd = false;
	}

	StockArray::StockArray(const StockArray& src)
		: xlf::FmtStringArray(src)
	{
		m_bSortEd = src.m_bSortEd;
	}

	StockArray& StockArray::operator =(const StockArray& src)
	{
		Clone(src);
		m_bSortEd = src.m_bSortEd;
		return *this;
	}


	void StockArray::FromString(LPCTSTR lpszText, int nLen, TCHAR ch)
	{
		if (lpszText == nullptr || lpszText[0] == 0)
		{
			return;
		}

		_tstring str(lpszText, nLen);
		TCHAR* pStr = (TCHAR*)str.c_str();

		int i = 0;
		while (pStr[i] != 0)
		{
			int begin = i;
			while (pStr[i] != ch && pStr[i] != 0)
			{
				i++;
			}
			bool bOver = (pStr[i] == 0);
			int end = i;
			if (end - begin > 0)
			{
				pStr[i] = 0;

				StockCode stockCode;
				if (stockCode.FromString(pStr+begin))
				{
					AddStockCode(stockCode);
				}
			}

			if (bOver)
			{
				break;
			}

			i++;

			
		}

	}

	void StockArray::ToString(_tstring& strCodes, TCHAR c)
	{
		TCHAR sz[2] = { 0 };
		sz[0] = c;
		int nSize = GetSize();
		_tstring str;
		for (int i = 0; i < nSize; i++)
		{
			StockCode& p = GetStockCodeAt(i);
			str = p.ToString();
			if (!str.empty())
			{
				strCodes += str;
				strCodes += sz;
			}
		}
	}

	void StockArray::WriteToBuffer(xlf::CBuffer& buf)
	{
		return xlf::FmtStringArray::WriteToBuffer(buf);
	}

	bool StockArray::ReadFromBuffer(const char* &data, int &nLen)
	{
		return xlf::FmtStringArray::ReadFromBuffer(data, nLen);
	}

	int _Compare(const void* src,const void* dst)
	{
		StockCode* f = (StockCode*)src;
		StockCode* s = (StockCode*)dst;
		return f->Compare(*s);
	}

	void StockArray::Sort()
	{
		if (m_data == nullptr || m_nSize <= 0 || IsSorted())
		{
			return;
		}

		std::qsort(m_data, m_nSize, sizeof(TCHAR) * m_nElemSize, _Compare);
	}

	bool StockArray::IsSorted() const
	{
		return m_bSortEd;
	}
	void StockArray::Union(const StockArray& src)
	{
		int nSize = src.GetSize();
		int nThisSize = GetSize();

		if (nSize == 0)
		{
			return;
		}
		else if (nThisSize == 0)
		{
			*this = src;
			return;
		}

		int nElemSize = sizeof(TCHAR) * m_nElemSize;

		StockArray ayTmp;
		int i = 0;
		int j = 0;
		int nRet = 0;
		while (i < nSize && j < nThisSize)
		{
			while (i < nSize && j < nThisSize && (nRet = _Compare(src.m_data + i * nElemSize, m_data + j * nElemSize)) <= 0)
			{
				if (nRet == 0)
				{
					j++;
				}
				ayTmp.Add(src.m_data + i * nElemSize);
				i++;
			}

			while (i < nSize && j < nThisSize && (nRet = _Compare(m_data + j * nElemSize, src.m_data + i * nElemSize)) <= 0)
			{
				if (nRet == 0)
				{
					i++;
				}
				ayTmp.Add(m_data + j * nElemSize);
				j++;
			}
		}

		while (i < nSize )
		{
			ayTmp.Add(src.m_data + i * nElemSize);
			i++;
		}

		while (j < nThisSize)
		{
			ayTmp.Add(m_data + j * nElemSize);
			j++;
		}

		*this = ayTmp;
	}

	void StockArray::Sub(const StockArray& src)
	{
		int nSize = src.GetSize();
		int nThisSize = GetSize();

		if (nSize == 0)
		{
			return;
		}
		else if (nThisSize == 0)
		{
			Clear();
			return;
		}

		int nElemSize = sizeof(TCHAR) * m_nElemSize;

		StockArray ayTmp;
		int i = 0;
		int j = 0;
		int nRet = 0;
		while (i < nSize && j < nThisSize)
		{
			while (i < nSize && j < nThisSize && (nRet = _Compare(src.m_data + i * nElemSize, m_data + j * nElemSize)) <= 0)
			{
				if (nRet == 0)
				{
					j++;
				}
				
				i++;
			}

			while (i < nSize && j < nThisSize && (nRet = _Compare(m_data + j * nElemSize, src.m_data + i * nElemSize)) <= 0)
			{
				if (nRet == 0)
				{
					i++;
				}
				else
				{
					ayTmp.Add(m_data + j * nElemSize);
				}
				
				j++;
			}
		}

		while (j < nThisSize)
		{	
			ayTmp.Add(m_data + j * nElemSize);
			j++;
		}

		*this = ayTmp;
	}

	void StockArray::Inner(const StockArray& src)
	{
		int nSize = src.GetSize();
		int nThisSize = GetSize();

		if (nSize == 0 || nThisSize == 0)
		{
			Clear();
			return;
		}

		int nElemSize = sizeof(TCHAR) * m_nElemSize;

		StockArray ayTmp;
		int i = 0;
		int j = 0;
		int nRet = 0;
		while (i < nSize && j < nThisSize)
		{
			while (i < nSize && j < nThisSize && (nRet = _Compare(src.m_data + i * nElemSize, m_data + j * nElemSize)) <= 0)
			{
				if (nRet == 0)
				{
					ayTmp.Add(src.m_data + i * nElemSize);
					j++;
				}

				i++;
			}

			while (i < nSize && j < nThisSize && (nRet = _Compare(m_data + j * nElemSize, src.m_data + i * nElemSize)) <= 0)
			{
				if (nRet == 0)
				{
					ayTmp.Add(m_data + j * nElemSize);
					i++;
				}

				j++;
			}
		}

		*this = ayTmp;
	}

	StockCode* StockArray::AddStockCode(const StockCode& stockCode)
	{
		LPCTSTR p = Add((LPCTSTR)stockCode.GetPtr(), sizeof(StockCode));
		if (p)
		{
			m_bSortEd = false;
		}
		return (StockCode*)p;
	}

	StockCode& StockArray::GetStockCodeAt(int index)
	{
		LPCTSTR p = GetData(index);
		return *((StockCode*)p);
	}

	const StockCode& StockArray::GetStockCodeAt(int index) const
	{
		LPCTSTR p = GetData(index);
		return *((const StockCode*)p);
	}

	int StockArray::FindStockCodeIndex(const StockCode& stockCode) const
	{
		if (IsSorted())
		{
			return _FindStockCodeBySort(stockCode);
		}
		else
		{
			return _FindStockCodeByOrder(stockCode);
		}
	}

	int StockArray::_FindStockCodeByOrder(const StockCode& stockCode) const
	{
		int size = GetSize();
		for (int i = 0; i < size; i++)
		{
			const StockCode& p = GetStockCodeAt(i);
			if (p.Compare(stockCode) == 0)
			{
				return i;
			}
		}

		return -1;
	}

	int StockArray::_FindStockCodeBySort(const StockCode& stockCode) const
	{
		int size = GetSize();
		if (size == 0)
		{
			return -1;
		}
		int left = 0;
		int right = size - 1;
		while (left <= right)
		{
			int mid = (left + right) / 2;
			const StockCode& pMid = GetStockCodeAt(mid);

			if (stockCode == pMid)
			{
				return mid;
			}
			if (stockCode > pMid)
			{
				left = mid + 1;
			}
			else
			{
				right = mid - 1;
			}
		}
		return -1;
	}

	int StockArray::RemoveStockCode(const StockCode& stockCode)
	{
		int index = IsSorted() ? _FindStockCodeBySort(stockCode) : _FindStockCodeByOrder(stockCode);
		if (index >= 0)
		{
			RemoveAt(index);
		}

		return index;
	}
}


