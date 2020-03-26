#include "stdafx.h"
#include "stockdata.h"
#include "../base/buffer.h"
#include "../base/utl.h"

namespace stock_wrapper
{
#define stock_data_flag_day		("daydata")
#define stock_data_flag_minute  ("fenshidata")

#define ini_section_system	(_T("system"))
#define ini_key_tradedate	(_T("tradeDate"))

#define stock_data_dir_day (_T("day\\"))
#define stock_data_dir_minute (_T("min\\"))

	static int _FindMarketStringIndex(LPCTSTR lpszMarket)
	{
		static LPCTSTR s_ayMarket[] = { _T("USHA"), _T("USZA") };
		int nCnt = _countof(s_ayMarket);
		for (int i = 0; i < nCnt; i++)
		{
			if (_tcsicmp(lpszMarket, s_ayMarket[i]) == 0)
			{
				return i;
			}
		}

		return -1;
	}

	static _tstring _makeMarketDirPath(const _tstring& strRoot, const std::string strMarket, LPCTSTR lpszDataDir)
	{
		_tstring strPath = strRoot + strMarket + _T("\\");
		strPath += lpszDataDir;
		return strPath;
	}

	static inline bool _IsStockDayData(const char* flag)
	{
		return _tcsicmp(flag, stock_data_flag_day) == 0;
	}

	static inline bool _IsStockMinuteData(const char* flag)
	{
		return _tcsicmp(flag, stock_data_flag_minute) == 0;
	}

	static stock_data_type_t _StockDataFlagToType(const char* flag)
	{
		if (_IsStockMinuteData(flag))
		{
			return stock_data_type_minute;
		}
		else if (_IsStockDayData(flag))
		{
			return stock_data_type_day;
		}
		else
		{
			return stock_data_type_error;
		}
	}

	inline std::string _MarketDayFileName(const std::string& strMarket)
	{
		return strMarket + _T(".dat");
	}

	// 返回读取的字节长度，包括字符串结束符，如果没有结束符，str为空，返回值为size
	inline int _ReadString(const unsigned char* pBuf, int size, std::string& str)
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

	static bool _ReadStockDataFlag(const unsigned char* data, int size, string& key, Stock& code, int& nReadSize)
	{
		nReadSize = 0;
		bool bSucc = false;
		std::string strData;
		int len = _ReadString(data, size, strData);
		if (len > 0 && strData.size() > 0)
		{
			std::string strValue;
			xlf::ParseKeyValue(strData, key, strValue);
			if (!key.empty() && !strValue.empty())
			{
				code = strValue.c_str();
				bSucc = true;
			}
		}
		if (len > 0)
		{
			nReadSize = len;
		}

		return bSucc;
	}

	static inline void _AppendZero(xlf::CBuffer& buf)
	{
		char c = 0;
		buf.Append(&c, 1);
	}

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

	static int _compareFenshiByDate(const void* src, const void* dst)
	{
		_minute_data_node_t* f = (_minute_data_node_t*)src;
		_minute_data_node_t* s = (_minute_data_node_t*)dst;
		return s->m_lDate - f->m_lDate;
	}

	void MinuteDataArray::SortByDate()
	{
		Sort(_compareFenshiByDate);
	}

	_minute_data_node_t* MinuteDataArray::FindByDate(long lDate)
	{
		int nSize = GetSize();
		for (int i = 0; i < nSize; i++)
		{
			_minute_data_node_t* pItem = GetAt(i);
			if (pItem->m_lDate = lDate)
			{
				return pItem;
			}
		}

		return nullptr;
	}

	MarketMinuteData::MarketMinuteData()
	{

	}

	MarketMinuteData::~MarketMinuteData()
	{

	}



	MinuteDataArray* MarketMinuteData::GetData(const Stock& code, BOOL bCreate)
	{
		MinuteDataArray* pFind = nullptr;
		_data_map_iterator_t it = m_mapData.find(code);
		if (it == m_mapData.end() || it->second == nullptr)
		{
			if (bCreate)
			{
				pFind = new MinuteDataArray;
				m_mapData[code] = pFind;
			}
		}
		else
		{
			pFind = it->second;
		}
		return pFind;
	}

	void MarketMinuteData::UpdateData(const Stock& code, const MinuteDataArray& data)
	{
		MinuteDataArray* pFind = GetData(code, TRUE);
		if (pFind)
		{
			*pFind = data;
		}
	}

	void MarketMinuteData::AppendData(const Stock& code, const MinuteDataArray& data)
	{
		MinuteDataArray* pFind = GetData(code, TRUE);
		if (pFind)
		{
			//pFind->AppendData(data);
		}
	}

	void MarketMinuteData::UpdateData(const Stock& code, const _minute_data_node_t& data)
	{
		MinuteDataArray* pFind = GetData(code, TRUE);
		if (pFind)
		{
		//	pFind->
		}
	}


