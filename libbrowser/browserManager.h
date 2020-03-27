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
	* @brief ��ʼ�������ģ��
	* @param uOption ��ʼ����һЩ����
	* @param lpResourceDirPath �ƶ���ԴĿ¼·��
	*
	* @return ����˵��
	*/
	bool BrowserInitialize(unsigned int uOption, LPCTSTR lpResourceDirPath);


	/**
	* @brief �������Ϣѭ��
	*
	* @return ��
	*/

	void BrowserMessageLoop();


	/**
	* @brief �ر������ģ��
	*
	* @return ��
	*/

	void BrowserUnInitialize();

	/**
	* @brief �������������
	*
	* @return �����������
	*/

	CBrowserHandler* BrowserCreateBrowser(HWND hParent, const RECT& rc);

	/**
	* @brief �ر������
	*
	* @return �����������
	*/

	void BrowserCloseBrowser(CBrowserHandler* pHandler);

protected:
	CefRefPtr<CBrowserApp> m_app;

};

