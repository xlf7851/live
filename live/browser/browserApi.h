#pragma once

class CBrowserApi
{
public:
	CBrowserApi();
	~CBrowserApi();

	/**
	* @brief ��ȡʵ��������ӿ�ʵ������
	*/
	static CBrowserApi* Instance();

	/**
	* @brief ��ʼ�������ģ��
	* @return ����˵��
	*/
	bool Initialize();


	/**
	* @brief �ر������ģ��
	*
	* @return ��
	*/
	void UnInitialize();

	/**
	* @brief �������������
	*
	* @return �����������
	*/
	BROWSER_HANDLE CreateBrowser(HWND hParent, int left = 0, int top = 0, int width = 0, int height = 0);

	/**
	* @brief �ر������
	*
	* @return �����������
	*/
	void CloseBrowser(BROWSER_HANDLE hBrowser);


	/** ������ҳ
	* @brief
	* @param hBrowser ���������
	* @return ��
	*/
	void Navigate(BROWSER_HANDLE hBrowser, LPCTSTR lpszUrl);

	/**
	* @brief �������Ϣѭ��
	*
	* @return ��
	*/
	void MoveWindow(BROWSER_HANDLE hBrowser, int left, int top, int width, int height);
};