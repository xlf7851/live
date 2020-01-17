#include "StdAfx.h"
#include "frame/MainWnd.h"
#include "login/login.h"
#include "global/customControlFactory.h"
#include <curl.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

static void InitResource()
{
	// ��Դ·��
	CDuiString strResourcePath = CPaintManagerUI::GetInstancePath();
	
	strResourcePath += _T("skin\\");
	CPaintManagerUI::SetResourcePath(strResourcePath.GetData());
	// ������Դ������
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

	// �°汾opengl����ʹ����ɫ��������Ϊ��ѧϰ����ָ���汾
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

static void UnInitGL()
{
	glfwTerminate();
}



int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int nCmdShow)
{
	// COM
	HRESULT Hr = ::CoInitialize(NULL);
	if (FAILED(Hr)) return 0;
	// OLE
	HRESULT hRes = ::OleInitialize(NULL);

	InitGL();
	// ��ʼ��UI������
	CPaintManagerUI::SetInstance(hInstance);
	// ��ʼ����Դ
	InitResource();
	// ע���Զ���ؼ���
	CCustomControlFactory::InitFactory();

	//curl��ʼ��
	curl_global_init(CURL_GLOBAL_ALL);

	//_TestOpenGL();
	/*
	CLogin *pLogin = new CLogin;
	g_mainWnd = pLogin;
	pLogin->Create(NULL, _T("��½"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
	UINT uRet = pLogin->ShowModal();
	g_mainWnd = nullptr;
	delete pLogin;
	*/

	// ��½�ɹ�
	//if (uRet == IDOK)
 	{
 		// ����������
 		g_mainWnd = new CMainWnd();
 		if (g_mainWnd == NULL) return 0;
 		g_mainWnd->Create(NULL, _T("live"), UI_WNDSTYLE_FRAME, 0L, 0, 0, 800, 572);
 		g_mainWnd->CenterWindow();
 		// ��Ϣѭ��
 		CPaintManagerUI::MessageLoop();
 		// ���ٴ���
 		delete g_mainWnd;
 		g_mainWnd = NULL;
 	}
	
	//����curl
	curl_global_cleanup();
	// ������Դ
	CPaintManagerUI::Term();
	// OLE
	OleUninitialize();
	// COM
	::CoUninitialize();
	UnInitGL();

	return 0;
}