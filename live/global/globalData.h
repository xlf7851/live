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

		
	public:
		void Init();
		void DoGlobalFunction(LPCTSTR lpszFunc);
	protected:
		void DoSetHqDataIpcDestHwnd();
	protected:
		HWND m_hHqDataIpcSrcHwnd;		
		HWND m_hHqDataIpcDestHwnd;
		
	};
}