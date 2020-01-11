#pragma once
#include "../base/fmtobj.h"
#include "stockArray.h"

namespace stock_wrapper
{
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
		_minute_data_item_t* m_data;
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
}