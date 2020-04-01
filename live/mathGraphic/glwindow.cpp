#include "stdafx.h"
#include "glwindow.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "mathBase.h"
#include "shader.h"
#include "../global/GlobalFuncton.h"


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

static GLuint _CreateVAO()
{
	GLuint VAO = 0;
	glGenVertexArrays(1, &VAO);
	//glBindVertexArray(VAO);
	return VAO;
}

static unsigned int _CreateTriangleVBO(float* data, int len)
{
	unsigned int id = 0;
	glGenBuffers(1, &id);
	glBindBuffer(GL_ARRAY_BUFFER, id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * len, data, GL_STATIC_DRAW);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	return id;
}


static void _BindVBOAndBAO(GLuint vbo, GLuint vao)
{
	glBindVertexArray(vao);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// ʹ�ú�����
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

static void _GetShaderPath(_tstring& strVertex, _tstring& strFragment)
{
	_tstring strRootFile = global_funciton::GetConfigDir();
	strRootFile += _T("math\\shader\\");
	strVertex = strRootFile + _T("vertexShaderSource.h");
	strFragment = strRootFile + _T("fragmentShaderSorce.h");
}


/*�� �������ι���
 * �������
 * 1��VBO
 * 2��VAO
 * 3����ɫ��
 * ������
 * ����1����ͼֻ��ҪVBO,�������뵽VBO��ֱ�ӵ��û�ͼ�������ɡ�����ҪVAO���
 * ����2����ͼֻ��ҪVAO������������VBO���ٰ�VBO���������뵽VAO���ͷŵ�VBO��Ȼ����û�ͼ������
 * ����3����ͼ��ҪVAO��VBO,VAO��VBO�Ĺ�ϵ������ʲô���ģ���ͼ�Ĺ���������ô����
 * ��ͼ������glDrawArrays
 * 1���ӽ̸̳�������������glDrawArraysǰҪglBindVertexArray(vao),��Ȼ����1�Ǵ��
 */

class _CTestDrawTrangle
{
public:
	_CTestDrawTrangle()
	{
		m_vbo = 0;
		m_vao = 0;
	}

	~_CTestDrawTrangle()
	{
		Clear();
	}

	void Clear()
	{
		if (m_vao)
		{
			glDeleteVertexArrays(1, &m_vao);
			m_vao = 0;
		}

		if (m_vbo)
		{
			glDeleteBuffers(1, &m_vbo);
			m_vbo = 0;
		}
	}

	// ����1����ͼֻ��ҪVBO, �������뵽VBO��ֱ�ӵ��û�ͼ�������ɡ�����ҪVAO���
	// �����ʧ��
	void Draw1(float* data, int len)
	{
		Clear();
		//����������󡢶��㻺�����İ�
		//m_vao = _CreateVAO();
		m_vbo = _CreateTriangleVBO(data, len);


		// ����һ��������
		//glBindVertexArray(m_vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		//glBindVertexArray(0);

	}

	//����2����ͼֻ��ҪVAO������������VBO���ٰ�VBO���������뵽VAO(��ô����)���ͷŵ�VBO��Ȼ����û�ͼ������
	// �����ʧ��
	void Draw2(float* data, int len)
	{
		Clear();
		//����������󡢶��㻺�����İ�
		m_vbo = _CreateTriangleVBO(data, len);
		m_vao = _CreateVAO();


		_DrawTrangle();

	}

	//����3����ͼ��ҪVAO��VBO,VAO��VBO�Ĺ�ϵ������ʲô���ģ���ͼ�Ĺ���������ô����
	// 1������vao��
	// 2������vbo,�����������뵽vbo�С���һ���͵�1�����Ե�˳��û��Ҫ��
	// 3����vao��vbo�����������������������ô��
	// 4�������ǣ�����glDrawArraysҪ��Ҫ�󶨶������飿�󶨹��Ͳ�Ҫ�ˣ�Ϊ�˷�ֹ֮ǰû����󶨣����ǵ���һ�ΰ󶨱ȽϺ�
	//    vbo�����������ȽӴ���vbo�İ�ʱ�����Ŀ�����ʾ�ġ��������glDeleteBuffers��vbo�ǲ������ġ�
	// ��������:
	// 1�������3��ʵ��ʱ���ö�������  
	// �ܽ᣺
	// 1������������ʱ�������vao��ͬʱvbo����ɾ����
	// 2��ʹ��gl����(vao����vbo)ǰ�������Ȱ�
	void Draw3(float* data, int len)
	{
		Clear();
		//����������󡢶��㻺�����İ�

		m_vao = _CreateVAO();
		m_vbo = _CreateTriangleVBO(data, len);


		_BindVBOAndBAO(m_vbo, m_vao);

		_BindVao();
		_DrawTrangle();

	}

protected:
	void _BindVao()
	{
		if (m_vao)
		{
			glBindVertexArray(m_vao);
		}
	}

	void _DrawTrangle()
	{
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}

private:
	GLuint m_vbo;
	GLuint m_vao;
};

static void _Triangle2(GLFWwindow* pWindow)
{
	_tstring strVertex;
	_tstring strFragment;
	_GetShaderPath(strVertex, strFragment);
	ns_opengl::CShader shader(strVertex.c_str(), strFragment.c_str());
	if (shader.GetProgram() == 0)
	{
		return;
	}

	// ���õ����� (���л���) ���õ�����ԣ�����������ȣ�
	float vertices[] = {
		-0.5f, -0.5f, 0.0f, // left  
		0.5f, -0.5f, 0.0f, // right 
		0.0f,  0.5f, 0.0f  // top   
	};

	// �����Ļ
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// ʹ����ɫ��
	shader.Use();
	// ������
	_CTestDrawTrangle drawObj;
	drawObj.Draw3(vertices, 9);

	// ��ʾ������
	glfwSwapBuffers(pWindow);
}


// �ϰ汾������
static void _TriangleOld(GLFWwindow* pWindow)
{
	//glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_TRIANGLES);
	glColor3f(0.0, 1.0, 0.0);  //��ɫ  
	glVertex3f(-0.5f, -0.5f, 0.0f);
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(0.5f, -0.5f, 0.0f);
	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0.0f, 0.5f, 0.0f);
	glEnd();

	glFlush();
	glfwSwapBuffers(pWindow);
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
	if (m_pGLWindow == nullptr)
	{
		return;
	}

	glfwDestroyWindow(m_pGLWindow);
	m_pGLWindow = nullptr;
}

