#pragma once

// 定义一些全局变量


class CInitGlobalData
{
public:
	// 初始化一些全局对象实例，不应有消耗大量时间的对象构造
	static void InitGlobalInstance();

	// 初始化工作，
	static void DoWork();

};

class CGlobalData
{
public:
	CGlobalData();
	~CGlobalData();

	static CGlobalData* Instance();

public:
	void Init();
	void DoGlobalFunction(LPCTSTR lpszFunc);
protected:


};
