#include "stdafx.h"
#include "stockdataFile.h"
#include "stockArray.h"

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



static inline bool _IsStockDayData(const char* flag)
{
	return _tcsicmp(flag, stock_data_flag_day) == 0;
}

static inline bool _IsStockMinuteData(const char* flag)
{
	return _tcsicmp(flag, stock_data_flag_minute) == 0;
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


	// 	MarketBaseData::MarketBaseData()
	// 	{
	// 
	// 	}
	// 
	// 	MarketBaseData::~MarketBaseData()
	// 	{
	// 		Clear();
	// 	}
	// 
	// 	void MarketBaseData::Clear()
	// 	{
	// 		for (_data_map_iterator_t it = m_mapData.begin(); it != m_mapData.end(); it++)
	// 		{
	// 			delete it->second;
	// 		}
	// 		m_mapData.clear();
	// 	}
	// 
	// 	void MarketBaseData::Init(uint32 uMarket, LPCTSTR lpszFilePath /* = NULL */)
	// 	{
	// 		m_uMarket = uMarket;
	// 		if (lpszFilePath)
	// 		{
	// 			m_strFilePathRoot = lpszFilePath;
	// 		}
	// 	}
	// 
	// 	void MarketBaseData::LoadFrom(LPCTSTR lpszDir /* = nullptr */)
	// 	{
	// 		std::string strRoot = (lpszDir && lpszDir[0] != 0) ? lpszDir : m_strFilePathRoot;
	// 		if (strRoot.empty())
	// 		{
	// 			return;
	// 		}
	// 
	// 		std::vector<std::string> vcFile;
	// 		xlf::FindDirAllFile(strRoot.c_str(), vcFile);
	// 
	// 		for (int i = 0; i < vcFile.size(); i++)
	// 		{
	// 			std::string strPath = m_strFilePathRoot + vcFile[i];
	// 			ReadCodeDataFromFile(strPath.c_str());
	// 		}
	// 	}
	// 
	// 	void MarketBaseData::SaveTo(LPCTSTR lpszDir /* = nullptr */)
	// 	{
	// 		std::string strRoot = (lpszDir && lpszDir[0] != 0) ? lpszDir : m_strFilePathRoot;
	// 		if (strRoot.empty())
	// 		{
	// 			return;
	// 		}
	// 
	// 		if (m_mapData.size() == 0)
	// 		{
	// 			return;
	// 		}
	// 
	// 
	// 		_tstring strFileName;
	// 		for (_data_map_iterator_t it = m_mapData.begin(); it != m_mapData.end(); it++)
	// 		{
	// 			if (it->second == nullptr || it->second->GetStockDataSize() == 0)
	// 			{
	// 				continue;
	// 			}
	// 
	// 			_StockFileName(it->first, strFileName);
	// 			_tstring strFilePath = strRoot + strFileName;
	// 			WriteCodeDataToFile(it->first, it->second, strFilePath.c_str());
	// 		}
	// 	}
	// 
	// 	void MarketBaseData::ReadCodeDataFromFile(LPCTSTR lpszFilePath)
	// 	{
	// 		xlf::CBuffer buf;
	// 		xlf::ReadBufferFromFile(buf, lpszFilePath);
	// 		if (buf.GetSize() > 0)
	// 		{
	// 			Stock code;
	// 			ReadCodeDataFromBufferWidthFlag(code, buf.GetBuffer(), buf.GetSize());
	// 		}
	// 	}
	// 
	// 	void MarketBaseData::WriteCodeDataToFile(const Stock& code, IStockDataArray* pData, LPCTSTR lpszFile)
	// 	{
	// 
	// 	}
	// 
	// 	int MarketBaseData::ReadCodeDataFromBufferWidthFlag(Stock& code, const unsigned char* buf, int len)
	// 	{
	// 		int nRead = 0;
	// 		if (code.Empty())
	// 		{
	// 			if (!ReadFlag(buf, len, code, nRead) || code.Empty())
	// 			{
	// 				return nRead;
	// 			}
	// 		}
	// 		else
	// 		{
	// 			std::string flag;
	// 			nRead = _ReadString(buf, len, flag);
	// 		}
	// 
	// 
	// 		len -= nRead;
	// 		buf += nRead;
	// 
	// 		return ReadCodeDataFromBuffer(code, buf, len);
	// 	}
	// 
	// 	int MarketBaseData::ReadCodeDataFromBuffer(const Stock& code, const unsigned char* buf, int len)
	// 	{
	// 		int nRead = 0;
	// 		if (buf && len > 0)
	// 		{
	// 			IStockDataArray* pArray = GetData(code, TRUE);
	// 			if (pArray)
	// 			{
	// 				nRead = pArray->ReadStockDataFromBuf((char*)buf, len);
	// 			}
	// 		}
	// 
	// 		return nRead;
	// 	}
	// 
	// 	bool MarketBaseData::ReadFlag(const unsigned char* buf, int len, Stock& code, int& readLen)
	// 	{
	// 		std::string key;
	// 		return _ReadStockDataFlag(buf, len, key, code, readLen) && IsThisDataFlag(key.c_str());
	// 	}


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


}

