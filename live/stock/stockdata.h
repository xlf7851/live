#pragma once
#include "../base/fmtobj.h"
#include "stockArray.h"

#define STOCK_DATA_TYPE_NOW			0x00000000
#define STOCK_DATA_TYPE_MINUTE		0x00001000
#define STOCK_DATA_TYPE_TRACE		0x00002000
#define STOCK_DATA_TYPE_DAY			0x00003000
#define STOCK_DATA_TYPE_NONE		0x0000FFFF

#define STOCK_DATA_BASE_TYPE_MASK	0x0000F000
#define STOCK_DATA_BASE_TYPE(x)		(x&STOCK_DATA_BASE_TYPE_MASK)
#define IS_STOCK_DATA_TYPE_NONE(x)	(x==STOCK_DATA_TYPE_NONE)

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

	class IStockDataArray
	{
	public:
		virtual int GetStockDataSize() = 0;
		virtual int ReadStockDataFromBuf(const char* buf, int len) = 0;
		virtual int WriteStockDataToBuf(xlf::CBuffer& buf) = 0;
		virtual void ClearStockDataArray() = 0;
		virtual void AppendStockDataArray(const IStockDataArray* pArray) = 0;
		virtual void CloneStockDataArray(const IStockDataArray* pArray) = 0;
		virtual void GetStockDataType() = 0;
		virtual LPVOID GetInterface(LPCTSTR pstrName) = 0;
	};


	class DayDataArray : public xlf::FmtObjArray<_day_data_item_t>
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



	class MinuteDataArray : public xlf::FmtObjArray<_minute_data_node_t>
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

	


	

	class StockTypeDataNode
	{
	public:
		StockTypeDataNode();
		virtual ~StockTypeDataNode();

		virtual void Init(uint32 u32Type);
		uint32 GetStockDataType();

		// 数据存取接口
		IStockDataArray* GetStockDataArray(bool bNew = false);

	protected:
		uint32 m_u32StockDataType;
		IStockDataArray* m_data;
	};

	class StockDataNode
	{
	public:
		typedef std::vector<StockTypeDataNode*> _data_container_t;
		typedef _data_container_t::iterator _data_iterator_t;

	public:
		StockDataNode();
		virtual ~StockDataNode();

		void Relase();
		void Init(const Stock& code);
		Stock GetStock() const;

		IStockDataArray* GetStockDataArray(uint32 u32Type, bool bNew = false);
		StockTypeDataNode* GetStockTypeDataNode(uint32 uType, bool bNew = false);
	protected:
		Stock m_stock;
		_data_container_t m_data;

	};

	class StockDataMarketNode
	{
	public:
		typedef std::map<Stock, StockDataNode*> _data_container_t;
		typedef _data_container_t::iterator _data_iterator_t;

	public:
		StockDataMarketNode();
		virtual ~StockDataMarketNode();

		void Release();
		void Init(uint32 uMarket);
		uint32 GetMarket();

		IStockDataArray* GetStockDataArray(const Stock& code, uint32 u32Type, bool bNew = false);
		StockDataNode* GetStockDataNode(const Stock& stock, bool bNew = false);
	
	protected:
		uint32 m_uMarket;
		_data_container_t m_data;
	};

	class StockDataPool
	{
	public:
		typedef std::vector<StockDataMarketNode*> _data_container_t;
		typedef _data_container_t::iterator _data_iterator_t;
	public:
		StockDataPool();
		~StockDataPool();

		// 获取对象实例
		static StockDataPool* Instance();
		
		void Release();
	

		// 获取数据接口
		IStockDataArray* GetStockDataArray(const StockCode& code, uint32 u32StockDataType, bool bNew = false);
		

		
	protected:
		StockDataMarketNode* GetMarketNode(uint32 uMarket, bool bNew = false);

	protected:
		_data_container_t m_dataContaienr; // 数据容器
	};
}