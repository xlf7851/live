#pragma once
#include "buffer.h"

namespace xlf
{
	// 内部赋值通过memcpy，所以对象不可含有需要深拷贝的指针
	template<class _Type>
	class FmtObjArray
	{
	public:
		FmtObjArray(){}
		FmtObjArray(const FmtObjArray& src)
		{
			*this = src;
		}
		virtual ~FmtObjArray()
		{
			Clear();
		}

		// 运算符重载
	public:
		FmtObjArray& operator=(const FmtObjArray& src)
		{
			Clear();
			if (src.m_data && src.m_nSize > 0)
			{
				int nAlloc = src.m_nSize + 1;
				Alloc(nAlloc);
				m_nSize = src.m_nSize;
				memcpy(m_data, src.m_data, m_nSize * GetElemSize());
			}

			return *this;
		}

	public:
		_Type* Add(const _Type& obj)
		{
			CheckAlloc(1);
			_Type* pData = GetAt(m_nSize);
			memcpy(pData, &obj, GetElemSize());
			m_nSize++;

			return pData;
		}

		_Type* GetAt(int nIndex) const
		{
			return m_data + nIndex;
		}

		int GetSize() const
		{
			return m_nSize;
		}

		_Type* GetData() const
		{
			return m_data;
		}
		

		void Clear()
		{
			if (m_data)
			{
				delete []m_data;
			}

			m_data = nullptr;
			m_nSize = 0;
			m_nAlloc = 0;
		}

		void Resize(int nSize)
		{
			if (nSize >= m_nAlloc)
			{
				Alloc(nSize + 1);
			}
			m_nSize = nSize;
		}

		bool Empty() const
		{
			return m_nSize <= 0 || m_data == nullptr;
		}

		void SetCapacity(int nSize)
		{
			if (nSize > m_nAlloc)
			{
				Alloc(nSize);
			}
		}

		int WriteToBuf(CBuffer& buf)
		{
			int len = m_nSize > 0 && m_data ? m_nSize : 0;
			buf.Append((unsigned char*)&len, sizeof(int));
			if (len > 0)
			{
				len = len * GetElemSize();
				buf.Append((unsigned char*)m_data, len);
			}

			len += sizeof(int);

			return len;
		}

		int ReadFromBuf(const char* buf, int len)
		{
			int nReadLen = 0;
			if (buf == nullptr || len < 2)
			{
				return 0;
			}

			int nDataLen = *((int*)buf);
			nReadLen = sizeof(int);
			len -= nReadLen;
			if (nDataLen * GetElemSize() < len)
			{
				return 0;
			}

			
			if (nDataLen > 0)
			{
				Append((const _Type*)(buf+nReadLen), nDataLen);
			}
			
			nReadLen += nDataLen * GetElemSize();

			return nReadLen;
		}

		void Append(const _Type* obj, int len)
		{
			if (obj == nullptr || len <= 0)
			{
				return;
			}

			CheckAlloc(len);
			_Type* pLast = GetAt(m_nSize);
			memcpy(pLast, obj, len * GetElemSize());
		}

		typedef  int(*_sort_compare)(const void* , const void* );
		void Sort(_sort_compare func)
		{
			if (m_data && m_nSize > 1)
			{
				std::qsort(m_data, m_nSize, GetElemSize(), func);
			}
			
		}


	protected:
		void CheckAlloc(int nAddElemCount = 1)
		{
			int nNewSize = m_nSize + nAddElemCount;
			if (nNewSize >= m_nAlloc)
			{
				Alloc(nNewSize + 1);
			}
		}

		void Alloc(int nSize)
		{
			int nAlloc = (nSize + 7) / 8 * 8;
			_Type* pNew = new _Type[nAlloc];
			if (m_data)
			{
				if (m_nSize > 0)
				{
					memcpy(pNew, m_data, m_nSize * GetElemSize());
				}

				delete[] m_data;
				m_data = nullptr;
			}

			m_data = pNew;
			m_nAlloc = nAlloc;
		}


		int GetDataBufferSize()
		{
			return m_nSize * GetElemSize();
		}

		_Type* GetDataBuffer()
		{
			return m_data;
		}
		int GetElemSize()
		{
			return sizeof(_Type);
		}

	protected:
		_Type* m_data;
		int	 m_nSize;				// 数组大小
		int	  m_nAlloc;			// 分配的元素个数
	};
}