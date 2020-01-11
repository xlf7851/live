#pragma once


namespace xlf
{
	/**
	* @brief 实现对齐的字符串数组
	*/

	class FmtStringArray
	{
	public:
		FmtStringArray(int nElemSize = 8, int nAllocCount = 16);
		FmtStringArray(const FmtStringArray& src);
		virtual ~FmtStringArray();

		// 运算符重载
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
		int	 m_nSize;				// 数组大小
		int	   m_nAlloc;			// 分配的元素个数
			
		int m_nAddCount;			// 每次分配增加的个数
		int m_nElemSize;			// 每个元素的大小
	};
}