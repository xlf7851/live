#pragma once
#include "stockArray.h"
#include "../base/httpHandler.h"
namespace stock_wrapper
{
	struct BLOCK_ASKER_RET_LPARAM 
	{
		stock_wrapper::StockArray m_ayCode;
		int m_nErrorCode;
		DWORD m_dwContext;
	};
	class IBlockAskerRecever
	{
	public:
		virtual bool BlockStockRevever(void* pContext, int nErrorCode, const stock_wrapper::StockArray& ayCode) = 0;
	};

	class BlockAsker : public http_utl::IHttpHandler
	{
	public:
		BlockAsker();
		virtual ~BlockAsker();

		void Init(HWND hNotify, IBlockAskerRecever* hanler = nullptr);
		bool Ask(LPCTSTR lpszQuery, void* pContext = nullptr);
		virtual bool OnReveiveData(uint32 id, uint32 code, const char* buf, int len, void* pContext);

	protected:
		uint32 m_uReqID;
		bool m_bInitRegister;
		HWND m_hNotify;
		IBlockAskerRecever* m_handler;

	};
}
