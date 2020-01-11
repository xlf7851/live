#pragma once
#include "buffer.h"
namespace http_utl
{
	/* http请求，通过消息通知数据到达
	 * 1、hNotify为接受消息的窗口
	 * 2、请求函数返回值为请求id
	 * 3、消息的wparam为请求id，lparam为_http_notify_param结构指针，处理消息的窗口要负责delete
	 */
	
	struct _http_notify_param
	{
		xlf::CBuffer m_buf;
		int32 m_code;
		void* m_pContext;

		_http_notify_param()
		{
			m_code = -1;
			m_pContext = nullptr;
		}
	};
	class HttpRequester
	{
	public:
		uint32 Get(const char* url, HWND hNotify, void* pContext = nullptr);
		uint32 Post(const char* url, const char* post, int len, HWND hNotify, void* pContext = nullptr);
	};

	/* http请求的处理池，使用主窗口作为消息处理，可以通过这个对象注册回调函数
	*/

	class IHttpHandler
	{
	public:
		virtual bool OnReveiveData(uint32 id, uint32 code, const char* buf, int len, void* pContext) = 0;
	};

	class HttpHandlerPool
	{
	public:
		HttpHandlerPool();
		virtual ~HttpHandlerPool();

		void Register(IHttpHandler* pHandler);
		void UnRegister(IHttpHandler* pHandler);
		void OnHandlerMsg(WPARAM wParam, LPARAM lParam);

	protected:
		void Clear();
	protected:
		std::set<IHttpHandler*> m_handlers;

	};

	inline HttpHandlerPool* GetHttpHandlerPool()
	{
		static HttpHandlerPool s_obj;
		return &s_obj;
	}
}

