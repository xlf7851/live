#pragma once

class CScreenShotUI : public CControlUI
{
public:
	CScreenShotUI();
	~CScreenShotUI();

	void PaintBkImage(HDC hDC);
	void SetScreenShotBitmap(HBITMAP hBitmap, int& nCx, int& nCy);

protected:
	HBITMAP m_hScreenBitmap;
};

class CScreenShotWnd : public WindowImplBase
{
public:
	CScreenShotWnd();
	~CScreenShotWnd();

	CDuiString GetSkinFile();
	LPCTSTR GetWindowClassName() const;

	void Work(HWND hParentWindow);
	void InitWindow();
protected:
	HBITMAP ScreenShotBitmap();
	bool CheckCreateWindow(HWND hParentWindow);
	void AdjustWindow(int cx, int cy);

protected:
	CScreenShotUI* m_pScreenShot;
};