	void MarketMinuteData::Lock()
	{
	}

	void MarketMinuteData::Unlock()
	{

	}

	void MarketMinuteData::Init(LPCTSTR lpszMarket, LPCTSTR lpszFilePath /* = NULL */)
	{
		m_strMarket = lpszMarket;
		if (lpszFilePath)
		{
			m_strFilePathRoot = lpszFilePath;
		}
	}

	bool MarketMinuteData::IsThisMarket(LPCTSTR lpszMarket)
	{
		return m_strMarket.compare(lpszMarket) == 0;
	}

	void MarketMinuteData::Clear()
	{
		for (_data_map_iterator_t it = m_mapData.begin(); it != m_mapData.end(); it++)
		{
			delete it->second;
		}
		m_mapData.clear();
	}


	void MarketMinuteData::LoadFrom(LPCTSTR lpszDir /* = nullptr */)
	{
		std::string strRoot = (lpszDir && lpszDir[0] != 0) ? lpszDir : m_strFilePathRoot;
		if (strRoot.empty())
		{
			return;
		}

		std::vector<std::string> vcFile;
		xlf::FindDirAllFile(strRoot.c_str(), vcFile);

		for (int i = 0; i < vcFile.size(); i++)
		{
			std::string strPath = m_strFilePathRoot + vcFile[i];
			ReadCodeDataFromFile(strPath.c_str());
		}

	}

	bool MarketMinuteData::ReadFlag(const unsigned char* buf, int len, Stock& code, int& readLen)
	{
		std::string key;
		return _ReadStockDataFlag(buf, len, key, code, readLen) && _IsStockMinuteData(key.c_str());
	}

	void MarketMinuteData::ReadCodeDataFromFile(LPCTSTR lpszFilePath)
	{
		xlf::CBuffer buf;
		xlf::ReadBufferFromFile(buf, lpszFilePath);
		if (buf.GetSize() > 0)
		{
			Stock code;
			ReadCodeDataFromBufferWidthFlag(code, buf.GetBuffer(), buf.GetSize());
		}
	}

	int MarketMinuteData::ReadCodeDataFromBufferWidthFlag(Stock& code, const unsigned char* buf, int len)
	{
		int nRead = 0;
		if (code.Empty())
		{
			if (!ReadFlag(buf, len, code, nRead) || code.Empty())
			{
				return nRead;
			}
		}
		else
		{
			std::string flag;
			nRead = _ReadString(buf, len, flag);
		}


		len -= nRead;
		buf += nRead;

		return ReadCodeDataFromBuffer(code, buf, len);
	}

	int MarketMinuteData::ReadCodeDataFromBuffer(const Stock& code, const unsigned char* buf, int len)
	{
		int nRead = 0;
		if (buf && len > 0)
		{
			MinuteDataArray* pArray = GetData(code, TRUE);
			if (pArray)
			{
				nRead = pArray->ReadFromBuf((char*)buf, len);
			}
		}

		return nRead;
	}

	

	void MarketMinuteData::SaveTo(LPCTSTR lpszDir /* = nullptr */)
	{
		std::string strRoot = ( lpszDir && lpszDir[0]!= 0) ? lpszDir : m_strFilePathRoot;
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

// 			std::string code = it->first.ToString();
// 			buffer.Append(code.c_str(), code.size());
// 			_AppendZero(buffer);
// 			it->second->WriteToBuf(buffer);
		}
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

	bool MarketDayData::ReadFlag(const unsigned char* buf, int len, Stock& code, int& readLen)
	{
		std::string key;
		return _ReadStockDataFlag(buf, len, key, code, readLen) && _IsStockDayData(key.c_str());
	}

	int MarketDayData::ReadCodeDataFromBufferWidthFlag(Stock& code, const unsigned char* buf, int len)
	{
		int nRead = 0;
		if (code.Empty())
		{
			if (!ReadFlag(buf, len, code, nRead) || code.Empty())
			{
				return nRead;
			}
		}
		else
		{
			std::string flag;
			nRead = _ReadString(buf, len, flag);
		}
		

		len -= nRead;
		buf += nRead;

		return ReadCodeDataFromBuffer(code, buf, len);
	}

