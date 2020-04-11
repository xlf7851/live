#pragma once
#include "stockdata.h"

namespace stock_wrapper
{

	class TradeDate : public xlf::FmtObjArray<long>
	{
	public:
		void SetFilePath(LPCTSTR lpszFilePath);
		void Read();
		void Write();

		void SortDate();
		void UpdateDate(const long* pDate, int len);

	protected:
		_tstring m_strFilePath;
	};

#pragma pack(1)
	struct StockFileHeader 
	{
		int32 m_size;			// 结构大小
		uint32 m_flag;			// 文件标记，"XLFD"
		uint32 m_uDataType;		// 数据类型
		uint32 m_market;		// 市场
		char   m_code[8];		// 代码
		uint32 m_uParam;			// 保留一个参数
		int32 m_totalSize;		// 总的大小，包括数据和文件头部分
	};
#pragma pack()

	

	class BaseStockFileParse
	{
	public:
		BaseStockFileParse();

		bool ParseFromFile(LPCTSTR lpszFilePath);

	public:
		StockCode m_stock;
		uint32 m_dataType;
		uint32 m_uParam;
		xlf::CBuffer m_dataBuf;

	};

	class CLoadThsStockFile
	{
	public:
		static void Load(LPCTSTR lpDir);

	};
}