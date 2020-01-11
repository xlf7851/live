#pragma once
namespace xlf
{
	class CBuffer
	{
	public:
		CBuffer();
		~CBuffer();

		unsigned char* GetBuffer() const;
		int GetSize() const;
		void Resize(int size);
		void Reserve(int size);
		void Append(const void* pbuf, int nLen);
		void Clear();
	protected:
		void Alloc(int nNewSize);
	protected:
		unsigned char* m_data;
		int m_nSize;
		int m_nAlloc;
	};
}

