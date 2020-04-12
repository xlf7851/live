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

		_Type& operator[](int index)
		{
			return GetAt(index);
		}

		const _Type& operator[](int index) const
		{
			return GetAt(index);
		}

	public:
		_Type* Add(const _Type& obj)
		{
			CheckAlloc(1);
			_Type* pData = GetPtr(m_nSize);
			memcpy(pData, &obj, GetElemSize());
			m_nSize++;

			return pData;
		}

		_Type& GetAt(int nIndex)
		{
			return m_data[nIndex];
		}

		const _Type& GetAt(int nIndex) const
		{
			return m_data[nIndex];
		}

		bool RemoveAt(int index)
		{
			if (index < 0 || index >= m_nSize)
			{
				return false;
			}

			if (index < m_nSize - 1 && m_nSize > 1)
			{
				memmove(m_data + index, m_data + index + 1, sizeof(_Type) * m_nSize-1-index);
			}
			m_nSize--;

			return true;
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

		virtual void WriteToBuffer(CBuffer& buf)
		{
			// 元素的大小
			int nElem = GetElemSize();
			buf.Append((unsigned char*)&nElem, sizeof(int));

			// 数组长度
			buf.Append((unsigned char*)&m_nSize, sizeof(int));
			
			// 数据部分
			if (m_nSize > 0)
			{
				buf.Append((unsigned char*)m_data, m_nSize * GetElemSize());
			}
		}

		virtual bool ReadFromBuffer(const char*& buf, int& len)
		{
			if (buf == nullptr || len < sizeof(int))
			{
				return false;
			}

			// 元素的大小
			int nElem = *((int*)buf);
			if (nElem != GetElemSize())
			{
				return false;
			}
			buf += sizeof(int);
			len -= sizeof(int);

			if (len < sizeof(int))
			{
				return false;
			}
			// 数组的大小
			int nArrayLen = *((int*)buf);
			len -= sizeof(int);
			buf += sizeof(int);

			int nDataLen = nArrayLen * nElem;
			if (nDataLen > len)
			{
				return false;
			}

			// 数据部分
			if (nArrayLen > 0)
			{
				Append((const _Type*)(buf), nArrayLen);

				len -= nDataLen;
				buf += nDataLen;
			}

			return true;
		}

		void Append(const _Type* obj, int len)
		{
			if (obj == nullptr || len <= 0)
			{
				return;
			}

			CheckAlloc(len);
			_Type* pLast = GetPtr(m_nSize);
			memcpy(pLast, obj, len * GetElemSize());
			m_nSize += len;
		}

		void Append(const FmtObjArray& src)
		{
			return Append(src.GetData(), src.GetSize());
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

		_Type* GetPtr(int index) const
		{
			return m_data + index;
		}
		

	protected:
		_Type* m_data;
		int	 m_nSize;				// 数组大小
		int	  m_nAlloc;			// 分配的元素个数
	};

	class CUnt32Array : public xlf::FmtObjArray<uint32>
	{
	public:
		int FindValue(uint32 value) const;
		bool RemoveValue(uint32 value);
		void SortValue();

	
	};
}