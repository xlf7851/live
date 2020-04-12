#include "stdafx.h"
#include "fmtobj.h"

namespace xlf
{
	int CUnt32Array::FindValue(uint32 value) const
	{
		for (int i = 0; i < m_nSize; i++)
		{
			if (m_data[i] == value)
			{
				return i;
			}
		}

		return -1;
	}
	
	bool CUnt32Array::RemoveValue(uint32 value)
	{
		int nIdex = FindValue(value);
		if (nIdex >= 0)
		{
			return RemoveAt(nIdex);
		}

		return false;
	}

	inline static int _compareUint32(const void* src, const void* dst)
	{
		return *((uint32*)src) - *((uint32*)dst);
	}

	void CUnt32Array::SortValue()
	{
		Sort(_compareUint32);
	}
}