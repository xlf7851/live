#pragma once

/* 自定义消息
*/
#define UWM_BEGIN (WM_USER + 0x1234)
#define UWM_SHOW_PAGE	(UWM_BEGIN + 0x0001)
#define UWM_SKIN_CHANGED (UWM_BEGIN + 0x0002)
#define UWM_HTTP_REVEIVE (UWM_BEGIN + 0X0003)
#define UWM_BLOCK_ASK_RET	(UWM_BEGIN + 0x0004)

/* 控件自定义消息
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

// custom control interface
#define DUI_CUSTOM_CTRL_INTERFACE_ToolbarPage			(_T("ToolbarPage"))
#define DUI_CUSTOM_CTRL_INTERFACE_CaptionPageUI			(_T("CaptionPage"))
#define DUI_CUSTOM_CTRL_INTERFACE_BasePage				(_T("BasePage"))
#define DUI_CUSTOM_CTRL_INTERFACE_BlockcalcPage			(_T("BlockcalcPage"))
#define DUI_CUSTOM_CTRL_INTERFACE_Base64Page			(_T("Base64Page"))
#define DUI_CUSTOM_CTRL_INTERFACE_TestControlPage		(_T("TestControlPage"))
#define DUI_CUSTOM_CTRL_INTERFACE_GL				(_T("GL"))