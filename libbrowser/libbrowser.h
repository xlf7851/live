#pragma once

#ifdef  __cplusplus
	#define   LIB_BROWSER_API extern "C" __declspec(dllexport)
#else
	#define   LIB_BROWSER_API __declspec(dllexport)
#endif

// �¼�֪ͨ
typedef void* BROWSER_HANDLE;

/**
* @brief ��ʼ�������ģ��
* @param uOption ��ʼ����һЩ����    
* @param lpResourceDirPath �ƶ���ԴĿ¼·��          
*
* @return ����˵��
*/
LIB_BROWSER_API bool BrowserInitialize(unsigned int uOption, LPCTSTR lpResourceDirPath);


/**
* @brief �������Ϣѭ��
*
* @return ��
*/

LIB_BROWSER_API void BrowserMessageLoop();


/**
* @brief �ر������ģ��
*
* @return ��
*/

LIB_BROWSER_API void BrowserUnInitialize();

/**
* @brief �������������
*
* @return �����������
*/

LIB_BROWSER_API BROWSER_HANDLE BrowserCreateBrowser(HWND hParent, int left, int top, int width, int height);

/**
* @brief �ر������
*
* @return �����������
*/

LIB_BROWSER_API void BrowserCloseBrowser(BROWSER_HANDLE hBrowser);


/** ������ҳ
* @brief 
* @param hBrowser ���������
* @return ��
*/
LIB_BROWSER_API void BrowserNavigate(BROWSER_HANDLE hBrowser, LPCTSTR lpszUrl);

/**
* @brief �������Ϣѭ��
*
* @return ��
*/

LIB_BROWSER_API void BrowserMoveWindow(BROWSER_HANDLE hBrowser, int left, int top, int width, int height);