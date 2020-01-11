#pragma once


namespace xlf
{
	/**
	* @brief ʵ�ֶ�����ַ�������
	*/

	class FmtStringArray
	{
	public:
		FmtStringArray(int nElemSize = 8, int nAllocCount = 16);
		FmtStringArray(const FmtStringArray& src);
		virtual ~FmtStringArray();

		// ���������
	public:
		FmtStringArray& operator=(const FmtStringArray& src);

	public:
		LPCTSTR Add(LPCTSTR lpszText, int nLen = -1);
		LPCTSTR GetAt(int nIndex) const;
		int GetSize() const;
		void Clear();
		bool Empty();
		void SetCapacity(int nSize);

	protected:
		void CheckAlloc(int nAddElemCount = 1);
		void Alloc(int nSize);
		TCHAR* GetData(int nSize = -1) const;
		int GetDataBufferSize();
		TCHAR* GetDataBuffer();

	protected:
		TCHAR* m_data;
		int	 m_nSize;				// �����С
		int	   m_nAlloc;			// �����Ԫ�ظ���
			
		int m_nAddCount;			// ÿ�η������ӵĸ���
		int m_nElemSize;			// ÿ��Ԫ�صĴ�С
	};
}