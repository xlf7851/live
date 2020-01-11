#include "stdafx.h"
#include "blockAsk.h"
#include "../base/buffer.h"
#include "../base/utl.h"
#include "../global/GlobalFuncton.h"
#include <json.h>
#include "../base/jsonutl.h"

namespace stock_wrapper
{
	BlockAsker::BlockAsker()
	{
		m_uReqID = 0;
		m_bInitRegister = false;
		m_hNotify = nullptr;
	}

	BlockAsker::~BlockAsker()
	{
		if (m_bInitRegister)
		{
			http_utl::HttpHandlerPool* pPool = http_utl::GetHttpHandlerPool();
			if (pPool)
			{
				pPool->UnRegister(this);
			}
		}
	}

	void BlockAsker::Init(HWND hNotify, IBlockAskerRecever* hanler /* = nullptr */)
	{
		m_hNotify = hNotify;
		m_handler = hanler;
	}

	bool BlockAsker::Ask(LPCTSTR lpszQuery, void* pContext /* = nullptr */)
	{
		std::string strQuery;
		if (lpszQuery)
		{
			strQuery = lpszQuery;
		}
		m_uReqID = 0;

		if (strQuery.empty())
		{
			return false;
		}

		http_utl::HttpHandlerPool* pPool = http_utl::GetHttpHandlerPool();
		if (pPool)
		{
			if (!m_bInitRegister)
			{
				m_bInitRegister = true;
				pPool->Register(this);

			}
			
			HWND hWnd = global_funciton::GetMianHwnd();
			http_utl::HttpRequester requester;
			std::string url = "http://zx.10jqka.com.cn/i/index/index/app/19/?num=600&query=";
			xlf::CBuffer buf;
			int nLen = xlf::AnsiToUtf8(strQuery.c_str(), strQuery.size(), buf);
			if (nLen > 0)
			{
				std::string query((const char*)buf.GetBuffer(), nLen);
				url += xlf::UrlEncode(query);
			}

			m_uReqID = requester.Get(url.c_str(), hWnd, pContext);

			return  m_uReqID != 0;
		}

		return false;
	}

	bool BlockAsker::OnReveiveData(uint32 id, uint32 code, const char* buf, int len, void* pContext)
	{
		if (id != m_uReqID)
		{
			return false;
		}
		m_uReqID = 0;

		BLOCK_ASKER_RET_LPARAM* param = new BLOCK_ASKER_RET_LPARAM;
		param->m_nErrorCode = -1;
		param->m_dwContext = (DWORD)pContext;
		do 
		{
			if (code != 0)
			{
				break;
			}

			Json::Value jsRoot;
			Json::Reader reader;
			std::string strData(buf, len);
			if (!reader.parse(strData, jsRoot) || jsRoot.isNull() || !jsRoot.isObject())
			{
				break;
			}

			param->m_nErrorCode = json_utl::json_to_int(jsRoot["errorcode"], -1);
			if (param->m_nErrorCode != 0)
			{
				break;
			}

			Json::Value& jsResult = jsRoot["result"];
			if (jsResult.isNull() || !jsResult.isObject())
			{
				break;
			}

			Json::Value jsData = jsResult["data"];
			if (jsData.isNull() || !jsData.isArray())
			{
				break;
			}
			string strTmp;
			int nSize = jsData.size();
			for (int i = 0; i < nSize; i++)
			{
				Json::Value& item = jsData[i];

				strTmp = json_utl::json_to_string(item);
				if (strTmp.empty())
				{
					continue;
				}

				param->m_ayCode.Add(strTmp.c_str());
			}
		} while (FALSE);

		if (m_hNotify)
		{
			::PostMessage(m_hNotify, UWM_BLOCK_ASK_RET, (WPARAM)this, (LPARAM)param);
		}
		else 
		{
			if (m_handler)
			{
				m_handler->BlockStockRevever(pContext, param->m_nErrorCode, param->m_ayCode);
			}

			delete param;
			
		}

		return true;
	}
}