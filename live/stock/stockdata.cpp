#include "stdafx.h"
#include "stockdata.h"
#include "../base/buffer.h"
#include "../base/utl.h"

namespace stock_wrapper
{

	long DayDataArray::GetFirstDate() const
	{
		long lDate = 0;
		int nSize = GetSize();
		if (nSize > 0)
		{
			_day_data_item_t* pData = GetAt(nSize - 1);
			if (pData)
			{
				lDate = pData->m_lDate;
			}
		}

		return lDate;
	}

	long DayDataArray::GetLastData() const
	{
		long lDate = 0;
		int nSize = GetSize();
		if (nSize > 0)
		{
			_day_data_item_t* pData = GetAt(0);
			if (pData)
			{
				lDate = pData->m_lDate;
			}
		}

		return lDate;
	}
	static int _compareByDate(const void* src, const void* dst)
	{
		_day_data_item_t* f = (_day_data_item_t*)src;
		_day_data_item_t* s = (_day_data_item_t*)dst;
		return s->m_lDate - f->m_lDate;
	}

	void DayDataArray::SortByDate()
	{
		Sort(_compareByDate);
	}

	int DayDataArray::FindIndexByDate()
	{
		return -1;
	}

	void DayDataArray::AppendData(const DayDataArray& data)
	{

	}

	MarketDayData::MarketDayData()
	{

	}

	MarketDayData::~MarketDayData()
	{

	}

	DayDataArray* MarketDayData::GetData(const Stock& code, BOOL bCreate)
	{
		DayDataArray* pFind = nullptr;
		_data_map_iterator_t it = m_mapData.find(code);
		if (it == m_mapData.end() || it->second == nullptr)
		{
			if (bCreate)
			{
				pFind = new DayDataArray;
				m_mapData[code] = pFind;
			}
		}
		else
		{
			pFind = it->second;
		}
		return pFind;
	}

	void MarketDayData::UpdateData(const Stock& code, const DayDataArray& data)
	{
		DayDataArray* pFind = GetData(code, TRUE);
		if (pFind)
		{
			*pFind = data;
		}
	}

	void MarketDayData::AppendData(const Stock& code, const DayDataArray& data)
	{
		DayDataArray* pFind = GetData(code, TRUE);
		if (pFind)
		{
			pFind->AppendData(data);
		}
	}


	void MarketDayData::Lock()
	{
	}

	void MarketDayData::Unlock()
	{

	}

	void MarketDayData::Init(LPCTSTR lpszMarket, LPCTSTR lpszFilePath /* = NULL */)
	{
		m_strMarket = lpszMarket;
		if (lpszFilePath)
		{
			m_strFilePathRoot = lpszFilePath;
		}
	}

	bool MarketDayData::IsThisMarket(LPCTSTR lpszMarket)
	{
		return m_strMarket.compare(lpszMarket) == 0;
	}

	void MarketDayData::Clear()
	{
		for (_data_map_iterator_t it = m_mapData.begin(); it !=m_mapData.end(); it++)
		{
			delete it->second;
		}
		m_mapData.clear();
	}

	inline std::string _MarketDayFileName(const std::string& strMarket)
	{
		return strMarket + _T(".dat");
	}

	inline int _ReadString(unsigned char* pBuf, int size, std::string& str)
	{
		int i = 0;
		for (; i < size; i++)
		{
			if (pBuf[i] == 0)
			{
				str.assign((char*)pBuf);
				i++;
				break;
			}
		}
		
		return i;
	}

	void MarketDayData::ReadFromFile(LPCTSTR lpszFile)
	{
		std::string strRoot = lpszFile ? lpszFile : m_strFilePathRoot;
		if (strRoot.empty())
		{
			return;
		}

		strRoot += _MarketDayFileName(m_strMarket);
		
		xlf::CBuffer buffer;
		xlf::ReadBufferFromFile(buffer, strRoot.c_str());
		

		unsigned char* pBuf = buffer.GetBuffer();
		int size = buffer.GetSize();

		std::string tmp;
		int read = _ReadString(pBuf, size, tmp);
		size -= read;
		pBuf += read;
		while (size > 0)
		{
			// 
			tmp.clear();
			read = _ReadString(pBuf, size, tmp);
			size -= read;
			pBuf += read;

			stock_wrapper::Stock stock = tmp.c_str();
			if (stock.Empty())
			{
				break;
			}

			DayDataArray* pArray = GetData(stock, TRUE);
			if (NULL == pArray)
			{
				break;
			}

			read = pArray->ReadFromBuf((char*)pBuf, size);
			size -= read;
			pBuf += read;
		}
		
	}

	static inline void _AppendZero(xlf::CBuffer& buf)
	{
		char c = 0;
		buf.Append(&c, 1);
	}

	void MarketDayData::WriteToFile(LPCTSTR lpszFile)
	{
		std::string strRoot = lpszFile ? lpszFile :  m_strFilePathRoot;
		if (strRoot.empty())
		{
			return;
		}

		strRoot += _MarketDayFileName(m_strMarket);

		if (m_mapData.size() == 0)
		{
			return;
		}
		
		xlf::CBuffer buffer;
		// market flag
		std::string strTm;
		strTm = _T("market=") + m_strMarket;
		buffer.Append(strTm.c_str(), strTm.size());
		_AppendZero(buffer);

		for (_data_map_iterator_t it = m_mapData.begin(); it != m_mapData.end(); it++)
		{
			if (it->second == nullptr || it->second->GetSize() == 0)
			{
				continue;
			}

			std::string code = it->first.ToString();
			buffer.Append(code.c_str(), code.size());
			_AppendZero(buffer);
			it->second->WriteToBuf(buffer);
		}
	}

	MarketDayDataPool::MarketDayDataPool()
	{

	}

	
	MarketDayDataPool::~MarketDayDataPool()
	{

	}

	void MarketDayDataPool::Init(LPCTSTR lpszFileRoot)
	{
		m_strRoot = lpszFileRoot;
	}

	void MarketDayDataPool::Read(LPCTSTR lpszMarket)
	{
		MarketDayData* pFind = GetData(lpszMarket, TRUE);
		if (pFind)
		{
			pFind->ReadFromFile();
		}
	}

	void MarketDayDataPool::Write(LPCTSTR lpszMarket)
	{
		MarketDayData* pFind = GetData(lpszMarket, TRUE);
		if (pFind)
		{
			pFind->WriteToFile();
		}
	}


	MarketDayData* MarketDayDataPool::GetData(const std::string& strMarket, BOOL bCreate)
	{
		MarketDayData* pFind = nullptr;
		for (_data_map_iterator_t it = m_mapData.begin(); it != m_mapData.end(); it++)
		{
			if (it->first == strMarket)
			{
				pFind = it->second;
				break;
			}
		}

		if (nullptr == pFind && bCreate)
		{
			pFind = new MarketDayData;
			pFind->Init(strMarket.c_str(), m_strRoot.c_str());
			m_mapData[strMarket] = pFind;
		}

		return pFind;
	}
	void MarketDayDataPool::UpdateData(const std::string& strMarket, const Stock& code, const DayDataArray& data)
	{
		MarketDayData* pFind = GetData(strMarket, TRUE);
		if (pFind)
		{
			pFind->UpdateData(code, data);
		}
	}
}