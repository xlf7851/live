#pragma once

namespace global_funciton{

	// ��ȡ�����ھ��
	HWND GetMianHwnd();

	/* ��ʾ��ǰ��ҳ��
	   lpszPath: xml�ļ�
	   hNotify:Ϊ����ȡ������
	*/
	void ShowPage(LPCTSTR lpszSkinFile, HWND hNotify = NULL);

	// ����������͸��
	void EnableMainWndLayered(bool bEnable);
	void SetMainWndLayered(int nLayered);

	// ��ȡ����Ŀ¼
	LPCTSTR GetProgramPath();
	LPCTSTR GetRootDir();

	// ��ȡ����Ŀ¼
	LPCTSTR GetConfigDir();

	// ���ļ�

}