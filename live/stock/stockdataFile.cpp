#include "stdafx.h"
#include "stockdataFile.h"
#include "stockArray.h"
#include "../base/file.h"

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

		const char* pBuf = (char*)buf.GetBuffer();
		int len = buf.GetSize();
		_tstring str;
		int nRead = _ReadString((unsigned const char*)pBuf, len, str);
		if (nRead <= 0)
		{
			return;
		}

		pBuf += nRead;
		len -= nRead;

		ReadFromBuffer(pBuf, len);
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
		WriteToBuffer(buf);

		xlf::WriteBufferToFile(buf, m_strFilePath.c_str());

	}

	const static char g_szFileHeaderFlag[4] = {'X','L', 'F', 'D' };
	static uint32 _GetStockFileFlag()
	{
		return *((uint32*)g_szFileHeaderFlag);
	}

	BaseStockFileParse::BaseStockFileParse()
	{
		m_dataType = STOCK_DATA_TYPE_NONE;
		m_uParam = 0;
	}

	bool BaseStockFileParse::ParseFromFile(LPCTSTR lpszFilePath)
	{
		xlf::CTFile file;
		if (!file.Open(lpszFilePath))
		{
			return false;
		}

		int nHeaderLen = sizeof(StockFileHeader);
		DWORD len = file.GetFileLength();
		if (len < nHeaderLen)
		{
			return false;
		}

		m_dataBuf.Reserve(nHeaderLen + 1);
		DWORD nRead = file.Read(m_dataBuf.GetBuffer(), nHeaderLen);
		if (nRead < nHeaderLen)
		{
			return false;
		}

		len -= nHeaderLen;

		// check header
		StockFileHeader* pHeader = (StockFileHeader*)m_dataBuf.GetBuffer();
		if (pHeader->m_size != sizeof(StockFileHeader) || _GetStockFileFlag() != pHeader->m_flag)
		{
			return false;
		}
		m_stock.Assign(pHeader->m_market, pHeader->m_code);
		if (m_stock.Empty())
		{
			return false;
		}

		int nDataLen = pHeader->m_totalSize - nHeaderLen;
		if (len < nDataLen)
		{
			return false;
		}

		m_dataType = pHeader->m_uDataType;
		m_uParam = pHeader->m_uParam;

		m_dataBuf.Reserve(nDataLen);
		nRead = file.Read(m_dataBuf.GetBuffer(), nDataLen);
		if (nRead < nDataLen)
		{
			return false;
		}
		file.Close();
		m_dataBuf.Resize(nRead);
	

		return true;
	}

	
	static void ParseThsStockFileData(const StockCode& code, uint32 uStockType, LPCTSTR lpszFilePath)
	{
		xlf::CBuffer buffer;
		xlf::ReadBufferFromFile(buffer, lpszFilePath);
		if (buffer.GetSize() > 0)
		{
			int nRead = 0;
			 unsigned char* buf = buffer.GetBuffer();
			int len = buffer.GetSize();
		
			std::string flag;
			nRead = _ReadString(buf, len, flag);
			len -= nRead;
			buf += nRead;

			if (len > 0)
			{
				StockDataPool::Instance()->HandleDataFromBuffer(code, uStockType, (const char*)buf, len);
			}
		}
	}

	void CLoadThsStockFile::Load(LPCTSTR lpDir)
	{
		std::vector<_tstring> vcMarket;
		xlf::FindDirAllDir(lpDir, vcMarket);
		uint32 uMarket = 0;
		_tstring strRoot = lpDir;
		for (int i = 0; i < vcMarket.size(); i++)
		{
			uMarket = StringToMarket(vcMarket[i].c_str());
			if (uMarket == 0)
			{
				continue;
			}
			_tstring strMarketRoot = strRoot + vcMarket[i] + _T("\\");
			std::vector<_tstring> vcType;
			xlf::FindDirAllDir(strMarketRoot.c_str(), vcType);
			for (int j = 0; j < vcType.size(); j++)
			{
				uint32 uStockDataType = STOCK_DATA_TYPE_NONE;
				_tstring& strType = vcType[j];
				if (_tcsicmp(strType.c_str(), _T("day")) == 0)
				{
					uStockDataType = STOCK_DATA_TYPE_DAY;
				}
				else if (_tcsicmp(strType.c_str(), _T("min")) == 0)
				{
					uStockDataType = STOCK_DATA_TYPE_MINUTE;
				}
				else
				{
					continue;
				}

				_tstring strStockFileRoot = strMarketRoot + strType + _T("\\");
				std::vector<_tstring> vcStock;
				xlf::FindDirAllFile(strStockFileRoot.c_str(), vcStock);
				for (int i = 0; i < vcStock.size(); i++)
				{
					_tstring strStock = vcStock[i];
					int nFlag = strStock.find(_T('.'));
					if (nFlag < 1 && nFlag >= sizeof(Stock))
					{
						continue;
					}

					Stock stock(strStock.c_str(), nFlag);
					StockCode code;
					code.Assign(uMarket, stock);
					_tstring strFilePath = strStockFileRoot + strStock;

					ParseThsStockFileData(code, uStockDataType, strFilePath.c_str());
				}
			
			}

		}
	}

}

