#include "stdafx.h"
#include "hqDataQuery.h"
#include "../base/buffer.h"
#include "../base/utl.h"
#include <tinyxml2.h>
#include "../global/GlobalFuncton.h"
#include <algorithm>

namespace stock_wrapper
{
	CHqDataItemManager::CHqDataItemManager()
	{

	}

	CHqDataItemManager::~CHqDataItemManager()
	{
		_Release();
	}

	CHqDataItemManager& CHqDataItemManager::GetManager()
	{
		static CHqDataItemManager s_obj;
		static bool s_bInit = false;
		if (!s_bInit)
		{
			s_bInit = true;
			_tstring strPath = global_funciton::GetConfigDir();
			strPath += _T("stockdata\\dataitem.xml");
			s_obj.Init(strPath.c_str());
		}

		return s_obj;
	}

	void CHqDataItemManager::Init(LPCTSTR lpszConfigPath)
	{
		tinyxml2::XMLDocument doc;
		tinyxml2::XMLError ret = doc.LoadFile(lpszConfigPath);
		if (ret != 0)
		{
			return;
		}

		tinyxml2::XMLElement* root = doc.RootElement();
		if (root == NULL)
		{
			return;
		}

		for (tinyxml2::XMLElement* cur = root->FirstChildElement(); cur ; cur = cur->NextSiblingElement())
		{
			const char* id = cur->Attribute(_T("id"));
			const char* name = cur->Attribute(_T("name"));
			if (id == nullptr || name == nullptr)
			{
				continue;
			}

			_data_item_t* pNew = new _data_item_t;
			pNew->m_strName = name;
			pNew->m_dwID = _ttoi(id);
			m_data.push_back(pNew);
		}
	}

	DWORD CHqDataItemManager::FindIDByName(LPCTSTR lpszName)
	{
		for (int i = 0; i < m_data.size(); i++)
		{
			if (m_data[i]->m_strName.compare(lpszName) == 0)
			{
				return m_data[i]->m_dwID;
			}
		}

		return 0;
	}

	_tstring CHqDataItemManager::FindNameByID(DWORD dwID)
	{
		_tstring rst;
		_data_item_t* pFind = _FindItem(dwID);
		if (pFind)
		{
			rst = pFind->m_strName;
		}

		return rst;
	}

	

	CHqDataItemManager::_data_item_t* CHqDataItemManager::_FindItem(DWORD dwID)
	{
		_data_item_t* pFind = NULL;
		_data_item_t item;
		item.m_dwID = dwID;
 		std::pair<_data_iterator, _data_iterator> rst = std::equal_range(m_data.begin(), m_data.end(), &item, _CompareItem);
		if (rst.first != rst.second)
		{
			pFind = *rst.first;
		}

		return pFind;
	}

	void CHqDataItemManager::_Release()
	{
		for (int i = 0; i < m_data.size(); i++)
		{
			delete m_data[i];
		}

		m_data.clear();
	}

	static DWORD NewHqQueryID()
	{
		static DWORD s_begin = 0x100;
		DWORD dwNew = s_begin;
		s_begin++;
		if (s_begin & 0x80000000)
		{
			s_begin = 0x100;
		}

		return dwNew;
	}

	struct hq_data_xml_head_t
	{
		char m_szFlag[8];
		uint32 m_ulHeadSize;
		uint32 m_ulPacklen;
		DWORD  m_dwStyle;
		hq_data_xml_head_t()
		{
			memset(this, 0, sizeof(hq_data_xml_head_t));
			m_ulHeadSize = sizeof(hq_data_xml_head_t);
			strcpy_s(m_szFlag, "hxml1.0");
		}
	};