void CGLUI::SetInternVisible(bool bVisible)
{
	CControlUI::SetInternVisible(bVisible);
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

void CGLUI::SetGLViewport()
{
	if (m_pGLWindow)
	{
		glViewport(0, 0, m_rcItem.right - m_rcItem.left, m_rcItem.bottom - m_rcItem.top);
	}
}

IMPLEMENT_DUICONTROL(CGLPageUI)

CGLPageUI::CGLPageUI()
{
	m_pGLUI = nullptr;
	m_vbo = 0;
}

CGLPageUI::~CGLPageUI()
{

}

LPCTSTR CGLPageUI::GetClass() const
{
	return DUI_CUSTOM_CTRL_CLASS_GLPage;
}

LPVOID CGLPageUI::GetInterface(LPCTSTR pstrName)
{
	if (_tcsicmp(pstrName, DUI_CUSTOM_CTRL_INTERFACE_GLPage) == 0) return static_cast<CGLPageUI*>(this);
	return CBasePageUI::GetInterface(pstrName);


}

void CGLPageUI::InitPage()
{
	if (m_pManager)
	{
		m_pGLUI = static_cast<CGLUI*>(control_utl::FindChildByName(this, _T("glpageGlwnd")));
	}
	
}

void CGLPageUI::OnNotify(TNotifyUI& msg)
{
	CDuiString strName = msg.pSender->GetName();
	if (msg.sType == DUI_MSGTYPE_CLICK)
	{
		if (strName == _T("btnGlpageGlwndTest"))
		{
			OnTestGL();
			return;
		}
		else if (strName == _T("btnGlpageTriangle"))
		{
			OnBtnTriangle();
			return;
		}
	}
}

GLFWwindow* CGLPageUI::GetGLWindow()
{
	return m_pGLUI ? m_pGLUI->GetGLWindow() : NULL;
}
void CGLPageUI::OnTestGL()
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

void CGLPageUI::InitVBO()
{
	if (m_vbo == 0)
	{
		glGenBuffers(1, &m_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	}
}



void CGLPageUI::OnBtnTriangle()
{
	GLFWwindow* pWindow = GetGLWindow();
	if (NULL == pWindow)
	{
		return;
	}

	glfwMakeContextCurrent(pWindow);
	glfwSetFramebufferSizeCallback(pWindow, framebuffer_size_callback);


	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		return;
	}

	if (m_pGLUI)
	{
		m_pGLUI->SetGLViewport();
	}

	_Triangle2(pWindow);
}



CGLShowWnd::CGLShowWnd()
{
	
}

CGLShowWnd::~CGLShowWnd()
{
	
}

CDuiString CGLShowWnd::GetSkinFile()
{
	return _T("XML_show_glWindow");
}

LPCTSTR CGLShowWnd::GetWindowClassName() const
{
	return _T("glShowWnd");
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