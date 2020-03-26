#pragma once
#include "../base/fmtobj.h"
#include "stockArray.h"

namespace stock_wrapper
{
	DWORD QueryHqData(const StockArray& codes, const std::vector<DWORD> vcDataID, DWORD dwPeriod, long lBegin = 0, long lEnd = 0, void* pContext = nullptr);

	class CHqDataItemManager
	{
	public:
		struct _data_item_t 
		{
			DWORD  m_dwID;
			_tstring m_strName;
		};
		typedef std::vector<_data_item_t*> _data_container_t;
		typedef _data_container_t::iterator _data_iterator;
	public:
		CHqDataItemManager();
		virtual ~CHqDataItemManager();

		void Init(LPCTSTR lpszConfigPath);
		_tstring FindNameByID(DWORD dwID);
		DWORD FindIDByName(LPCTSTR lpszName);

		static CHqDataItemManager& GetManager();
	protected:
		_data_item_t* _FindItem(DWORD dwID);
		void _Release();
		static bool _CompareItem(_data_item_t* f, _data_item_t* l)
		{
			return f->m_dwID < l->m_dwID;
		}
	protected:
		std::vector<_data_item_t*> m_data; // sort by id
	};

	class CHqDataQueryHandler
	{
	public:
		static CHqDataQueryHandler& GetHqDataQueryHandler();

	public:
		CHqDataQueryHandler();
		void SetCopyDataDstHwnd(HWND hWnd);
		void SetCopyDataSrcHwnd(HWND hWnd);

		bool Request(const std::string& strData, DWORD dwReqID, void* pContext);
		bool OnCopyData(WPARAM wParam, LPARAM lParam);

	protected:
		HWND m_hCopyDataDstHwnd;
		HWND m_hCopyDataSrcHwnd;

	};
}