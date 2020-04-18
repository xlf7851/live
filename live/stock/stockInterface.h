#pragma once

typedef void* HSynCode;

// 代码表事件
#define STOCK_CHANGE_EVENT_RESET		0x0001		// 重置代码表
#define STOCK_CHANGE_EVENT_CURRENT		0x0002		// 切换选中的代码
#define STOCK_CHANGE_EVENT_SCROLL		0x0004		// 滚动了代码表

// 代码表滚动属性
#define STOCK_CODELIST_SCROLL_MASK			0x000000FF
#define STOCK_CODELIST_SCROLL_POS_MASK		0x00000007		// 滚动位置掩码
#define STOCK_CODELIST_SCROLL_POS_ONE		0x00000000		// 相对位置滚动：单个代码滚动
#define STOCK_CODELIST_SCROLL_POS_PAGE		0x00000001		// 相对位置滚动：按页滚动
#define STOCK_CODELIST_SCROLL_POS_ABS		0x00000002		// 绝对位置滚动
#define STOCK_CODELIST_SCROLL_CYCLE			0x00000008		// 可循环滚动
#define STOCK_CODELIST_SCROLL_LAST_PAGE		0x00000010		// 滚动到最后一页
#define STOCK_CODELIST_SCROLL_BYCODE		0x00000020		// 滚动到指定代码

#define GET_STOCK_CODELIST_SCROLL_POS(x)    (x&STOCK_CODELIST_SCROLL_POS_MASK)
#define IS_STOCK_CODELIST_SCROLL_CYCLE(x)	((x&STOCK_CODELIST_SCROLL_CYCLE)?true:false)

class IStockChangeEventHandler
{
public:
	virtual void OnStockChangeEvent(HSynCode hSynCode, uint32 uCodeListID,  uint64 uEvent) = 0;
};

namespace stock_wrapper
{
	// code interface
	// 构造和析构
	HSynCode NewSynCode(uint32 id=0);
	void ReleaseSynCode(HSynCode hSynCode);
	// 代码表id
	void SetCodeDataID(HSynCode hSynCode, uint32 id);
	uint32  GetCodeDataID(HSynCode hSynCode);
	// 代码访问函数
	int GetCodeSize(HSynCode hSynCode);		// 获取代码个数
	StockCode& GetCodeAt(HSynCode hSynCode, int index);
	bool GetCodeAt(HSynCode hSynCode, int index, StockCode& code);
	StockArray& GetCodeArray(HSynCode hSynCode);
	void GetCodeArray(HSynCode hSyncode, StockArray& ayCode);
	void SetCodeList(HSynCode hSynCode, StockArray& ayCode, uint64 uMode = 0, int nSeltectedIndex = -1, StockCode* pSelectedCode = nullptr);
	// 代码滚动相关
	int GetCodeScrollIndex(HSynCode hSynCode);	// 获取当前滚动索引，即当前页第一个代码索引
	int GetCodeCurIndex(HSynCode hSynCode);		// 获取当前选择的代码
	int GetCodePage(HSynCode hSynCode);		// 获取页面的代码个数
	void SetCodePage(HSynCode hSynCode, int nPage);
	void GetCodeScrollInfo(HSynCode hSynCode, int* nScroll, int* nPage=nullptr, int* nTotal=nullptr);

	// 滚动分为相对位置滚动和绝对位置滚动以及一些特殊的指定滚动(滚动第一页，滚动到醉后一页等等)，另外还有循环滚动属性、滚动到指定代码
	void ScrollCode(HSynCode hSynCode, uint32 uMode, int nPos, StockCode* stockCode = nullptr);
	// 代码事件
	void RegisterStockChangeEventHandler(HSynCode hSynCode, IStockChangeEventHandler* pHandler);
	void UnRegisterStockChangeEventHandler(HSynCode hSynCode, IStockChangeEventHandler* pHandler);
}
