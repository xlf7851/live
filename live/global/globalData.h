#pragma once

// ����һЩȫ�ֱ���


class CInitGlobalData
{
public:
	// ��ʼ��һЩȫ�ֶ���ʵ������Ӧ�����Ĵ���ʱ��Ķ�����
	static void InitGlobalInstance();

	// ��ʼ��������
	static void DoWork();

};

class CGlobalData
{
public:
	CGlobalData();
	~CGlobalData();

	static void OnTimer(UINT uEventID);

	static CGlobalData* Instance();

public:
	void Init();
	void DoGlobalFunction(LPCTSTR lpszFunc);
protected:


};
