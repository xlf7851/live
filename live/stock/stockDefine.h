#pragma once

// block group id define
#define BLOCK_GROUP_ID_SELF			0x0001
#define BLOCK_GROUP_ID_STRATEGY 	0x0002
#define BLOCK_GROUP_BEGIN			0x0010
#define BLOCK_GROUP_END				0x8FFF


// block id define
#define BLOCK_ID_SELF_BEGIN			0x0001
#define BLOCK_ID_SELF_END			0x8fff

#define STOCK_CONFIG_ROOT_PATH				(_T("stock\\"))
#define STOCK_CONFIG_BLOCK_CACHE_PATH		(_T("stock\\blockcache.dat"))
#define STOCK_CONFIG_BLOCK_GROUP_PATH		(_T("stock\\blockgroup.dat"))



struct _block_draw_item_it
{
	uint32 m_u32BlockID;
	std::string m_strName;
	int m_nStockCnt;
	uint32 m_uParam;
	_block_draw_item_it()
	{
		m_u32BlockID = 0;
		m_uParam = 0;
	}
};