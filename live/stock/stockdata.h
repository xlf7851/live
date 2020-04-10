#pragma once
#include "../base/fmtobj.h"
#include "stockArray.h"

namespace stock_wrapper
{

	enum stock_data_type_t
	{
		stock_data_type_error = 0,
		stock_data_type_minute,
		stock_data_type_day,

	};

#define minute_day_count 242
#pragma pack(1)
	// ��������
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

	// ��ʱ
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

	class IStockDataArray
	{
	public:
		virtual int GetStockDataSize() = 0;
		virtual int ReadStockDataFromBuf(const char* buf, int len) = 0;
		virtual int WriteStockDataToBuf(xlf::CBuffer& buf) = 0;
		virtual void AppendStockDataArray(const IStockDataArray* pArray) = 0;
		virtual void CloneStockDataArray(const IStockDataArray* pArray) = 0;
	};

	class MarketBaseData
	{
	public:
		typedef std::map<Stock, IStockDataArray*> _data_map_t;
		typedef _data_map_t::iterator _data_map_iterator_t;
	public:
		MarketBaseData();
		virtual ~MarketBaseData();

		virtual void Clear();
		virtual void Init(uint32 uMarket, LPCTSTR lpszFilePath = NULL);
		
		// ��Ŀ¼��д
		void LoadFrom(LPCTSTR lpszDir = nullptr);
		void SaveTo(LPCTSTR lpszDir = nullptr);

		// �ӵ����ļ��ж�д
		void ReadCodeDataFromFile(LPCTSTR lpszFilePath);
		void WriteCodeDataToFile(const Stock& code, IStockDataArray* pData, LPCTSTR lpszFile);

		// �ڴ��н�������
		// �����������ݱ�־��Ϣ�� bParseFlag �� codeΪ�ղ�Ϊ��ʱ����������־��Ϣ
		int ReadCodeDataFromBufferWidthFlag(Stock& code, const unsigned char* buf, int len);
		// �����������ݱ�־��Ϣ
		int ReadCodeDataFromBuffer(const Stock& code, const unsigned char* buf, int len);

		// ���ݶ�����
		bool ReadFlag(const unsigned char* buf, int len, Stock& code, int& readLen);
		
		
		// ���ݴ�ȡ�ӿ�
		IStockDataArray* GetData(const Stock& code, BOOL bCreate = FALSE);
		void UpdateData(const Stock& code, const IStockDataArray* data);
		void AppendData(const Stock& code, const IStockDataArray* data);

		bool IsThisMarket(uint32 uMarket);

		// ������д���麯��
		virtual bool IsThisDataFlag(LPCTSTR lpszFlag) { return false; }
		virtual IStockDataArray* NewStockDataArray() { return nullptr; }
		virtual stock_data_type_t GetMarketDataType() { return stock_data_type_error; }

	protected:
		_data_map_t m_mapData;
		_tstring m_strFilePathRoot;
		uint32 m_uMarket;

	};

	class MarketDataBasePool
	{
	public:
		typedef std::vector<MarketBaseData*> _data_container_t;
		typedef _data_container_t::iterator _data_iterator_t;
	public:
		MarketDataBasePool();
		virtual ~MarketDataBasePool();

		virtual void Clear();
		virtual void Init(LPCTSTR lpszFileRoot);

		void Read(uint32 uMarket);
		void Write(uint32 uMarket);


		MarketBaseData* GetData(uint32 uMarket, BOOL bCreate = FALSE);
		void UpdateData(uint32 uMarket, const Stock& code, const IStockDataArray* data);

	protected:
		virtual MarketBaseData* NewMarketData() { return nullptr; }
	protected:
		_data_container_t m_data;
		_tstring m_strRoot;
	};


	class DayDataArray : public xlf::FmtObjArray<_day_data_item_t>, public IStockDataArray
	{
	public:
		long GetFirstDate() const;
		long GetLastData()const;

		int FindIndexByDate();
		void SortByDate();

		virtual int GetStockDataSize();
		virtual int ReadStockDataFromBuf(const char* buf, int len);
		virtual int WriteStockDataToBuf(xlf::CBuffer& buf);
		virtual void AppendStockDataArray(const IStockDataArray* pArray);
		virtual void CloneStockDataArray(const IStockDataArray* pArray);
	};

	class MarketDayData : public MarketBaseData
	{
	public:
		MarketDayData();
		virtual ~MarketDayData();

		
		virtual bool IsThisDataFlag(LPCTSTR lpszFlag);
		virtual IStockDataArray* NewStockDataArray();
		virtual stock_data_type_t GetMarketDataType() { return stock_data_type_day; }

	};

	class MarketDayDataPool : public MarketDataBasePool
	{
	public:
		MarketDayDataPool();
		virtual ~MarketDayDataPool();

	protected:
		virtual MarketBaseData* NewMarketData();
	};



	class MinuteDataArray : public xlf::FmtObjArray<_minute_data_node_t>, public IStockDataArray
	{
	public:
		void SortByDate();
		_minute_data_node_t* FindByDate(long lDate);

		virtual int GetStockDataSize();
		virtual int ReadStockDataFromBuf(const char* buf, int len);
		virtual int WriteStockDataToBuf(xlf::CBuffer& buf);
		virtual void AppendStockDataArray(const IStockDataArray* pArray);
		virtual void CloneStockDataArray(const IStockDataArray* pArray);
	};


	class MarketMinuteData : public MarketBaseData
	{
	public:
		MarketMinuteData();
		virtual ~MarketMinuteData();

		virtual bool IsThisDataFlag(LPCTSTR lpszFlag);
		virtual IStockDataArray* NewStockDataArray();
		virtual stock_data_type_t GetMarketDataType() { return stock_data_type_minute; }

	};


	class MarketMinuteDataPool: public MarketDataBasePool
	{
	public:
		MarketMinuteDataPool();
		virtual ~MarketMinuteDataPool();

	protected:
		virtual MarketBaseData* NewMarketData();
	};


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


	class StockDataPool
	{
	public:
		StockDataPool();
		~StockDataPool();
		
		void Clear();
		void ClearMarket(uint32 uMarket);

		void InitDataPath(LPCTSTR lpszFilePath);
		void LoadData();
		void LoadMarketData(uint32 uMarket);

		static StockDataPool* Instance();

		// ������
		void GetTradeDate(TradeDate& tradeDate, uint32 uMarket = 0);
		TradeDate* GetTradeDate(uint32 uMarket = 0);

		// ��ʱ����
		MinuteDataArray* GetMinuteDataArray(uint32 uMarket, const Stock& code, BOOL bCreate = FALSE);
		void GetMinuteData(uint32 uMarket, const Stock& code, MinuteDataArray& ayData);
		void GetMinuteData(uint32 uMarket, const Stock& code, long lDate, _minute_data_node_t& data);

		static stock_data_type_t CheckStockDataType(const xlf::CBuffer& buf, Stock* pStock = NULL);

	protected:
		void LoadTradeData();
	protected:
		_tstring m_strRoot;
		_tstring m_strConfigFilePath;
		MarketMinuteDataPool* m_pMinuteDataPool;
		MarketDayDataPool* m_pDayDataPool;
		TradeDate m_tradeDate;
	};
}