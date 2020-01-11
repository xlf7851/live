#include "stdafx.h"
#include "screenShot.h"


CScreenShotWnd::CScreenShotWnd()
{
	m_pScreenShot = NULL;
}

CScreenShotWnd::~CScreenShotWnd()
{
	
}

CDuiString CScreenShotWnd::GetSkinFile()
{
	return _T("xml_empty_common");
}

LPCTSTR CScreenShotWnd::GetWindowClassName() const
{
	return _T("PopupSliderMainWnd");
}

void CScreenShotWnd::Work(HWND hParentWindow)
{
	HBITMAP hBitmap = ScreenShotBitmap();
	if (hBitmap == NULL)
	{
		return;
	}

	if (!CheckCreateWindow(hParentWindow) || m_pScreenShot == NULL)
	{
		DeleteObject(hBitmap);
		return;
	}

	int cx = 0;
	int cy = 0;
	m_pScreenShot->SetScreenShotBitmap(hBitmap, cx, cy);

 	AdjustWindow(cx, cy);

	CenterWindow();
	ShowModal();
}

void CScreenShotWnd::AdjustWindow(int cx, int cy)
{
	ResizeClient(cx, cy);
}

HBITMAP CScreenShotWnd::ScreenShotBitmap()
{
	HWND hDesktop = ::GetDesktopWindow();
	HDC hDC = ::GetDC(hDesktop);
	RECT rect = {0,0,0,0};
	::GetClientRect(hDesktop, &rect);
	HDC hdcMem = ::CreateCompatibleDC(hDC);
	HBITMAP hBitmap = ::CreateCompatibleBitmap(hDC, rect.right, rect.bottom);
	HBITMAP hOldBitmap = (HBITMAP)::SelectObject(hdcMem, hBitmap);

	::BitBlt(hdcMem, 0, 0, rect.right, rect.bottom, hDC, 0, 0, SRCCOPY);

	::SelectObject(hdcMem, hOldBitmap);
	::DeleteDC(hdcMem);
	::ReleaseDC(hDesktop, hDC);

	return hBitmap;
}

bool CScreenShotWnd::CheckCreateWindow(HWND hParentWindow)
{
	if (GetHWND())
	{
		return true;
	}

	return CreateDuiWindow(hParentWindow, _T("ScreenShot")) != NULL;
}

void CScreenShotWnd::InitWindow()
{
	CVerticalLayoutUI* pRoot = static_cast<CVerticalLayoutUI*>(m_pm.GetRoot());
	if (pRoot)
	{
		m_pScreenShot = new CScreenShotUI;
		m_pScreenShot->SetBkColor(0xff666666);
		pRoot->Add(m_pScreenShot);
	}
}

CScreenShotUI::CScreenShotUI()
{
	m_hScreenBitmap = NULL;
}

CScreenShotUI::~CScreenShotUI()
{
	if (m_hScreenBitmap)
	{
		::DeleteObject(m_hScreenBitmap);
		m_hScreenBitmap = NULL;
	}
}

void CScreenShotUI::SetScreenShotBitmap(HBITMAP hBitmap, int& nCx, int& nCy)
{
	if (m_hScreenBitmap)
	{
		::DeleteObject(m_hScreenBitmap);
	}

	m_hScreenBitmap = hBitmap;

	BITMAP bmap;
 	GetObject(m_hScreenBitmap, sizeof(BITMAP), &bmap);
	nCx = bmap.bmWidth;
	nCy = bmap.bmHeight;

	nCx = min(nCx, 800);
	nCy = min(nCy, 600);

}

void CScreenShotUI::PaintBkImage(HDC hDC)
{
	if (NULL == m_hScreenBitmap)
	{
		return;
	}

	RECT rcClient = m_rcItem;
	int cx = rcClient.right - rcClient.left;
	int cy = rcClient.bottom - rcClient.top;



	HDC hdcMem = ::CreateCompatibleDC(hDC);
	HBITMAP hOldBitmap = (HBITMAP)::SelectObject(hdcMem, m_hScreenBitmap);
	::BitBlt(hDC, rcClient.left, rcClient.top, cx, cy, hdcMem, 0, 0, SRCCOPY);
	SelectObject(hdcMem, hOldBitmap);
	DeleteDC(hdcMem);

}