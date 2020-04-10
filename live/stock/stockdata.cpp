#include "stdafx.h"
#include "stockdata.h"
#include "../base/buffer.h"
#include "../base/utl.h"

namespace stock_wrapper
{


	
	static IStockDataArray* _CreateStockDataArrayObject(uint32 u32Type)
	{
		return nullptr;
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


	
	

	StockTypeDataNode::StockTypeDataNode()
	{
		m_u32StockDataType = STOCK_DATA_TYPE_NONE;
		m_data = nullptr;
	}

	StockTypeDataNode::~StockTypeDataNode()
	{

	}

	void StockTypeDataNode::Init(uint32 u32StockDataType)
	{
		m_u32StockDataType = u32StockDataType;
	}

	uint32 StockTypeDataNode::GetStockDataType()
	{
		return m_u32StockDataType;
	}

	IStockDataArray* StockTypeDataNode::GetStockDataArray(bool bNew)
	{
		if (m_data == nullptr && bNew)
		{
			m_data = _CreateStockDataArrayObject(m_u32StockDataType);
		}
		return m_data;
	}


	StockDataNode::StockDataNode()
	{
		
	}

	StockDataNode::~StockDataNode()
	{
		Relase();
	}

	void StockDataNode::Relase()
	{

	}

	void StockDataNode::Init(const Stock& code)
	{
		m_stock = code;
	}

	Stock StockDataNode::GetStock() const
	{
		return m_stock;
	}

	IStockDataArray* StockDataNode::GetStockDataArray(uint32 uType, bool bNew)
	{
		StockTypeDataNode* pNode = GetStockTypeDataNode(uType, bNew);
		return pNode ? pNode->GetStockDataArray(bNew) : nullptr;

	}

	StockTypeDataNode* StockDataNode::GetStockTypeDataNode(uint32 uType, bool bNew)
	{
		StockTypeDataNode* pFind = nullptr;
		for (_data_iterator_t it = m_data.begin(); it != m_data.end(); it++)
		{
			StockTypeDataNode* p = *it;
			if (p && p->GetStockDataType() == uType)
			{
				pFind = p;
				break;
			}
		}

		if (pFind == nullptr && bNew)
		{
			pFind = new StockTypeDataNode;
			pFind->Init(uType);
			m_data.push_back(pFind);
		}

		return pFind;
	}



	StockDataMarketNode::StockDataMarketNode()
	{
		m_uMarket = 0;
	}

	StockDataMarketNode::~StockDataMarketNode()
	{
		Release();
	}

	void StockDataMarketNode::Init(uint32 uMarket)
	{
		m_uMarket = uMarket;
	}

	uint32 StockDataMarketNode::GetMarket()
	{
		return m_uMarket;
	}

	void StockDataMarketNode::Release()
	{

	}

	IStockDataArray* StockDataMarketNode::GetStockDataArray(const Stock& code, uint32 u32Type, bool bNew)
	{
		StockDataNode* pNode = GetStockDataNode(code, bNew);
		return pNode ? pNode->GetStockDataArray(u32Type, bNew) : nullptr;
	}

	StockDataNode* StockDataMarketNode::GetStockDataNode(const Stock& stock, bool bNew)
	{
		StockDataNode* pFind = nullptr;
		_data_iterator_t it = m_data.find(stock);
		if (it != m_data.end())
		{
			pFind = it->second;
		}

		if (pFind == nullptr && bNew)
		{
			pFind = new StockDataNode;
			pFind->Init(stock);
			m_data[stock] = pFind;
		}

		return pFind;
	}



	StockDataPool::StockDataPool()
	{
		
	}

	StockDataPool::~StockDataPool()
	{
		Release();
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

	void StockDataPool::Release()
	{
		for (_data_iterator_t it = m_dataContaienr.begin(); it != m_dataContaienr.end(); it++)
		{
			delete *it;
		}

		m_dataContaienr.clear();
	}

	StockDataMarketNode* StockDataPool::GetMarketNode(uint32 uMarket, bool bNew)
	{
		StockDataMarketNode* pFind = nullptr;
		for (_data_iterator_t it = m_dataContaienr.begin(); it != m_dataContaienr.end(); it++)
		{
			StockDataMarketNode* p = *it;
			if (p && p->GetMarket() == uMarket)
			{
				pFind = p;
				break;
			}
		}
		if (pFind == nullptr && bNew)
		{
			pFind = new StockDataMarketNode;
			pFind->Init(uMarket);
			m_dataContaienr.push_back(pFind);
		}

		return pFind;
	}

	IStockDataArray* StockDataPool::GetStockDataArray(const StockCode& code, uint32 u32StockDataType, bool bNew)
	{
		StockDataMarketNode* pMarketNode = GetMarketNode(u32StockDataType);
		if (pMarketNode)
		{
			return pMarketNode->GetStockDataArray(code.GetStock(), u32StockDataType, bNew);
		}

		return nullptr;
	}
}