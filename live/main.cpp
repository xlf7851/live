#include "StdAfx.h"
#include "frame/MainWnd.h"
#include "login/login.h"
#include "global/customControlFactory.h"
#include <curl.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "browser/browserModule.h"
#include "global/globalData.h"



static void LoadDuilibResource()
{
#ifdef _DEBUG
	// 资源路径
	CDuiString strResourcePath = CPaintManagerUI::GetInstancePath();
	strResourcePath += _T("skin\\");
	CPaintManagerUI::SetResourcePath(strResourcePath.GetData());
#else 
	CPaintManagerUI::SetResourceType(UILIB_ZIPRESOURCE);
	HRSRC hResource = ::FindResource(CPaintManagerUI::GetResourceDll(), MAKEINTRESOURCE(IDR_ZIPRES_SKIN), _T("ZIPRES"));
	if (hResource != NULL) {
		DWORD dwSize = 0;
		HGLOBAL hGlobal = ::LoadResource(CPaintManagerUI::GetResourceDll(), hResource);
		if (hGlobal != NULL) {
			dwSize = ::SizeofResource(CPaintManagerUI::GetResourceDll(), hResource);
			if (dwSize > 0) {
				CPaintManagerUI::SetResourceZip((LPBYTE)::LockResource(hGlobal), dwSize);
			}
		}
		::FreeResource(hResource);
	}
#endif
	
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

inline static bool InitCef()
{
	return CBrowserModule::Instance()->Initialize();
}

inline static void UnInitCef()
{
	CBrowserModule::Instance()->UnInitialize();
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
	
	// 初始化DUILIB资源
	CPaintManagerUI::SetInstance(hInstance);
	LoadDuilibResource();

	// 注册自定义控件类
	CCustomControlFactory::InitFactory();

	//curl初始化
	curl_global_init(CURL_GLOBAL_ALL);

	// 全局数据初始化
	CInitGlobalData::InitGlobalInstance();

	CInitGlobalData::DoWork();

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