#pragma once



class CFunctionCall
{
public:
	static int Call(const CFunctionCallItem& callinfo);

protected:
	// �л�ҳ��
	static int SwitchPage(const _tstring& xmlres);

	// command
	static int Command(const _tstring& strCommand, const CFunctionCallItem& callInfo);

	// webdlg
	static int ShowWebDlg(const _tstring& url, const CFunctionCallItem& callInfo);
};