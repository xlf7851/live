#pragma once

// 定义一些全局变量

namespace stock_wrapper
{
	class StockDataPool;
}

namespace xlf
{
	
	class CGlobalData
	{
	public:
		CGlobalData();
		~CGlobalData();

		HWND GetHqDataIpcSrcHwnd() { return m_hHqDataIpcSrcHwnd; }
		void SetHqDataIpcSrcHwnd(HWND hHwnd) { m_hHqDataIpcSrcHwnd = hHwnd; }

		HWND GetHqDataIpcDestHwnd() { return m_hHqDataIpcDestHwnd; }
		void SetHqDataIpcDestHwnd(HWND hHwnd) { m_hHqDataIpcDestHwnd = hHwnd; }

		stock_wrapper::StockDataPool* GetStockDataPool() { return m_pStockDataPool; }
	public:
		void Init();
		void DoGlobalFunction(LPCTSTR lpszFunc);
	protected:
		void DoSetHqDataIpcDestHwnd();
		void InitStockDataPool();
	protected:
		HWND m_hHqDataIpcSrcHwnd;		
		HWND m_hHqDataIpcDestHwnd;
		stock_wrapper::StockDataPool* m_pStockDataPool;
	};
}