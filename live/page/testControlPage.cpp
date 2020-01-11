#include "stdafx.h"
#include "testControlPage.h"
#include "../screenShot/screenShot.h"
#include "../mathGraphic/glwindow.h"


IMPLEMENT_DUICONTROL(CTestControlPageUI)

CTestControlPageUI::CTestControlPageUI()
{

}

CTestControlPageUI::~CTestControlPageUI()
{

}

LPCTSTR CTestControlPageUI::GetClass() const
{
	return DUI_CUSTOM_CTRL_CLASS_TestControlPage;
}

LPVOID CTestControlPageUI::GetInterface(LPCTSTR pstrName)
{
	if (_tcsicmp(pstrName, DUI_CUSTOM_CTRL_INTERFACE_TestControlPage) == 0) return static_cast<CTestControlPageUI*>(this);
	return _TestControlPageUIBase::GetInterface(pstrName);
}

void CTestControlPageUI::InitPage()
{
	CPieChartUI* pPie = (CPieChartUI*)control_utl::FindChildByName(this, _T("pieChar"));
	if (pPie)
	{
		DWORD v[8] = { 3.45, 5.65, 6.36, 3.54, 2.36, 7.58, 9.25, 4.26 };
		DWORD dwColor[8] = { 0xffff0000, 0xffbb0000, 0xff990000, 0xff770000, 0xff007700, 0xff009900, 0xff00bb00, 0xff00ff00 };
		for (int i = 0; i < 8; i++)
		{
			pPie->AddDataItem(i, dwColor[i], v[i]);
		}
		pPie->CalcData();
	}
}

void CTestControlPageUI::OnNotify(TNotifyUI& msg)
{
	CDuiString name = msg.pSender->GetName();
	if (msg.sType == DUI_MSGTYPE_CLICK)
	{
		if (name == _T("btnTestPageScreenshots"))
		{
			OnScreenshot();
			return;
		}
		else if (name == _T("btnTestPageShowGL"))
		{
			CGLShowWnd::ShowGLWindow(m_pManager ? m_pManager->GetPaintWindow() : NULL, 800, 600);
			return;
		}
	}
}

void CTestControlPageUI::OnScreenshot()
{
	CScreenShotWnd dlg;
	dlg.Work(m_pManager ? m_pManager->GetPaintWindow() : NULL);
}