#include "stdafx.h"
#include "base64Page.h"
#include "../control/controlGlobalFunc.h"
#include "../base/buffer.h"
#include "../base/utl.h"

IMPLEMENT_DUICONTROL(CBase64PageUI)

CBase64PageUI::CBase64PageUI()
{
	m_pSrcEdit = nullptr;
	m_pDstEdit = nullptr;
}

CBase64PageUI::~CBase64PageUI()
{

}

LPCTSTR CBase64PageUI::GetClass() const
{
	return DUI_CUSTOM_CTRL_CLASS_Base64Page;
}

LPVOID CBase64PageUI::GetInterface(LPCTSTR pstrName)
{
	if (_tcsicmp(pstrName, DUI_CUSTOM_CTRL_INTERFACE_Base64Page) == 0) return static_cast<CBase64PageUI*>(this);
	return _Base64PageUIBase::GetInterface(pstrName);

	
}

void CBase64PageUI::InitPage()
{
	ReadEditControl();
	CPieChartUI* pPie = (CPieChartUI*)control_utl::FindChildByName(this, _T("pieChar"));
	if (pPie)
	{
		DWORD v[8] = { 3.45, 5.65, 6.36, 3.54, 2.36, 7.58, 9.25, 4.26 };
		DWORD dwColor[8] = {0xffff0000, 0xffbb0000, 0xff990000, 0xff770000, 0xff007700, 0xff009900, 0xff00bb00, 0xff00ff00};
		for (int i = 0; i < 8; i++)
		{
			pPie->AddDataItem(i, dwColor[i], v[i]);
		}
		pPie->CalcData();
	}
}

void CBase64PageUI::OnNotify(TNotifyUI& msg)
{
	CDuiString name = msg.pSender->GetName();
	if (msg.sType == DUI_MSGTYPE_CLICK)
	{
		if (name == _T("base64PageBtnEncode"))
		{
			OnEncode();
		}
		else if (name == _T("base64PageBtnDecode"))
		{
			OnDecode();
		}
	}
 }

void CBase64PageUI::ReadEditControl()
{
	if (m_pSrcEdit == nullptr)
	{
		m_pSrcEdit = static_cast<CRichEditUI*>(control_utl::FindChildByName(this, _T("base64PageResultSrc")));
	}

	if (m_pDstEdit == nullptr)
	{
		m_pDstEdit = static_cast<CRichEditUI*>(control_utl::FindChildByName(this, _T("base64PageResultDst")));
	}
}

void CBase64PageUI::OnDecode()
{
	std::string strSrc;
	if (m_pSrcEdit)
	{
		strSrc = m_pSrcEdit->GetText();
	}

	std::string strDst;
	if (!strSrc.empty())
	{
		xlf::CBuffer buf;
		if (xlf::Base64Decode(strSrc.c_str(), strSrc.size(), buf))
		{
			strDst.assign((char*)buf.GetBuffer(), buf.GetSize());
		}
	}
	
	if (m_pDstEdit)
	{
		m_pDstEdit->SetText(strDst.c_str());
	}
	
}

void CBase64PageUI::OnEncode()
{
	std::string strSrc;
	if (m_pSrcEdit)
	{
		strSrc = m_pSrcEdit->GetText();
	}

	std::string strDst;
	if (!strSrc.empty())
	{
		xlf::CBuffer buf;
		if (xlf::Base64Encode(strSrc.c_str(), strSrc.size(), buf))
		{
			strDst.assign((char*)buf.GetBuffer(), buf.GetSize());
		}
	}

	if (m_pDstEdit)
	{
		m_pDstEdit->SetText(strDst.c_str());
	}
}