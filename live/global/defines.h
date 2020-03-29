#pragma once

/* �Զ�����Ϣ
*/
#define UWM_BEGIN (WM_USER + 0x1234)
#define UWM_SHOW_PAGE	(UWM_BEGIN + 0x0001)
#define UWM_SKIN_CHANGED (UWM_BEGIN + 0x0002)
#define UWM_HTTP_REVEIVE (UWM_BEGIN + 0X0003)
#define UWM_BLOCK_ASK_RET	(UWM_BEGIN + 0x0004)

/* �ؼ��Զ�����Ϣ
*/

#define DUI_UMSGTYPE_POPUP_SLIDER_VALUECHANGED      (_T("u_popup_slider_valuechange"))

// custom control class
#define DUI_CUSTOM_CTRL_CLASS_ToolbarPage			(_T("ToolbarPageUI"))
#define DUI_CUSTOM_CTRL_CLASS_CaptionPageUI			(_T("CaptionPageUI"))
#define DUI_CUSTOM_CTRL_CLASS_BasePage				(_T("BasePageUI"))
#define DUI_CUSTOM_CTRL_CLASS_BlockcalcPage			(_T("BlockcalcPageUI"))
#define DUI_CUSTOM_CTRL_CLASS_Base64Page			(_T("Base64PageUI"))
#define DUI_CUSTOM_CTRL_CLASS_TestControlPage		(_T("TestControlPageUI"))
#define DUI_CUSTOM_CTRL_CLASS_GL					(_T("GLUI"))
#define DUI_CUSTOM_CTRL_CLASS_StockDataPage			(_T("StockDataPageUI"))
#define DUI_CUSTOM_CTRL_CLASS_StockDataTable		(_T("StockDataTableUI"))
#define DUI_CUSTOM_CTRL_CLASS_StockDataTableHeader	(_T("StockDataTableHeaderUI"))
#define DUI_CUSTOM_CTRL_CLASS_Browser				(_T("BrowserUI"))
#define DUI_CUSTOM_CTRL_CLASS_ToolbarItem			(_T("ToolbarItemUI"))
#define DUI_CUSTOM_CTRL_CLASS_WebBrowserPage		(_T("WebBrowserPageUI"))

// custom control interface
#define DUI_CUSTOM_CTRL_INTERFACE_ToolbarPage			(_T("ToolbarPage"))
#define DUI_CUSTOM_CTRL_INTERFACE_CaptionPageUI			(_T("CaptionPage"))
#define DUI_CUSTOM_CTRL_INTERFACE_BasePage				(_T("BasePage"))
#define DUI_CUSTOM_CTRL_INTERFACE_BlockcalcPage			(_T("BlockcalcPage"))
#define DUI_CUSTOM_CTRL_INTERFACE_Base64Page			(_T("Base64Page"))
#define DUI_CUSTOM_CTRL_INTERFACE_TestControlPage		(_T("TestControlPage"))
#define DUI_CUSTOM_CTRL_INTERFACE_GL					(_T("GL"))
#define DUI_CUSTOM_CTRL_INTERFACE_StockDataPage			(_T("StockDataPage"))
#define DUI_CUSTOM_CTRL_INTERFACE_StockDataTable		(_T("StockDataTable"))
#define DUI_CUSTOM_CTRL_INTERFACE_StockDataTableHeader	(_T("StockDataTableHeader"))
#define DUI_CUSTOM_CTRL_INTERFACE_Browser				(_T("Browser"))
#define DUI_CUSTOM_CTRL_INTERFACE_ToolbarItem			(_T("ToolbarItem"))
#define DUI_CUSTOM_CTRL_INTERFACE_WebBrowserPage		(_T("WebBrowserPage"))


// ���ܵ���,XML�е��ò�����a:b,c:d��ʽ���룬������map
#define FUNCTION_CALL_CallName						_T("functoncallname")
#define FUNCTION_CALL_CallParam						_T("functioncallparam")

// ���ܵ�������
#define FUNCTION_CALL_CallName_SwitchPage			_T("switchPage")	// ����ҳ��
#define FUNCTION_CALL_CallName_Command				_T("command")		// ����ҳ��
#define FUNCTION_CALL_CallName_ShowWebDlg			_T("showWebDlg")

// ���ܵ��ò�������
#define FUNCTION_CALL_PARAM_xmlres					_T("xmlres")		// xml�ļ�����id
#define FUNCTION_CALL_PARAM_CommandName				_T("CommandName")
#define FUNCTION_CALL_PARAM_Url						_T("url")



// ���ܵ��÷��ش�����Ϣ
enum function_call_error_code
{
	function_call_error_none = 0,			// �ɹ�
	function_call_error_unknow = 1,			// δ֪����
	function_call_error_name = 2,			// ����������
	function_call_error_param = 3,		    // ���ò�������

};


