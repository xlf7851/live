#include "stdafx.h"
#include "stockArray.h"

namespace stock_wrapper
{
	StockArray::StockArray()
		:xlf::FmtStringArray(STOCK_CODE_LEN, 64)
	{
	}

	void StockArray::FromString(LPCTSTR lpszText, int nLen, TCHAR ch)
	{
		if (lpszText == nullptr || lpszText[0] == 0)
		{
			return;
		}

		int nOffset = 0;
		int nBegin = nOffset;
		while (nOffset < nLen)
		{
			if (ch == lpszText[nOffset] || nOffset == nLen - 1)
			{
				if (nOffset > nBegin)
				{
					Add(lpszText + nBegin, nOffset - nBegin);
				}

				nBegin = nOffset + 1;
			}

			nOffset++;
		}

	}

	void StockArray::ToString(std::string& strCodes, char c)
	{
		char sz[2] = { 0 };
		sz[0] = c;
		int nSize = GetSize();
		for (int i = 0; i < nSize; i++)
		{
			strCodes += GetAt(i);
			strCodes += sz;
		}
	}

	void StockArray::WriteToBuf(xlf::CBuffer& buf)
	{
		// 前2个字节表示字节数
		unsigned short sLen = GetDataBufferSize();
		buf.Append((unsigned char*)&sLen, sizeof(unsigned short));
		if (sLen > 0)
		{
			TCHAR* pBuf = GetDataBuffer();
			buf.Append((unsigned char*)pBuf, sLen);
		}
	}

	int StockArray::ReadFromBuf(const char* data, int nLen)
	{
		Clear();
		if (data == nullptr || nLen < sizeof(unsigned short))
		{
			return 0;
		}

		int nReadLen = sizeof(unsigned short);
		unsigned short slen = *((unsigned short*)data);
		if (slen == 0)
		{
			return nReadLen;
		}

		if (nLen - nReadLen < slen) // 字节不够，数据错误
		{
			return nLen;
		}

		nReadLen += slen;
		// 检查是否是整数倍
		int nElemBufferSize = m_nElemSize * sizeof(TCHAR);
		if (slen % nElemBufferSize != 0)
		{
			return nReadLen;
		}

		// copy
		int nSize = slen / nElemBufferSize;
		int nAllocSize = (nSize + 7) / 8 * 8;
		Alloc(nAllocSize);
		memcpy(m_data, data + sizeof(unsigned short), slen);
		m_nSize = nSize;

		return nReadLen;
		
	}

	int _Compare(const void* src,const void* dst)
	{
		Stock* f = (Stock*)src;
		Stock* s = (Stock*)dst;
		return f->Compare(*s);
	}

	void StockArray::Sort()
	{
		if (m_data == nullptr || m_nSize <= 0 )
		{
			return;
		}

		//_code_struct_t* pStart = (_code_struct_t*)m_data;
		//_code_struct_t* pEnd = (_code_struct_t*)GetData();
		std::qsort(m_data, m_nSize, sizeof(TCHAR) * m_nElemSize, _Compare);
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
}


