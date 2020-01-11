#pragma once
#include "stockArray.h"
namespace stock_wrapper
{
	class CMarketStock
	{
	public:
		CMarketStock();

		void Parse(LPCTSTR lpszText);

		DWORD GetMarket();
	protected:
		DWORD m_dwMarket;
		StockArray m_ayCode;
	};

	class CMarket
	{
	public:
		typedef std::vector<CMarketStock*> _market_array_t;
	public:
		CMarket();
		~CMarket();

	protected:
		_market_array_t m_market;
	};
}
