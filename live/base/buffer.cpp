#include "stdafx.h"
#include "buffer.h"

namespace xlf
{
	CBuffer::CBuffer()
	{
		m_data = nullptr;
		m_nSize = 0;
		m_nAlloc = 0;
	}

	CBuffer::~CBuffer()
	{
		if (m_data)
		{
			delete[] m_data;
		}
	}

	void CBuffer::Clear()
	{
		m_nSize = 0;
		m_nAlloc = 0;
		if (m_data)
		{
			delete[] m_data;
			m_data = nullptr;
		}
	}

	unsigned char* CBuffer::GetBuffer() const
	{
		return m_data;
	}

	int CBuffer::GetSize() const
	{
		return m_nSize;
	}

	void CBuffer::Reserve(int size)
	{
		size = (((size + 15) >> 4) << 4);
		Alloc(size);
	}

	void CBuffer::Resize(int size)
	{
		if (size > m_nAlloc)
		{
			Reserve(size);
		}
			
		m_nSize = size;
		if (m_nSize < m_nAlloc)
		{
			m_data[m_nSize] = 0;
		}
		
	}

	void CBuffer::Alloc(int nNewSize)
	{
		if (nNewSize <= m_nAlloc)
		{
			return;
		}

		unsigned char* pNew = new unsigned char[nNewSize];
		if (m_data)
		{
			if (m_nSize > 0)
			{
				memcpy(pNew, m_data, m_nSize);
			}
			delete[] m_data;
		}
		m_data = pNew;
		m_nAlloc = nNewSize;
	}

	unsigned char* CBuffer::Append(const void* pbuf, int nLen)
	{
		int nNewSize = m_nSize + nLen;
		Reserve(nNewSize);
		unsigned char* p = m_data + m_nSize;
		memcpy(m_data + m_nSize, pbuf, nLen);
		m_nSize = nNewSize;

		return p;
	}

	void CBuffer::AppendChar(TCHAR c)
	{
		Append(&c, sizeof(TCHAR));
	}

	void CBuffer::AppendNullChar()
	{
		static TCHAR sz[1] = { 0 };
		Append(sz, sizeof(TCHAR));
	}

	void CBuffer::AppendUInt16(uint16 val)
	{
		Append(&val, sizeof(uint16));
	}

	void CBuffer::AppendUInt32(uint32 val)
	{
		Append(&val, sizeof(uint32));
	}

	void CBuffer::AppendString(const _tstring& str, bool bAppendNull)
	{
		uint32 size = str.size()*sizeof(TCHAR);
		AppendUInt32(bAppendNull ? size + sizeof(TCHAR) : size);
		if (size > 0)
		{
			Append(str.c_str(), size * sizeof(TCHAR));
		}
		
		if (bAppendNull)
		{
			AppendNullChar();
		}
	}

	void CBuffer::AppendString2(const _tstring& str)
	{
		if (str.size() > 0)
		{
			Append(str.c_str(), str.size() * sizeof(TCHAR));
		}

		AppendNullChar();
	}
}