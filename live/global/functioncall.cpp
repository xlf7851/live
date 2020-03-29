#include "stdafx.h"
#include "functioncall.h"
#include "../browser/browserwindow.h"

int CFunctionCall::Call(const CFunctionCallItem& callinfo)
{
	_tstring strFuncName = callinfo.GetName();
	if (strFuncName.empty())
	{
		return function_call_error_name;
	}

	if (_tcsicmp(strFuncName.c_str(), FUNCTION_CALL_CallName_SwitchPage) == 0)	// 切换页面
	{
		_tstring strXml = callinfo.GetParamValue(FUNCTION_CALL_PARAM_xmlres);
		if (strXml.empty())
		{
			return function_call_error_param;
		}

		return SwitchPage(strXml);
	}
	else if (_tcsicmp(strFuncName.c_str(), FUNCTION_CALL_CallName_Command) == 0) // 
	{
		_tstring strXml = callinfo.GetParamValue(FUNCTION_CALL_PARAM_CommandName);
		if (strXml.empty())
		{
			return function_call_error_param;
		}

		return Command(strXml, callinfo);
	}
	else if (_tcsicmp(strFuncName.c_str(), FUNCTION_CALL_CallName_ShowWebDlg) == 0)
	{
		_tstring url = callinfo.GetParamValue(FUNCTION_CALL_PARAM_Url);
		if (url.empty())
		{
			return function_call_error_param;
		}

		return ShowWebDlg(url, callinfo);
	}

	return function_call_error_unknow;
}

int CFunctionCall::SwitchPage(const _tstring& xmlres)
{
	global_funciton::ShowPage(xmlres.c_str());
	return function_call_error_none;
}

int CFunctionCall::Command(const _tstring& strCommand, const CFunctionCallItem& callInfo)
{
	if (_tcsicmp(strCommand.c_str(), _T("restartProgram")) == 0)
	{
		global_funciton::RestartProgram();
		return function_call_error_none;
	}
	return function_call_error_unknow;
}

int CFunctionCall::ShowWebDlg(const _tstring& url, const CFunctionCallItem& callInfo)
{
	//if (callInfo.IsAttributeTrue(_T("commmon")))	// 通用对话框
	{
		CWebBrowserDlg* dlg = new CWebBrowserDlg;
		dlg->CreateDuiWindow(global_funciton::GetMianHwnd(), _T("webDlg"), UI_WNDSTYLE_FRAME, 0);
		dlg->CenterWindow();
		if (callInfo.IsAttributeTrue(_T("modal")))
		{
			dlg->ShowModal();
			delete dlg;
		}
		else
		{
			dlg->SetDeleteSelf(true);
			dlg->ShowWindow();
		}
	}
	return function_call_error_none;
}