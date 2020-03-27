#pragma once
#include "include/cef_base.h"
class CBrowserApp;
class CBrowserHandler;
class CBrowerManager
{
public:
	CBrowerManager();
	~CBrowerManager();

	/**
	* @brief 初始化浏览器模块
	* @param uOption 初始化的一些属性
	* @param lpResourceDirPath 制定资源目录路径
	*
	* @return 返回说明
	*/
	bool BrowserInitialize(unsigned int uOption, LPCTSTR lpResourceDirPath);


	/**
	* @brief 浏览器消息循环
	*
	* @return 无
	*/

	void BrowserMessageLoop();


	/**
	* @brief 关闭浏览器模块
	*
	* @return 无
	*/

	void BrowserUnInitialize();

	/**
	* @brief 创建浏览器对象
	*
	* @return 浏览器对象句柄
	*/

	CBrowserHandler* BrowserCreateBrowser(HWND hParent, const RECT& rc);

	/**
	* @brief 关闭浏览器
	*
	* @return 浏览器对象句柄
	*/

	void BrowserCloseBrowser(CBrowserHandler* pHandler);

protected:
	CefRefPtr<CBrowserApp> m_app;

};

