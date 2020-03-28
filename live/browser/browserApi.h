#pragma once

class CBrowserApi
{
public:
	CBrowserApi();
	~CBrowserApi();

	/**
	* @brief 获取实例浏览器接口实例对象
	*/
	static CBrowserApi* Instance();

	/**
	* @brief 初始化浏览器模块
	* @return 返回说明
	*/
	bool Initialize();


	/**
	* @brief 关闭浏览器模块
	*
	* @return 无
	*/
	void UnInitialize();

	/**
	* @brief 创建浏览器对象
	*
	* @return 浏览器对象句柄
	*/
	BROWSER_HANDLE CreateBrowser(HWND hParent, int left = 0, int top = 0, int width = 0, int height = 0);

	/**
	* @brief 关闭浏览器
	*
	* @return 浏览器对象句柄
	*/
	void CloseBrowser(BROWSER_HANDLE hBrowser);


	/** 加载网页
	* @brief
	* @param hBrowser 浏览器对象
	* @return 无
	*/
	void Navigate(BROWSER_HANDLE hBrowser, LPCTSTR lpszUrl);

	/**
	* @brief 浏览器消息循环
	*
	* @return 无
	*/
	void MoveWindow(BROWSER_HANDLE hBrowser, int left, int top, int width, int height);
};