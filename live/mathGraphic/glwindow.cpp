#include "stdafx.h"
#include "glwindow.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "mathBase.h"


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

void framebuffer_pos_callback(GLFWwindow* window, int x, int y)
{
	glViewport(x, y, 0, 0);
}


IMPLEMENT_DUICONTROL(CGLUI)
CGLUI::CGLUI()
{
	m_pGLWindow = nullptr;
}

CGLUI::~CGLUI()
{
	DestoryGLWindow();
}

LPCTSTR CGLUI::GetClass() const
{
	return DUI_CUSTOM_CTRL_CLASS_GL;
}

LPVOID CGLUI::GetInterface(LPCTSTR pstrName)
{
	if (_tcsicmp(pstrName, DUI_CUSTOM_CTRL_INTERFACE_GL) == 0)
	{
		return this;
	}

	return CControlUI::GetInterface(pstrName);
}

void CGLUI::DoInit()
{
	if (m_pManager)
	{
		CreateGLWindow();
	}
}

GLFWwindow* CGLUI::GetGLWindow()
{
	return m_pGLWindow;
}

void CGLUI::CreateGLWindow()
{
	if (m_pGLWindow)
	{
		return;
	}
	if (m_pManager)
	{
		m_pGLWindow = glfwCreateWindowEx(100, 100, "LearnOpenGL", NULL, NULL, (unsigned int)m_pManager->GetPaintWindow());
	}

}

void CGLUI::DestoryGLWindow()
{
	if (nullptr == nullptr)
	{
		return;
	}

	glfwDestroyWindow(m_pGLWindow);
	m_pGLWindow = nullptr;
}

void CGLUI::SetInternVisible(bool bVisible)
{
	CControlUI::SetInternVisible(bVisible);
// 	if (m_pGLWindow)
// 	{
// 		glfwsetwindow
// 	}
// 	::ShowWindow(m_hWnd, bVisible);
}

void CGLUI::SetPos(RECT rc, bool bNeedInvalidate)
{
	CControlUI::SetPos(rc, bNeedInvalidate);
	if (m_pGLWindow)
	{
		glfwSetWindowPosEx(m_pGLWindow, m_rcItem.left, m_rcItem.top, m_rcItem.right - m_rcItem.left, m_rcItem.bottom - m_rcItem.top, SWP_NOZORDER | SWP_NOACTIVATE, 0);
		
	}
	//::SetWindowPos(m_hWnd, NULL, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_NOZORDER | SWP_NOACTIVATE);
}

CGLShowWnd::CGLShowWnd()
{
	m_pGLUI = NULL;
	m_vbo = 0;
}

CGLShowWnd::~CGLShowWnd()
{
	
}

CDuiString CGLShowWnd::GetSkinFile()
{
	return _T("XML_show_gl");
}

LPCTSTR CGLShowWnd::GetWindowClassName() const
{
	return _T("glShowWnd");
}

void CGLShowWnd::InitWindow()
{
	m_pGLUI = static_cast<CGLUI*>(m_pm.FindControl(_T("glWnd")));
}

void CGLShowWnd::OnClick(TNotifyUI& msg)
{
	CDuiString strName = msg.pSender->GetName();
	if (strName == _T("btnTest"))
	{
		OnTestGL();
		return;
	}
	else if (strName == _T("btnTriangle"))
	{
		OnBtnTriangle();
		return;
	}
	WindowImplBase::OnClick(msg);
}


void CGLShowWnd::ShowGLWindow(HWND hParent, int width, int height)
{
	//_TestOpenGL();
	CGLShowWnd wnd;
	wnd.CreateDuiWindow(hParent, _T("glwindow"));
	if (width > 0 || height > 0)
	{
		wnd.ResizeClient(width > 0 ? width : -1, height > 0 ? height : -1);
	}
	
	wnd.CenterWindow();
	wnd.ShowModal();
}

GLFWwindow* CGLShowWnd::GetGLWindow()
{
	return m_pGLUI ? m_pGLUI->GetGLWindow() : NULL;
}
void CGLShowWnd::OnTestGL()
{
	GLFWwindow* pWindow = GetGLWindow();
	if (NULL == pWindow)
	{
		return;
	}

	glfwMakeContextCurrent(pWindow);
	glfwSetFramebufferSizeCallback(pWindow, framebuffer_size_callback);

	static bool s_init = false;
	if (!s_init)
	{
		s_init = true;
		gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	}
	
	static float s_d[6][4] = {
		{0.2f, 0.2f, 0.2f, 0.5f},
		{0.0f, 0.0f, 0.7f, 0.5f},
		{0.5f, 0.0f, 0.7f, 0.5f},
		{1.0f, 0.0f, 0.7f, 0.5f},
		{0.0f, 0.5f, 0.7f, 0.5f},
		{0.0f, 1.0f, 0.7f, 0.5f}
	};

	int Count = 6;
	static int s_index = 0;
	

	glClearColor(s_d[s_index][0], s_d[s_index][1], s_d[s_index][2], s_d[s_index][3]);

	s_index++;
	if (s_index >= Count)
	{
		s_index = 0;
	}
	glClear(GL_COLOR_BUFFER_BIT);

	glfwSwapBuffers(pWindow);
	glfwPollEvents();
}

void CGLShowWnd::InitVBO()
{
	if (m_vbo == 0)
	{
		glGenBuffers(1, &m_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	}
}

void CGLShowWnd::OnBtnTriangle()
{
	GLFWwindow* pWindow = GetGLWindow();
	if (NULL == pWindow)
	{
		return;
	}

	float rectVertices[] = {
	0.5f, 0.5f, 0.0f,   //右上角
	0.5f, -0.5f, 0.0f,  //右下角
	-0.5f, -0.5f, 0.0f, //左下角
	-0.5f, 0.5f, 0.0f   //左上角
	};
	unsigned int indices[] = {
	0, 1, 3,    //第一个三角形
	1, 2, 3 //第二个三角形
	};
	glfwMakeContextCurrent(pWindow);
	glfwSetFramebufferSizeCallback(pWindow, framebuffer_size_callback);

	static bool s_init = false;
	if (!s_init)
	{
		s_init = true;
		gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	}

	GLuint vbo = 0;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glfwSwapBuffers(pWindow);
	glfwPollEvents();
}