#include "StdAfx.h"
#include "frame/MainWnd.h"
#include "login/login.h"
#include "global/customControlFactory.h"
#include <curl.h>
#include <libbrowser.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

static void InitResource()
{
	// 资源路径
	CDuiString strResourcePath = CPaintManagerUI::GetInstancePath();
	
	strResourcePath += _T("skin\\");
	CPaintManagerUI::SetResourcePath(strResourcePath.GetData());
	// 加载资源管理器
	CResourceManager::GetInstance()->LoadResource(_T("res.xml"), NULL);
}



static WindowImplBase* g_mainWnd = nullptr;
HWND GlobalGetMainWnd()
{
	if (g_mainWnd)
	{
		return g_mainWnd->GetHWND();
	}

	return NULL;
}

static void InitGL()
{
	glfwInit();

	// 新版本opengl必须使用着色器，这里为了学习，不指定版本
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
}

static void UnInitGL()
{
	glfwTerminate();
}

LPCTSTR _GetModulePath()
{
	static TCHAR szFilePath[MAX_PATH + 1] = { 0 };
	if (szFilePath[0] != 0)
	{
		return szFilePath;
	}
	GetModuleFileName(NULL, szFilePath, MAX_PATH);
	for (int i = MAX_PATH; i >= 0; i--)
	{
		if (szFilePath[i] == _T('\\') || szFilePath[i] == _T('/'))
		{
			szFilePath[i + 1] = 0;
			break;
		}
	}

	return szFilePath;
}

std::string _GetCefResPath()
{
	std::string strPath = _GetModulePath();

	strPath += _T("cefres\\");


	return strPath;

}


static bool InitCef()
{
	std::string strCefRespath = _GetCefResPath();

	std::wstring strPath;
	xlf::AnsiToWstring(strCefRespath.c_str(), strCefRespath.size(), strPath);
	if (BrowserInitialize(0, (LPCTSTR)strPath.c_str()))
	{
		return true;
	}

	return false;
}

static void UnInitCef()
{
	BrowserUnInitialize();
}



int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int nCmdShow)
{
	// COM
	HRESULT Hr = ::CoInitialize(NULL);
	if (FAILED(Hr)) return 0;
	// OLE
	HRESULT hRes = ::OleInitialize(NULL);
	if (!InitCef())
	{
		//MessageBox(NULL, "test", "fuck", MB_OK);
		return 0;
	}

	InitGL();
	// 初始化UI管理器
	CPaintManagerUI::SetInstance(hInstance);
	// 初始化资源
	InitResource();
	// 注册自定义控件类
	CCustomControlFactory::InitFactory();

	//curl初始化
	curl_global_init(CURL_GLOBAL_ALL);

	//_TestOpenGL();
	/*
	CLogin *pLogin = new CLogin;
	g_mainWnd = pLogin;
	pLogin->Create(NULL, _T("登陆"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
	UINT uRet = pLogin->ShowModal();
	g_mainWnd = nullptr;
	delete pLogin;
	*/

	// 登陆成功
	//if (uRet == IDOK)
 	{
 		// 创建主窗口
 		g_mainWnd = new CMainWnd();
 		if (g_mainWnd == NULL) return 0;
 		g_mainWnd->Create(NULL, _T("live"), UI_WNDSTYLE_FRAME, 0L, 0, 0, 800, 572);
 		g_mainWnd->CenterWindow();
 		// 消息循环
 		CPaintManagerUI::MessageLoop();
 		// 销毁窗口
 		delete g_mainWnd;
 		g_mainWnd = NULL;
 	}
	
	//清理curl
	curl_global_cleanup();

	UnInitGL();

	UnInitCef();

	// 清理资源
	CPaintManagerUI::Term();
	// OLE
	OleUninitialize();
	// COM
	::CoUninitialize();
	

	return 0;
}