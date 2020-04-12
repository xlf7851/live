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
		unsigned char* Append(const void* pbuf, int nLen);
		void AppendChar(TCHAR c);
		void AppendNullChar();
		void AppendUInt32(uint32 val);
		void AppendString(const _tstring& str); // 可以追加空字符串
		void AppendStringEx(const _tstring& str, bool bAppendNull = false); // 前4个字节表示字符串长度，然后是字符串，bAppendNull指定是否包括空结束符
		
		void Clear();
	protected:
		void Alloc(int nNewSize);
	protected:
		unsigned char* m_data;
		int m_nSize;
		int m_nAlloc;
	};
}

