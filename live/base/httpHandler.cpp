#include "stdafx.h"
#include "httpHandler.h"
#include <iostream>
#include <sstream>
#include <curl.h>

namespace http_utl
{
	static uint32 g_request_id = 0x0100;
	static uint32 _get_new_request_id()
	{
		uint32 ret = g_request_id;
		g_request_id++;
		if (g_request_id & 0x80000000)
		{
			g_request_id = 0x0100;
		}

		return ret;
	}

	struct _http_thread_parma 
	{
		uint32 m_id;
		std::string m_url;
		HWND m_hNofity;
		xlf::CBuffer m_post;
		void* m_pContext;

		_http_thread_parma()
		{
			m_id = 0;
			m_hNofity = nullptr;
			m_pContext = nullptr;
		}
	};

	size_t receive_data(void *buffer, size_t size, size_t nmemb, xlf::CBuffer* buf)
	{
		size_t nSize = size * nmemb;
		if (nSize > 0)
		{
			buf->Append((unsigned char*)buffer, nSize);
		}

		return nSize;
	}

	static DWORD WINAPI _request_threa_func(void* p)
	{
		_http_thread_parma* param = (_http_thread_parma*)p;
		if (nullptr == param)
		{
			return 0;
		}

	
		_http_notify_param* notifyParma = new _http_notify_param;
		curl_slist *headers = NULL;
	//	headers = curl_slist_append(headers, "'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/64.0.3282.140 Safari/537.36 Edge/17.17134'");
		CURL* curl = curl_easy_init();    // ³õÊ¼»¯
		if (curl)
		{
		//	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);// 
			curl_easy_setopt(curl, CURLOPT_URL, param->m_url.c_str());
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, receive_data);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, &(notifyParma->m_buf));
			char* pPost = (char*)param->m_post.GetBuffer();
			int postLen = param->m_post.GetSize();
			if (pPost && postLen > 0)
			{
				curl_easy_setopt(curl, CURLOPT_POSTFIELDS, pPost);
				curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, postLen);
			}
			CURLcode res = curl_easy_perform(curl);  
			notifyParma->m_code = res;
			curl_easy_cleanup(curl);
		}

		WPARAM wParam = param->m_id;
		HWND hNotify = param->m_hNofity;
		notifyParma->m_pContext = param->m_pContext;
		delete param;
		if (hNotify)
		{
			::PostMessage(hNotify, UWM_HTTP_REVEIVE, wParam, (LPARAM)notifyParma);
		}

		return 0;
	}

	uint32 HttpRequester::Get(const char* url, HWND hNotify /* = nullptr */, void* pContext)
	{
		uint32 id = _get_new_request_id();
		_http_thread_parma* param = new _http_thread_parma;
		param->m_url = url;
		param->m_hNofity = hNotify;
		param->m_id = id;
		param->m_pContext = pContext;
		
		DWORD dwThreadID = 0;
		HANDLE hThread = ::CreateThread(NULL, 0, _request_threa_func, param, 0, &dwThreadID);
		if (hThread)
		{
			CloseHandle(hThread);
		}

		return id;
	}

	uint32 HttpRequester::Post(const char* url, const char* post, int len, HWND hNotify /* = nullptr */, void* pContext)
	{
		uint32 id = _get_new_request_id();
		_http_thread_parma* param = new _http_thread_parma;
		param->m_url = url;
		param->m_hNofity = hNotify;
		param->m_id = id;
		param->m_pContext = pContext;
		if (post && len > 0)
		{
			param->m_post.Append((unsigned char*)post, len);
		}
		
		DWORD dwThreadID = 0;
		HANDLE hThread = ::CreateThread(NULL, 0, _request_threa_func, param, 0, &dwThreadID);
		if (hThread)
		{
			CloseHandle(hThread);
		}
		return id;
	}

	HttpHandlerPool::HttpHandlerPool()
	{

	}

	HttpHandlerPool::~HttpHandlerPool()
	{
		Clear();
	}

	void HttpHandlerPool::Clear()
	{
		m_handlers.clear();
	}

	void HttpHandlerPool::Register(IHttpHandler* pHandler)
	{
		m_handlers.insert(pHandler);
	}

	void HttpHandlerPool::UnRegister(IHttpHandler* pHandler)
	{
		m_handlers.erase(pHandler);
	}

	void HttpHandlerPool::OnHandlerMsg(WPARAM wParam, LPARAM lParam)
	{
		uint32 id = wParam;
		_http_notify_param* pParam = (_http_notify_param*)lParam;
		if (id == 0 || pParam == nullptr)
		{
			return;
		}

		for (std::set<IHttpHandler*>::iterator it = m_handlers.begin(); it != m_handlers.end(); it++)
		{
			if (*it && (*it)->OnReveiveData(id, pParam->m_code, (const char*)pParam->m_buf.GetBuffer(), pParam->m_buf.GetSize(), pParam->m_pContext))
			{
				break;
			}
		}

		delete pParam;
	}
}