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
#define stock_data_dir_unknow (_T("unknow"))

	static inline void _StockFileName(const Stock& stock, _tstring& strFile)
	{
		strFile = stock.ToCString();
		strFile += _T(".dat");
	}
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

	static _tstring _StockDataTypeToDirName(stock_data_type_t _tp)
	{
		if (_tp == stock_data_type_minute)
		{
			return stock_data_dir_minute;
		}
		else if (_tp == stock_data_type_day)
		{
			return stock_data_dir_day;
		}
		else
		{
			return stock_data_dir_unknow;
		}
	}

	static _tstring _makeMarketDirPath(const _tstring& strRoot, uint32 uMarket, stock_data_type_t _tp)
	{
		_tstring strPath = strRoot;
		_tstring strMarket = MarketToString(uMarket);
		if (!strMarket.empty())
		{
			strPath += strMarket + _T("\\");
		}


		strPath += _StockDataTypeToDirName(_tp);
		
		
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

	

	MarketBaseData::MarketBaseData()
	{

	}

	MarketBaseData::~MarketBaseData()
	{
		Clear();
	}

	void MarketBaseData::Clear()
	{
		for (_data_map_iterator_t it = m_mapData.begin(); it != m_mapData.end(); it++)
		{
			delete it->second;
		}
		m_mapData.clear();
	}

	void MarketBaseData::Init(uint32 uMarket, LPCTSTR lpszFilePath /* = NULL */)
	{
		m_uMarket = uMarket;
		if (lpszFilePath)
		{
			m_strFilePathRoot = lpszFilePath;
		}
	}

	void MarketBaseData::LoadFrom(LPCTSTR lpszDir /* = nullptr */)
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

	void MarketBaseData::SaveTo(LPCTSTR lpszDir /* = nullptr */)
	{
		std::string strRoot = (lpszDir && lpszDir[0] != 0) ? lpszDir : m_strFilePathRoot;
		if (strRoot.empty())
		{
			return;
		}

		if (m_mapData.size() == 0)
		{
			return;
		}


		_tstring strFileName;
		for (_data_map_iterator_t it = m_mapData.begin(); it != m_mapData.end(); it++)
		{
			if (it->second == nullptr || it->second->GetStockDataSize() == 0)
			{
				continue;
			}

			_StockFileName(it->first, strFileName);
			_tstring strFilePath = strRoot + strFileName;
			WriteCodeDataToFile(it->first, it->second, strFilePath.c_str());
		}
	}

	void MarketBaseData::ReadCodeDataFromFile(LPCTSTR lpszFilePath)
	{
		xlf::CBuffer buf;
		xlf::ReadBufferFromFile(buf, lpszFilePath);
		if (buf.GetSize() > 0)
		{
			Stock code;
			ReadCodeDataFromBufferWidthFlag(code, buf.GetBuffer(), buf.GetSize());
		}
	}

	void MarketBaseData::WriteCodeDataToFile(const Stock& code, IStockDataArray* pData, LPCTSTR lpszFile)
	{

	}

	int MarketBaseData::ReadCodeDataFromBufferWidthFlag(Stock& code, const unsigned char* buf, int len)
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

	int MarketBaseData::ReadCodeDataFromBuffer(const Stock& code, const unsigned char* buf, int len)
	{
		int nRead = 0;
		if (buf && len > 0)
		{
			IStockDataArray* pArray = GetData(code, TRUE);
			if (pArray)
			{
				nRead = pArray->ReadStockDataFromBuf((char*)buf, len);
			}
		}

		return nRead;
	}

	bool MarketBaseData::ReadFlag(const unsigned char* buf, int len, Stock& code, int& readLen)
	{
		std::string key;
		return _ReadStockDataFlag(buf, len, key, code, readLen) && IsThisDataFlag(key.c_str());
	}

	IStockDataArray* MarketBaseData::GetData(const Stock& code, BOOL bCreate)
	{
		IStockDataArray* pFind = nullptr;
		_data_map_iterator_t it = m_mapData.find(code);
		if (it == m_mapData.end() || it->second == nullptr)
		{
			if (bCreate)
			{
				pFind = NewStockDataArray();
				if (pFind)
				{
					m_mapData[code] = pFind;
				}
			}
		}
		else
		{
			pFind = it->second;
		}
		return pFind;
	}

	void MarketBaseData::UpdateData(const Stock& code, const IStockDataArray* data)
	{
		IStockDataArray* pFind = GetData(code, TRUE);
		if (pFind)
		{
			pFind->CloneStockDataArray(data);
		}
	}

	void MarketBaseData::AppendData(const Stock& code, const IStockDataArray* data)
	{
		IStockDataArray* pFind = GetData(code, TRUE);
		if (pFind)
		{
			pFind->AppendStockDataArray(data);
		}
	}


	bool MarketBaseData::IsThisMarket(uint32 uMarket)
	{
		return uMarket == m_uMarket;
	}
	

	MarketDataBasePool::MarketDataBasePool()
	{

	}


	MarketDataBasePool::~MarketDataBasePool()
	{
		Clear();
	}

	void MarketDataBasePool::Clear()
	{
		for (_data_iterator_t it = m_data.begin(); it != m_data.end(); it++)
		{
			delete *it;
		}
		m_data.clear();
	}

	void MarketDataBasePool::Init(LPCTSTR lpszFileRoot)
	{
		m_strRoot = lpszFileRoot;
	}

	void MarketDataBasePool::Read(uint32 uMarket)
	{
		MarketBaseData* pFind = GetData(uMarket, TRUE);
		if (pFind)
		{
			pFind->LoadFrom();
		}
	}

	void MarketDataBasePool::Write(uint32 uMarket)
	{
		MarketBaseData* pFind = GetData(uMarket, TRUE);
		if (pFind)
		{
			pFind->SaveTo();
		}
	}


	MarketBaseData* MarketDataBasePool::GetData(uint32 uMarket, BOOL bCreate)
	{
		MarketBaseData* pFind = nullptr;
		for (_data_iterator_t it = m_data.begin(); it != m_data.end(); it++)
		{
			MarketBaseData* pCur = *it;
			if (pCur && pCur->IsThisMarket(uMarket))
			{
				pFind = pCur;
				break;
			}
		}

		if (nullptr == pFind && bCreate)
		{
			pFind = NewMarketData();
			if (pFind)
			{
				std::string strRoot = _makeMarketDirPath(m_strRoot, uMarket, pFind->GetMarketDataType());
				pFind->Init(uMarket, strRoot.c_str());
				m_data.push_back(pFind);
			}
		}

		return pFind;
	}

	void MarketDataBasePool::UpdateData(uint32 uMarket, const Stock& code, const IStockDataArray* data)
	{
		MarketBaseData* pFind = GetData(uMarket, TRUE);
		if (pFind)
		{
			pFind->UpdateData(code, data);
		}
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

	int DayDataArray::GetStockDataSize()
	{
		return GetSize();
	}

	int DayDataArray::ReadStockDataFromBuf(const char* buf, int len)
	{
		return ReadFromBuf(buf, len);
	}

	int DayDataArray::WriteStockDataToBuf(xlf::CBuffer& buf)
	{
		return WriteToBuf(buf);
	}

	void DayDataArray::AppendStockDataArray(const IStockDataArray* pArray)
	{
		if (pArray)
		{
			DayDataArray* pSrc = (DayDataArray*)pArray;
			Append(*pSrc);
		}
	}

	void DayDataArray::CloneStockDataArray(const IStockDataArray* pArray)
	{
		if (pArray)
		{
			DayDataArray* pSrc = (DayDataArray*)pArray;
			*this = *pSrc;
		}
	}


	MarketDayData::MarketDayData()
	{

	}

	MarketDayData::~MarketDayData()
	{

	}


	bool MarketDayData::IsThisDataFlag(LPCTSTR lpszFlag)
	{
		return _IsStockDayData(lpszFlag);
	}

	IStockDataArray* MarketDayData::NewStockDataArray()
	{
		return new DayDataArray;
	}


	MarketDayDataPool::MarketDayDataPool()
	{

	}


	MarketDayDataPool::~MarketDayDataPool()
	{

	}

	MarketBaseData* MarketDayDataPool::NewMarketData()
	{
		return new MarketDayData;
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

	int MinuteDataArray::GetStockDataSize()
	{
		return GetSize();
	}

	int MinuteDataArray::ReadStockDataFromBuf(const char* buf, int len)
	{
		return ReadFromBuf(buf, len);
	}

	int MinuteDataArray::WriteStockDataToBuf(xlf::CBuffer& buf)
	{
		return WriteToBuf(buf);
	}

	void MinuteDataArray::AppendStockDataArray(const IStockDataArray* pArray)
	{
		if (pArray)
		{
			MinuteDataArray* pSrc = (MinuteDataArray*)pArray;
			Append(*pSrc);
		}
	}

	void MinuteDataArray::CloneStockDataArray(const IStockDataArray* pArray)
	{
		if (pArray)
		{
			MinuteDataArray* pSrc = (MinuteDataArray*)pArray;
			*this = *pSrc;
		}
	}

	MarketMinuteData::MarketMinuteData()
	{

	}

	MarketMinuteData::~MarketMinuteData()
	{
		
	}

	bool MarketMinuteData::IsThisDataFlag(LPCTSTR lpszFlag)
	{
		return _IsStockMinuteData(lpszFlag);
	}

	IStockDataArray* MarketMinuteData::NewStockDataArray()
	{
		return new MinuteDataArray;
	}

	MarketMinuteDataPool::MarketMinuteDataPool()
	{

	}


	MarketMinuteDataPool::~MarketMinuteDataPool()
	{
		
	}

	MarketBaseData* MarketMinuteDataPool::NewMarketData()
	{
		return new MarketMinuteData;
	}


	void TradeDate::UpdateDate(const long* pDate, int len)
	{
		Clear();
		Append(pDate, len);
		Write();
	}

	static int _compareTradeDate(const void* src, const void* dst)
	{
		const long* f = (const long*)src;
		const long* s = (const long*)dst;
		return (*f - *s);
	}

	void TradeDate::SortDate()
	{
		Sort(_compareTradeDate);
	}

	void TradeDate::SetFilePath(LPCTSTR lpszFilePath)
	{
		m_strFilePath = lpszFilePath;
	}

	void TradeDate::Read()
	{
		if (m_strFilePath.empty())
		{
			return;
		}
		Clear();

		xlf::CBuffer buf;
		xlf::ReadBufferFromFile(buf, m_strFilePath.c_str());
		if (buf.GetSize() <= 0)
		{
			return;
		}

		unsigned char* pBuf = buf.GetBuffer();
		int len = buf.GetSize();
		_tstring str;
		int nRead = _ReadString(pBuf, len, str);
		if (nRead <= 0)
		{
			return;
		}

		pBuf += nRead;
		len -= nRead;

		ReadFromBuf((const char*)pBuf, len);
	}

	void TradeDate::Write()
	{
		if (m_strFilePath.empty())
		{
			return;
		}

		int nSize = GetSize();
		xlf::CBuffer buf;
		_tstring strFlag = _T("tradeDate=");
		TCHAR sz[64] = { 0 };
		_stprintf_s(sz, _T("%d"), nSize);
		strFlag += sz;
		buf.Append(strFlag.c_str(), strFlag.size());
		_AppendZero(buf);
		WriteToBuf(buf);

		xlf::WriteBufferToFile(buf, m_strFilePath.c_str());

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

	void StockDataPool::Clear()
	{
		if (m_pDayDataPool)
		{
			m_pDayDataPool->Clear();
		}

		if (m_pMinuteDataPool)
		{
			m_pMinuteDataPool->Clear();
		}
	}

	void StockDataPool::ClearMarket(uint32 uMarket)
	{
		if (m_pDayDataPool)
		{
			
		}

		if (m_pMinuteDataPool)
		{
		
		}
	}

	StockDataPool* StockDataPool::Instance()
	{
		static StockDataPool* s_obj = nullptr;
		if (s_obj == nullptr)
		{
			s_obj = new StockDataPool;
		}
		return s_obj;
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
		//LoadTradeData();
	}

	void StockDataPool::LoadData()
	{
		if (m_strRoot.empty())
		{
			return;
		}
		std::vector<_tstring> vcDir;
		xlf::FindDirAllDir(m_strRoot.c_str(), vcDir);
		for (int i = 0; i < vcDir.size(); i++)
		{
			uint32 uMarket = StringToMarket(vcDir[i].c_str());
			if (uMarket != 0)
			{
				LoadMarketData(uMarket);
			}
		}
	}

	void StockDataPool::LoadMarketData(uint32 uMarket)
	{
		m_pDayDataPool->Read(uMarket);
		m_pMinuteDataPool->Read(uMarket);
	}


	void StockDataPool::LoadTradeData()
	{
		if (m_strRoot.empty())
		{
			return;
		}

		_tstring strPath = m_strRoot + _T("tradeDate.dat");
		m_tradeDate.SetFilePath(strPath.c_str());
		m_tradeDate.Read();

	}

	void StockDataPool::GetTradeDate(TradeDate& tradeDate, uint32 uMarket)
	{
		tradeDate = m_tradeDate;
		tradeDate.SetFilePath(_T(""));
	}

	TradeDate* StockDataPool::GetTradeDate(uint32 uMarket)
	{
		return &m_tradeDate;
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

	MinuteDataArray* StockDataPool::GetMinuteDataArray(uint32 uMarket, const Stock& code, BOOL bCreate /* = FALSE */)
	{
		MarketMinuteData* pMarketData = (MarketMinuteData*)m_pMinuteDataPool->GetData(uMarket, bCreate);
		if (pMarketData)
		{
			return (MinuteDataArray*)pMarketData->GetData(code, bCreate);
		}

		return nullptr;
	}

	void StockDataPool::GetMinuteData(uint32 uMarket, const Stock& code, long lDate, _minute_data_node_t& data)
	{
		MinuteDataArray* pData = GetMinuteDataArray(uMarket, code, FALSE);
		if (pData)
		{
			_minute_data_node_t* pFind = pData->FindByDate(lDate);
			if (pFind)
			{
				data = *pFind;
			}
		}
	}

	void StockDataPool::GetMinuteData(uint32 uMarket, const Stock& code, MinuteDataArray& ayData)
	{
		MinuteDataArray* pData = GetMinuteDataArray(uMarket, code, FALSE);
		if (pData)
		{
			ayData = *pData;
		}
	}
}