	DWORD QueryHqData(const StockArray& codes, const std::vector<DWORD> vcDataID, DWORD dwPeriod, long lBegin /* = 0 */, long lEnd /* = 0 */, void* pContext /* = nullptr */)
	{
		
		do 
		{
			const char* declaration = _T("<?xml version=\"1.0\" encoding=\"gbk\"?>");
			tinyxml2::XMLDocument doc;
			tinyxml2::XMLError ret = doc.Parse(declaration);
			if (ret != 0) {
				break;
			}

			// root request
			tinyxml2::XMLElement* root = doc.NewElement(_T("request"));
			doc.InsertEndChild(root);

			// request attribute
			root->SetAttribute(_T("period"), (int)dwPeriod);
			root->SetAttribute(_T("force"), 1);
			DWORD dwReqID = NewHqQueryID();
			root->SetAttribute(_T("uid"), (int)dwReqID);

			// code
			tinyxml2::XMLElement* stockcode = doc.NewElement(_T("stockcode"));
			root->InsertEndChild(stockcode);
// 			for (int i = 0; i < codes.GetSize(); i++)
// 			{
// 				tinyxml2::XMLElement* codeItem = doc.NewElement(_T("item"));
// 				stockcode->InsertEndChild(codeItem);
// 				//codeItem->SetText(codes.GetAt(i));
// 			}

			// datetime
			tinyxml2::XMLElement* dataTime = doc.NewElement(_T("datetime"));
			dataTime->SetAttribute(_T("begian"), (int)lBegin);
			dataTime->SetAttribute(_T("end"), (int)lEnd);
			root->InsertEndChild(dataTime);

			// dataitem
			tinyxml2::XMLElement* dataType = doc.NewElement(_T("datatype"));
			root->InsertEndChild(dataType);
			
			CHqDataItemManager& mgr = CHqDataItemManager::GetManager();
			for (int i = 0; i < vcDataID.size(); i++)
			{
				_tstring strName = mgr.FindNameByID(vcDataID[i]);
				if (!strName.empty())
				{
					tinyxml2::XMLElement* datatypeitem = doc.NewElement(_T("item"));
					datatypeitem->SetAttribute(_T("name"), strName.c_str());
					dataType->InsertEndChild(datatypeitem);
				}
			}

// 			std::string strFile = global_funciton::GetConfigDir();
// 			strFile += _T("stockdata\\test.xml");
// 			doc.SaveFile(strFile.c_str());
		
			tinyxml2::XMLPrinter printer;
			doc.Accept(&printer);
			std::string strData = printer.CStr();

			CHqDataQueryHandler::GetHqDataQueryHandler().Request(strData, dwReqID, pContext);

			return dwReqID;

		} while (false);

		return 0;
	}

	CHqDataQueryHandler& CHqDataQueryHandler::GetHqDataQueryHandler()
	{
		static CHqDataQueryHandler s_obj;
		return s_obj;
	}

	CHqDataQueryHandler::CHqDataQueryHandler()
	{
		m_hCopyDataDstHwnd = NULL;
		m_hCopyDataSrcHwnd = NULL;
	}

	void CHqDataQueryHandler::SetCopyDataDstHwnd(HWND hWnd)
	{
		m_hCopyDataDstHwnd = hWnd;
	}

	bool CHqDataQueryHandler::Request(const std::string& strData, DWORD dwReqID, void* pContext)
	{
		if (m_hCopyDataDstHwnd == NULL)
		{
			return false;
		}

		if (m_hCopyDataSrcHwnd == nullptr)
		{
			m_hCopyDataSrcHwnd = global_funciton::GetMianHwnd();
			if (m_hCopyDataSrcHwnd == nullptr)
			{
				return false;
			}
		}

		hq_data_xml_head_t header;
		header.m_ulPacklen = header.m_ulHeadSize + strData.size();
		header.m_dwStyle = 0;
		static xlf::CBuffer buf;
		buf.Clear();
		buf.Append(&header, header.m_ulHeadSize);
		buf.Append(strData.c_str(), strData.size());
		static COPYDATASTRUCT st;
		st.lpData = buf.GetBuffer();
		st.cbData = buf.GetSize();
		st.dwData = 0x0c;// | 0x80000000;
		::SendMessage(m_hCopyDataDstHwnd, WM_COPYDATA, (WPARAM)m_hCopyDataSrcHwnd, (LPARAM)&st);
		
		return true;
	}

	bool CHqDataQueryHandler::OnCopyData(WPARAM wParam, LPARAM lParam)
	{
		if ((HWND)wParam != m_hCopyDataDstHwnd)
		{
			return false;
		}

		COPYDATASTRUCT * pSt = (COPYDATASTRUCT*)lParam;
		hq_data_xml_head_t* header = (hq_data_xml_head_t*)pSt->lpData;

		return true;
	}
}