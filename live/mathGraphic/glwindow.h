#pragma once

struct GLFWwindow;
class CGLUI : public CControlUI
{
	DECLARE_DUICONTROL(CGLUI)
public:
	CGLUI();
	~CGLUI();

	virtual LPCTSTR GetClass() const;
	virtual LPVOID GetInterface(LPCTSTR pstrName);

	GLFWwindow* GetGLWindow();

	void DoInit();
	virtual void SetInternVisible(bool bVisible = true);
	virtual void SetPos(RECT rc, bool bNeedInvalidate);
	void SetGLViewport();
protected:
	void CreateGLWindow();
	void DestoryGLWindow();

private:
	GLFWwindow* m_pGLWindow;
};

class CGLShowWnd : public WindowImplBase
{
public:
	CGLShowWnd();
	~CGLShowWnd();

	CDuiString GetSkinFile();
	LPCTSTR GetWindowClassName() const;

	void InitWindow();
	void OnClick(TNotifyUI& msg);

	static void ShowGLWindow(HWND hParent, int width = 0, int height = 0);

	void OnTestGL();

	GLFWwindow* GetGLWindow();
protected:
	// Èý½ÇÐÎ
	void OnBtnTriangle();


	void InitVBO();
protected:
	CGLUI* m_pGLUI;
	unsigned int m_vbo;
};