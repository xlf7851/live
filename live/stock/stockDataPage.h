#pragma once
#include "stockDataTable.h"

typedef  CBasePageUI _StockDataPageUIBase;
namespace stock_wrapper
{
	class MarketDayData;
}

class CStockDataTableUI;
class CStockDataPageUI : public _StockDataPageUIBase
{
	DECLARE_DUICONTROL(CStockDataPageUI)
public:
	CStockDataPageUI();
	~CStockDataPageUI();

	virtual LPCTSTR GetClass() const;
	virtual LPVOID GetInterface(LPCTSTR pstrName);

	virtual void OnNotify(TNotifyUI& msg);
	virtual void InitPage();
protected:
	void AddLog(LPCTSTR lpszLog);
	void FindGetDataWindow();
	void OnLoadData();

protected:
	void LoadPoolData(LPCTSTR lpszFile);
	void ShowDayData(stock_wrapper::MarketDayData& data, const stock_wrapper::Stock& stock);
	void UpdateStockText(const stock_wrapper::Stock& stock);
	void OnTestDayData();
	void OnTestMinData();
protected:
	HWND m_dataHwnd;
	CRichEditUI* m_pLogEdit;

	CStockDataTableUI* m_pDayDataTable;
	CLabelUI* m_pStockLabel;
};