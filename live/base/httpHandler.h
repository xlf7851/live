#pragma once
#include "buffer.h"
namespace http_utl
{
	/* http����ͨ����Ϣ֪ͨ���ݵ���
	 * 1��hNotifyΪ������Ϣ�Ĵ���
	 * 2������������ֵΪ����id
	 * 3����Ϣ��wparamΪ����id��lparamΪ_http_notify_param�ṹָ�룬������Ϣ�Ĵ���Ҫ����delete
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

	/* http����Ĵ���أ�ʹ����������Ϊ��Ϣ��������ͨ���������ע��ص�����
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

