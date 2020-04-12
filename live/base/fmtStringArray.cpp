#include "stdafx.h"
#include "fmtStringArray.h"

namespace xlf {

	FmtStringArray::FmtStringArray(int nElemSize /* = 8 */, int nAllocCount /* = 128 */)
		:m_nElemSize(nElemSize), m_nAddCount(nAllocCount), m_data(NULL), m_nAlloc(0), m_nSize(0)
	{

	}

	FmtStringArray::FmtStringArray(const FmtStringArray& src)
	{
		*this = src;
	}

	FmtStringArray::~FmtStringArray()
	{
		if (m_data)
		{
			delete[] m_data;
			m_data = nullptr;
		}
	}

	LPCTSTR FmtStringArray::Add(LPCTSTR lpszText, int nLen /* = -1 */)
	{
		LPCTSTR lpszData = nullptr;
		CheckAlloc();

		TCHAR* pLast = GetData(-1);
		
		if (lpszText == NULL || lpszText[0] == 0)
		{
			nLen = 0;
		}
		else if (nLen < 0)
		{
			nLen = _tcslen(lpszText);
			if (nLen >= m_nElemSize)
			{
				nLen = m_nElemSize - 1;
			}
		}

		if (nLen > 0)
		{
			memcpy(pLast, lpszText, sizeof(TCHAR) * nLen);
			pLast[nLen] = 0;
		}
		m_nSize++;

		return pLast;
	}

	LPCTSTR FmtStringArray::GetAt(int nIndex) const
	{
		if (nIndex < 0 || nIndex >= m_nSize)
		{
			return NULL;
		}

		return GetData(nIndex);
	}

	void FmtStringArray::RemoveAt(int index)
	{
		if (index < 0 || index >= m_nSize)
		{
			return;
		}

		if (m_nSize == index+1)
		{
			m_nSize--;
			return;
		}
		LPCTSTR p = GetAt(index);
		LPCTSTR d = GetAt(index + 1);
		int len = m_nSize - index - 1;
		memmove((void*)p, d, len*m_nElemSize);
		m_nSize--;
	}

	int FmtStringArray::GetSize() const
	{
		return m_nSize;
	}

	void FmtStringArray::Clear()
	{
		if (m_data)
		{
			delete[] m_data;
			m_data = NULL;
		}

		m_nSize = 0;
		m_nAlloc = 0;
	}

	bool FmtStringArray::Empty()
	{
		return m_nSize <= 0;
	}

	void FmtStringArray::CheckAlloc(int nAddElemCount /* = 1 */)
	{
		int nNewSize = m_nSize + nAddElemCount;

		if (nNewSize > m_nAlloc)
		{
			nNewSize += m_nAddCount;
			Alloc(nNewSize);
		}
	}


	TCHAR* FmtStringArray::GetData(int nSize /* = -1 */) const
	{
		if (nSize == -1)
		{
			nSize = m_nSize;
		}

		if (m_data)
		{
			return m_data + nSize*m_nElemSize;
		}

		return NULL;
	}

	void FmtStringArray::SetCapacity(int nSize)
	{
		if (nSize > m_nAlloc)
		{
			int nNew = nSize+1;
			Alloc(nNew);
		}
	}

	void FmtStringArray::Alloc(int nSize)
	{
		TCHAR* pNew = new TCHAR[nSize*m_nElemSize];

		if (m_data)
		{
			if (m_nSize > 0)
			{
				memcpy(pNew, m_data, sizeof(TCHAR)* m_nSize * m_nElemSize);
			}

			delete m_data;
		}
		m_data = pNew;
		m_nAlloc = nSize;
	}

	int FmtStringArray::GetDataBufferSize()
	{
		return m_nSize * m_nElemSize * sizeof(TCHAR);
	}

	TCHAR* FmtStringArray::GetDataBuffer()
	{
		return m_data;
	}

	FmtStringArray& FmtStringArray::operator =(const FmtStringArray& src)
	{
		Clone(src);
		return *this;
	}

	void FmtStringArray::Clone(const FmtStringArray& src)
	{
		Clear();
		m_nElemSize = src.m_nElemSize;
		m_nAddCount = src.m_nAddCount;
		if (src.m_data && src.m_nSize > 0 && m_nElemSize > 0)
		{
			int nAlloc = src.m_nSize;
			nAlloc = (nAlloc + m_nAddCount - 1) / m_nAddCount * m_nAddCount;

			Alloc(nAlloc);
			m_nSize = src.m_nSize;
			memcpy(m_data, src.m_data, sizeof(TCHAR) * m_nSize * m_nElemSize);
		}
	}

	void FmtStringArray::WriteToBuffer(xlf::CBuffer& buf)
	{
		if (m_nElemSize <= 0)
		{
			return;
		}
		// 每个元素的字符串长度
		uint16 val = m_nElemSize;
		buf.AppendUInt16(val);

		// 数组的长度
		val = m_nSize;
		buf.AppendUInt16(val);

		if (val > 0)
		{
			TCHAR* pBuf = GetDataBuffer();
			buf.Append((unsigned char*)pBuf, GetDataBufferSize());
		}
	}

	bool FmtStringArray::ReadFromBuffer(const char* &data, int nLen)
	{
		if (m_nElemSize <= 0)
		{
			return false;
		}

		Clear();

		uint16 elemSize = 0;
		if (!ReadUInt16FromBuffer(data, nLen, elemSize))
		{
			return false;
		}

		if (elemSize != m_nElemSize)
		{
			return false;
		}

		uint16 nSize = 0;
		if (!ReadUInt16FromBuffer(data, nLen, nSize))
		{
			return false;
		}

		int nNeed = nSize * m_nElemSize * sizeof(TCHAR);
		if (nNeed > nLen)
		{
			return false;
		}

		// copy
		int nAllocSize = (nSize + 7) / 8 * 8;
		Alloc(nAllocSize);
		memcpy(m_data, data, nNeed);
		m_nSize = nSize;

		return true;

	}

}