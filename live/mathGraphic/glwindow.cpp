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

//着色器代码---------------------------------------------------------------------
//点着色器
const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
//片段着色器
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

	// 设置点数据 (还有缓冲) 配置点的属性（包含点坐标等）
	float vertices[] = {
		-0.5f, -0.5f, 0.0f, // left  
		0.5f, -0.5f, 0.0f, // right 
		0.0f,  0.5f, 0.0f  // top   
	};
	//顶点数组对象、顶点缓冲对象的绑定
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// 绑定顶点数组, 然后绑定并设置缓冲, 最后配置顶点属性.
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//注意这是允许的，对glVertexAttribPointer的调用将VBO注册为顶点属性的绑定顶点缓冲区对象，所以之后我们可以安全地解除绑定
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// 您可以在之后取消绑定VAO，以便其他VAO调用不会意外地修改此VAO，但这种情况很少发生。无论如何， 
	// 修改其他VAO需要调用glBindVertexArray，因此我们通常不会在不直接需要时解除VAO（VBO同样）的绑定。
	glBindVertexArray(0);


		// 清除屏幕
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// 画第一个三角形
	shader.Use();
	glBindVertexArray(VAO); //可以知道我们只有一个三角形VAO，没必要每次都绑定它，但是我们这么做会让代码有一点组织性
	glDrawArrays(GL_TRIANGLES, 0, 3);
	// glBindVertexArray(0); //没必要每次都解绑 

	// glfw: 交换buffers和poll的IO事件 (按键按下/释放，鼠标移动等.)
	glfwSwapBuffers(pWindow);
	glfwPollEvents();
	//}

	// 一旦他们超出已有的资源，就取消所有资源的分配：
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

}
static void _Triangle(GLFWwindow* pWindow)
{
	// 建立并编译着色器--------------------------------------------------------------
	// 点着色器
	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// 检查点着色器是否有错误
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
	}
	// 片段着色器
	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// 检查片段着色器是否有错误
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
	}
	// 链接着色器 shaders
	int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// 检查着色器错误
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// 设置点数据 (还有缓冲) 配置点的属性（包含点坐标等）
	float vertices[] = {
		-0.5f, -0.5f, 0.0f, // left  
		0.5f, -0.5f, 0.0f, // right 
		0.0f,  0.5f, 0.0f  // top   
	};
	//顶点数组对象、顶点缓冲对象的绑定
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// 绑定顶点数组, 然后绑定并设置缓冲, 最后配置顶点属性.
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//注意这是允许的，对glVertexAttribPointer的调用将VBO注册为顶点属性的绑定顶点缓冲区对象，所以之后我们可以安全地解除绑定
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// 您可以在之后取消绑定VAO，以便其他VAO调用不会意外地修改此VAO，但这种情况很少发生。无论如何， 
	// 修改其他VAO需要调用glBindVertexArray，因此我们通常不会在不直接需要时解除VAO（VBO同样）的绑定。
	glBindVertexArray(0);

	// 取消注释此调用会绘制线框多边形。
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//while (!glfwWindowShouldClose(window))
	//{
		// 输入
	//	processInput(window);

		// 清除屏幕
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// 画第一个三角形
	glUseProgram(shaderProgram);
	glBindVertexArray(VAO); //可以知道我们只有一个三角形VAO，没必要每次都绑定它，但是我们这么做会让代码有一点组织性
	glDrawArrays(GL_TRIANGLES, 0, 3);
	// glBindVertexArray(0); //没必要每次都解绑 

	// glfw: 交换buffers和poll的IO事件 (按键按下/释放，鼠标移动等.)
	glfwSwapBuffers(pWindow);
	glfwPollEvents();
	//}

	// 一旦他们超出已有的资源，就取消所有资源的分配：
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

// 老版本画三角
static void _TriangleOld(GLFWwindow* pWindow)
{
	//glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_TRIANGLES);
	glColor3f(0.0, 1.0, 0.0);  //绿色  
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