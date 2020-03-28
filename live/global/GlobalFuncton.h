#pragma once

namespace global_funciton{

	// 获取主窗口句柄
	HWND GetMianHwnd();

	/* 显示当前主页面
	   lpszPath: xml文件
	   hNotify:为空则取主窗口
	*/
	void ShowPage(LPCTSTR lpszSkinFile, HWND hNotify = NULL);

	// 设置主窗口透明
	void EnableMainWndLayered(bool bEnable);
	void SetMainWndLayered(int nLayered);

	// 获取程序目录
	LPCTSTR GetProgramPath();
	LPCTSTR GetRootDir();

	// 获取配置目录
	LPCTSTR GetConfigDir();

	// 打开文件

}