	int MarketDayData::ReadCodeDataFromBuffer(const Stock& code, const unsigned char* buf, int len)
	{
		int nRead = 0;
		if (buf && len > 0)
		{
			DayDataArray* pArray = GetData(code, TRUE);
			if (pArray)
			{
				nRead = pArray->ReadFromBuf((char*)buf, len);
			}
		}

		return nRead;
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
			std::string strRoot = _makeMarketDirPath(m_strRoot, strMarket, stock_data_dir_day);
			pFind->Init(strMarket.c_str(), strRoot.c_str());
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

	MarketMinuteDataPool::MarketMinuteDataPool()
	{

	}


	MarketMinuteDataPool::~MarketMinuteDataPool()
	{

	}

	void MarketMinuteDataPool::Init(LPCTSTR lpszFileRoot)
	{
		m_strRoot = lpszFileRoot;
	}

	void MarketMinuteDataPool::Read(LPCTSTR lpszMarket)
	{
		MarketMinuteData* pFind = GetData(lpszMarket, TRUE);
		if (pFind)
		{
			pFind->LoadFrom();
		}
	}

	void MarketMinuteDataPool::Write(LPCTSTR lpszMarket)
	{
		MarketMinuteData* pFind = GetData(lpszMarket, FALSE);
		if (pFind)
		{
			pFind->SaveTo();
		}
	}


	MarketMinuteData* MarketMinuteDataPool::GetData(const std::string& strMarket, BOOL bCreate)
	{
		MarketMinuteData* pFind = nullptr;
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
			pFind = new MarketMinuteData;
			std::string strRoot = _makeMarketDirPath(m_strRoot, strMarket, stock_data_dir_minute);
			pFind->Init(strMarket.c_str(), strRoot.c_str());
			m_mapData[strMarket] = pFind;
		}

		return pFind;
	}
	void MarketMinuteDataPool::UpdateData(const std::string& strMarket, const Stock& code, const MinuteDataArray& data)
	{
		MarketMinuteData* pFind = GetData(strMarket, TRUE);
		if (pFind)
		{
			pFind->UpdateData(code, data);
		}
	}


	StockDataPool::StockDataPool()
	{
		m_pDayDataPool = new MarketDayDataPool;
		m_pMinuteDataPool = new MarketMinuteDataPool;
	}

	StockDataPool::~StockDataPool()
	{
		if (m_pDayDataPool)
		{
			delete m_pDayDataPool;
		}

		if (m_pMinuteDataPool)
		{
			delete m_pMinuteDataPool;
		}
	}

	StockDataPool* StockDataPool::GetStokDataPool(bool bNew)
	{
		auto gd = xlf::TSingleton<xlf::CGlobalData>::Instance();
		if (gd)
		{
			return gd->GetStockDataPool();
		}

		return NULL;
	}

	void StockDataPool::InitDataPath(LPCTSTR lpszFilePath)
	{
		m_strRoot = lpszFilePath;
		if (!m_strRoot.empty())
		{
			m_strConfigFilePath = m_strRoot + _T("config.ini");
			m_pDayDataPool->Init(m_strRoot.c_str());
			m_pMinuteDataPool->Init(m_strRoot.c_str());
		}
		LoadTradeData();
	}

	void StockDataPool::LoadData()
	{
		if (m_strRoot.empty())
		{
			return;
		}
		std::vector<std::string> vcDir;
		xlf::FindDirAllDir(m_strRoot.c_str(), vcDir);
		for (int i = 0; i < vcDir.size(); i++)
		{
			LoadMarketData(vcDir[i]);
		}
	}

	void StockDataPool::LoadMarketData(const std::string& strMarket)
	{
		//m_pDayDataPool->Read(strMarket.c_str());
		m_pMinuteDataPool->Read(strMarket.c_str());
	}


	void StockDataPool::LoadTradeData()
	{
		if (m_strConfigFilePath.empty())
		{
			return;
		}

		//GetProfileString()

	}

	stock_data_type_t StockDataPool::CheckStockDataType(const xlf::CBuffer& buf, Stock* pStock /* = NULL */)
	{
		stock_data_type_t tp = stock_data_type_error;

		std::string key;
		Stock code;
		int nRead = 0;
		if (_ReadStockDataFlag(buf.GetBuffer(), buf.GetSize(), key, code, nRead))
		{
			tp = _StockDataFlagToType(key.c_str());
			if (tp != stock_data_type_error && pStock)
			{
				*pStock = code;
			}
		}

		return tp;
	}

	void StockDataPool::GetMinuteData(const std::string market, const Stock& code, long lDate, _minute_data_node_t& data)
	{
		MarketMinuteData* pMarketData = m_pMinuteDataPool->GetData(market, FALSE);
		if (pMarketData)
		{
			MinuteDataArray* pData = pMarketData->GetData(code, FALSE);
			if (pData)
			{
				_minute_data_node_t* pFind = pData->FindByDate(lDate);
				if (pFind)
				{
					data = *pFind;
				}
			}
		}
	}

	void StockDataPool::GetMinuteData(const std::string& market, const Stock& code, MinuteDataArray& ayData)
	{
		MarketMinuteData* pMarketData = m_pMinuteDataPool->GetData(market, FALSE);
		if (pMarketData)
		{
			MinuteDataArray* pData = pMarketData->GetData(code, FALSE);
			if (pData)
			{
				ayData = *pData;
			}
		}
	}
}