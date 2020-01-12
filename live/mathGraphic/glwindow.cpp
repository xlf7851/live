#include "stdafx.h"
#include "glwindow.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "mathBase.h"
#include "shader.h"


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

void CGLUI::SetGLViewport()
{
	if (m_pGLWindow)
	{
		glViewport(0, 0, m_rcItem.right - m_rcItem.left, m_rcItem.bottom - m_rcItem.top);
	}
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

//��ɫ������---------------------------------------------------------------------
//����ɫ��
const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
//Ƭ����ɫ��
const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.9f, 0.2f, 0.1f, 1.0f);\n"
"}\n\0";

static void _Triangle2(GLFWwindow* pWindow)
{
	ns_opengl::CShader shader;
	shader.LoadFromData(vertexShaderSource, fragmentShaderSource);
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
	//����������󡢶��㻺�����İ�
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// �󶨶�������, Ȼ��󶨲����û���, ������ö�������.
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//ע����������ģ���glVertexAttribPointer�ĵ��ý�VBOע��Ϊ�������Եİ󶨶��㻺������������֮�����ǿ��԰�ȫ�ؽ����
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// ��������֮��ȡ����VAO���Ա�����VAO���ò���������޸Ĵ�VAO��������������ٷ�����������Σ� 
	// �޸�����VAO��Ҫ����glBindVertexArray���������ͨ�������ڲ�ֱ����Ҫʱ���VAO��VBOͬ�����İ󶨡�
	glBindVertexArray(0);


		// �����Ļ
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// ����һ��������
	shader.Use();
	glBindVertexArray(VAO); //����֪������ֻ��һ��������VAO��û��Ҫÿ�ζ�����������������ô�����ô�����һ����֯��
	glDrawArrays(GL_TRIANGLES, 0, 3);
	// glBindVertexArray(0); //û��Ҫÿ�ζ���� 

	// glfw: ����buffers��poll��IO�¼� (��������/�ͷţ�����ƶ���.)
	glfwSwapBuffers(pWindow);
	glfwPollEvents();
	//}

	// һ�����ǳ������е���Դ����ȡ��������Դ�ķ��䣺
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

}
static void _Triangle(GLFWwindow* pWindow)
{
	// ������������ɫ��--------------------------------------------------------------
	// ����ɫ��
	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// ������ɫ���Ƿ��д���
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
	}
	// Ƭ����ɫ��
	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// ���Ƭ����ɫ���Ƿ��д���
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
	}
	// ������ɫ�� shaders
	int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// �����ɫ������
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// ���õ����� (���л���) ���õ�����ԣ�����������ȣ�
	float vertices[] = {
		-0.5f, -0.5f, 0.0f, // left  
		0.5f, -0.5f, 0.0f, // right 
		0.0f,  0.5f, 0.0f  // top   
	};
	//����������󡢶��㻺�����İ�
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// �󶨶�������, Ȼ��󶨲����û���, ������ö�������.
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//ע����������ģ���glVertexAttribPointer�ĵ��ý�VBOע��Ϊ�������Եİ󶨶��㻺������������֮�����ǿ��԰�ȫ�ؽ����
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// ��������֮��ȡ����VAO���Ա�����VAO���ò���������޸Ĵ�VAO��������������ٷ�����������Σ� 
	// �޸�����VAO��Ҫ����glBindVertexArray���������ͨ�������ڲ�ֱ����Ҫʱ���VAO��VBOͬ�����İ󶨡�
	glBindVertexArray(0);

	// ȡ��ע�ʹ˵��û�����߿����Ρ�
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//while (!glfwWindowShouldClose(window))
	//{
		// ����
	//	processInput(window);

		// �����Ļ
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// ����һ��������
	glUseProgram(shaderProgram);
	glBindVertexArray(VAO); //����֪������ֻ��һ��������VAO��û��Ҫÿ�ζ�����������������ô�����ô�����һ����֯��
	glDrawArrays(GL_TRIANGLES, 0, 3);
	// glBindVertexArray(0); //û��Ҫÿ�ζ���� 

	// glfw: ����buffers��poll��IO�¼� (��������/�ͷţ�����ƶ���.)
	glfwSwapBuffers(pWindow);
	glfwPollEvents();
	//}

	// һ�����ǳ������е���Դ����ȡ��������Դ�ķ��䣺
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
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

void CGLShowWnd::OnBtnTriangle()
{
	GLFWwindow* pWindow = GetGLWindow();
	if (NULL == pWindow)
	{
		return;
	}

	glfwMakeContextCurrent(pWindow);
	glfwSetFramebufferSizeCallback(pWindow, framebuffer_size_callback);
	//glfwSetWindowRefreshCallback(pWindow, _flush);


	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		return;
	}

	if (m_pGLUI)
	{
		m_pGLUI->SetGLViewport();
	}

	_Triangle2(pWindow);
	
	//_TriangleOld(pWindow);
	
	//_Triangle(pWindow);
	

}