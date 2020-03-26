#pragma once
#include "../base/fmtobj.h"
#include "stockArray.h"

namespace stock_wrapper
{
#define minute_day_count 242
#pragma pack(1)
	// 日线数据
	struct _day_data_item_t
	{
		long m_lDate;
		float m_open;
		float m_close;
		float m_high;
		float m_low;
		float m_change;
		double m_money;
		double m_vol;
		double m_outVol;

		_day_data_item_t()
		{
			memset(this, 0, sizeof(_day_data_item_t));
		}
	};

	// 分时
	struct _minute_data_item_t
	{
		float m_price;
		double m_dbVol;
	};

	struct _minute_data_node_t
	{
		long m_lDate;
		_minute_data_item_t m_data[minute_day_count];

		_minute_data_node_t& operator=(const _minute_data_node_t& src)
		{
			memcpy(this, &src, sizeof(_minute_data_node_t));
			
			return *this;
		}
	};
#pragma pack()

	class DayDataArray : public xlf::FmtObjArray<_day_data_item_t>
	{
	public:
		long GetFirstDate() const;
		long GetLastData()const;

		int FindIndexByDate();
		void SortByDate();
		void AppendData(const DayDataArray& data);
	};

	class MinuteDataArray : public xlf::FmtObjArray<_minute_data_node_t>
	{
	public:
		void SortByDate();
		_minute_data_node_t* FindByDate(long lDate);
	};

	class MarketMinuteData
	{
	public:
		typedef std::map<Stock, MinuteDataArray*> _data_map_t;
		typedef _data_map_t::iterator _data_map_iterator_t;
	public:
		MarketMinuteData();
		virtual ~MarketMinuteData();

		void Init(LPCTSTR lpszMarket, LPCTSTR lpszFilePath = NULL);

		void Clear();

		void LoadFrom(LPCTSTR lpszDir = nullptr);
		void SaveTo(LPCTSTR lpszDir = nullptr);

		static bool ReadFlag(const unsigned char* buf, int len, Stock& code, int& readLen);
		void ReadCodeDataFromFile(LPCTSTR lpszFilePath);
		// 数据中有数据标志信息， bParseFlag ： code为空不为空时，跳过检查标志信息
		int ReadCodeDataFromBufferWidthFlag(Stock& code, const unsigned char* buf, int len);
		// 数据中无数据标志信息
		int ReadCodeDataFromBuffer(const Stock& code, const unsigned char* buf, int len);


		MinuteDataArray* GetData(const Stock& code, BOOL bCreate = FALSE);
		void UpdateData(const Stock& code, const MinuteDataArray& data);
		void AppendData(const Stock& code, const MinuteDataArray& data);
		void UpdateData(const Stock& code, const _minute_data_node_t& data);

		bool IsThisMarket(LPCTSTR lpszMarket);

		void Lock();
		void Unlock();

	protected:
		_data_map_t m_mapData;
		std::string m_strFilePathRoot;
		std::string m_strMarket;

	};


	class MarketDayData
	{
	public:
		typedef std::map<Stock, DayDataArray*> _data_map_t;
		typedef _data_map_t::iterator _data_map_iterator_t;
	public:
		MarketDayData();
		virtual ~MarketDayData();

		void Init(LPCTSTR lpszMarket, LPCTSTR lpszFilePath = NULL);

		void Clear();
		void ReadFromFile(LPCTSTR lpszFile = NULL);
		void WriteToFile(LPCTSTR lpszFile = NULL);
		static bool ReadFlag(const unsigned char* buf, int len, Stock& code, int& readLen);
		// 数据中有数据标志信息， bParseFlag ： code为空不为空时，跳过检查标志信息
		int ReadCodeDataFromBufferWidthFlag(Stock& code, const unsigned char* buf, int len);
		// 数据中无数据标志信息
		int ReadCodeDataFromBuffer(const Stock& code, const unsigned char* buf, int len);


		DayDataArray* GetData(const Stock& code, BOOL bCreate = FALSE);
		void UpdateData(const Stock& code, const DayDataArray& data);
		void AppendData(const Stock& code, const DayDataArray& data);

		bool IsThisMarket(LPCTSTR lpszMarket);

		void Lock();
		void Unlock();

	protected:
		_data_map_t m_mapData;
		std::string m_strFilePathRoot;
		std::string m_strMarket;
		
	};

	class MarketDayDataPool
	{
	public:
		typedef std::map<std::string, MarketDayData*> _data_map_t;
		typedef _data_map_t::iterator _data_map_iterator_t;
	public:
		MarketDayDataPool();
		virtual ~MarketDayDataPool();

		void Init(LPCTSTR lpszFileRoot);
		void Read(LPCTSTR lpszMarket);
		void Write(LPCTSTR lpszMarket);


		MarketDayData* GetData(const std::string& strMarket, BOOL bCreate = FALSE);
		void UpdateData(const std::string& strMarket, const Stock& code, const DayDataArray& data);

	protected:
		_data_map_t m_mapData;
		std::string m_strRoot;
	};

	class MarketMinuteDataPool
	{
	public:
		typedef std::map<std::string, MarketMinuteData*> _data_map_t;
		typedef _data_map_t::iterator _data_map_iterator_t;
	public:
		MarketMinuteDataPool();
		virtual ~MarketMinuteDataPool();

		void Init(LPCTSTR lpszFileRoot);
		void Read(LPCTSTR lpszMarket);
		void Write(LPCTSTR lpszMarket);


		MarketMinuteData* GetData(const std::string& strMarket, BOOL bCreate = FALSE);
		void UpdateData(const std::string& strMarket, const Stock& code, const MinuteDataArray& data);

	protected:
		_data_map_t m_mapData;
		std::string m_strRoot;
	};

	enum stock_data_type_t
	{
		stock_data_type_error = 0,
		stock_data_type_minute,
		stock_data_type_day,
		
	};

	class StockDataPool
	{
	public:
		StockDataPool();
		~StockDataPool();

		void InitDataPath(LPCTSTR lpszFilePath);
		void LoadData();
		void LoadMarketData(const std::string& strMarket);

		static StockDataPool* GetStokDataPool(bool bNew = true);
	public:
		static stock_data_type_t CheckStockDataType(const xlf::CBuffer& buf, Stock* pStock = NULL);
		void GetMinuteData(const std::string& market, const Stock& code, MinuteDataArray& ayData);
		void GetMinuteData(const std::string market, const Stock& code, long lDate, _minute_data_node_t& data);

	protected:
		void LoadTradeData();
	protected:
		_tstring m_strRoot;
		_tstring m_strConfigFilePath;
		MarketMinuteDataPool* m_pMinuteDataPool;
		MarketDayDataPool* m_pDayDataPool;
		std::vector<long> m_vcTradeDate;
	};
}