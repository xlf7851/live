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
		void AppendString(const _tstring& str); // ����׷�ӿ��ַ���
		void AppendStringEx(const _tstring& str, bool bAppendNull = false); // ǰ4���ֽڱ�ʾ�ַ������ȣ�Ȼ�����ַ�����bAppendNullָ���Ƿ�����ս�����
		
		void Clear();
	protected:
		void Alloc(int nNewSize);
	protected:
		unsigned char* m_data;
		int m_nSize;
		int m_nAlloc;
	};
}

