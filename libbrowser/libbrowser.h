#pragma once

#ifdef  __cplusplus
	#define   LIB_BROWSER_API extern "C" __declspec(dllexport)
#else
	#define   LIB_BROWSER_API __declspec(dllexport)
#endif

// 事件通知
typedef void* BROWSER_HANDLE;

/**
* @brief 初始化浏览器模块
* @param uOption 初始化的一些属性    
* @param lpResourceDirPath 制定资源目录路径          
*
* @return 返回说明
*/
LIB_BROWSER_API bool BrowserInitialize(unsigned int uOption, LPCTSTR lpResourceDirPath);


/**
* @brief 浏览器消息循环
*
* @return 无
*/

LIB_BROWSER_API void BrowserMessageLoop();


/**
* @brief 关闭浏览器模块
*
* @return 无
*/

LIB_BROWSER_API void BrowserUnInitialize();

/**
* @brief 创建浏览器对象
*
* @return 浏览器对象句柄
*/

LIB_BROWSER_API BROWSER_HANDLE BrowserCreateBrowser(HWND hParent, int left, int top, int width, int height);

/**
* @brief 关闭浏览器
*
* @return 浏览器对象句柄
*/

LIB_BROWSER_API void BrowserCloseBrowser(BROWSER_HANDLE hBrowser);


/** 加载网页
* @brief 
* @param hBrowser 浏览器对象
* @return 无
*/
LIB_BROWSER_API void BrowserNavigate(BROWSER_HANDLE hBrowser, LPCTSTR lpszUrl);

/**
* @brief 浏览器消息循环
*
* @return 无
*/

LIB_BROWSER_API void BrowserMoveWindow(BROWSER_HANDLE hBrowser, int left, int top, int width, int height);