#pragma once

////////////////////////////////////////////////////////////////////////////////////////
// begin 自定义消息
#define UWM_BEGIN (WM_USER + 0x1234)
#define UWM_SHOW_PAGE	(UWM_BEGIN + 0x0001)
#define UWM_SKIN_CHANGED (UWM_BEGIN + 0x0002)
#define UWM_HTTP_REVEIVE (UWM_BEGIN + 0X0003)
#define UWM_BLOCK_ASK_RET	(UWM_BEGIN + 0x0004)


////////////////////////////////////////////////////////////////////////////////////////
// begin duilib control
/* 控件自定义消息
*/

#define DUI_UMSGTYPE_POPUP_SLIDER_VALUECHANGED      (_T("u_popup_slider_valuechange"))

// custom control class
#define DUI_CUSTOM_CTRL_CLASS_ToolbarPage			(_T("ToolbarPageUI"))
#define DUI_CUSTOM_CTRL_CLASS_CaptionPageUI			(_T("CaptionPageUI"))
#define DUI_CUSTOM_CTRL_CLASS_BasePage				(_T("BasePageUI"))
#define DUI_CUSTOM_CTRL_CLASS_Base64Page			(_T("Base64PageUI"))
#define DUI_CUSTOM_CTRL_CLASS_TestControlPage		(_T("TestControlPageUI"))
#define DUI_CUSTOM_CTRL_CLASS_GL					(_T("GLUI"))
#define DUI_CUSTOM_CTRL_CLASS_GLPage				(_T("GLPageUI"))
#define DUI_CUSTOM_CTRL_CLASS_StockDataPage			(_T("StockDataPageUI"))
#define DUI_CUSTOM_CTRL_CLASS_StockDataTable		(_T("StockDataTableUI"))
#define DUI_CUSTOM_CTRL_CLASS_StockDataTableHeader	(_T("StockDataTableHeaderUI"))
#define DUI_CUSTOM_CTRL_CLASS_Browser				(_T("BrowserUI"))
#define DUI_CUSTOM_CTRL_CLASS_ToolbarItem			(_T("ToolbarItemUI"))
#define DUI_CUSTOM_CTRL_CLASS_WebBrowserPage		(_T("WebBrowserPageUI"))
#define DUI_CUSTOM_CTRL_CLASS_ImageShowPage			(_T("ImageShowPageUI"))
#define DUI_CUSTOM_CTRL_CLASS_BlockGroupTabContainer	(_T("BlockGroupTabContainerUI"))
#define DUI_CUSTOM_CTRL_CLASS_BlockList				(_T("BlockListUI"))


// custom control interface
#define DUI_CUSTOM_CTRL_INTERFACE_ToolbarPage			(_T("ToolbarPage"))
#define DUI_CUSTOM_CTRL_INTERFACE_CaptionPageUI			(_T("CaptionPage"))
#define DUI_CUSTOM_CTRL_INTERFACE_BasePage				(_T("BasePage"))
#define DUI_CUSTOM_CTRL_INTERFACE_Base64Page			(_T("Base64Page"))
#define DUI_CUSTOM_CTRL_INTERFACE_TestControlPage		(_T("TestControlPage"))
#define DUI_CUSTOM_CTRL_INTERFACE_GL					(_T("GL"))
#define DUI_CUSTOM_CTRL_INTERFACE_GLPage				(_T("GLPage"))
#define DUI_CUSTOM_CTRL_INTERFACE_StockDataPage			(_T("StockDataPage"))
#define DUI_CUSTOM_CTRL_INTERFACE_StockDataTable		(_T("StockDataTable"))
#define DUI_CUSTOM_CTRL_INTERFACE_StockDataTableHeader	(_T("StockDataTableHeader"))
#define DUI_CUSTOM_CTRL_INTERFACE_Browser				(_T("Browser"))
#define DUI_CUSTOM_CTRL_INTERFACE_ToolbarItem			(_T("ToolbarItem"))
#define DUI_CUSTOM_CTRL_INTERFACE_WebBrowserPage		(_T("WebBrowserPage"))
#define DUI_CUSTOM_CTRL_INTERFACE_ImageShowPage			(_T("ImageShowPage"))
#define DUI_CUSTOM_CTRL_INTERFACE_BlockGroupTabContainer	(_T("BlockGroupTabContainer"))
#define DUI_CUSTOM_CTRL_INTERFACE_BlockList				(_T("BlockList"))

////////////////////////////////////////////////////////////////////////////////////////
// begin function call
// 功能调用,XML中调用参数以a:b,c:d格式输入，解析成map
#define FUNCTION_CALL_CallName						_T("functoncallname")
#define FUNCTION_CALL_CallParam						_T("functioncallparam")

// 功能调用名称
#define FUNCTION_CALL_CallName_SwitchPage			_T("switchPage")	// 调整页面
#define FUNCTION_CALL_CallName_Command				_T("command")		// 调整页面
#define FUNCTION_CALL_CallName_ShowWebDlg			_T("showWebDlg")

// 功能调用参数名称
#define FUNCTION_CALL_PARAM_xmlres					_T("xmlres")		// xml文件后者id
#define FUNCTION_CALL_PARAM_CommandName				_T("CommandName")
#define FUNCTION_CALL_PARAM_Url						_T("url")



// 功能调用返回错误信息
enum function_call_error_code
{
	function_call_error_none = 0,			// 成功
	function_call_error_unknow = 1,			// 未知错误
	function_call_error_name = 2,			// 功能名错误
	function_call_error_param = 3,		    // 调用参数错误
};



////////////////////////////////////////////////////////////////////////////////////////
// begin binary file header
// 通用的二进制文件头
#pragma pack(1)
struct _xlf_common_binary_file_header_t
{
	uint32 m_nFlag;		// 标记
	uint8 m_nSize;		// 结构大小
	uint8 m_version;	// 版本号
	uint16 m_unuse;		// 
	uint32 m_param;		// 自定义参数
	uint32 m_ntotal;	// 总的大小
};
#pragma pack()

#define GET_XLF_BINARY_FILE_HEAD_FLAG(p) (*(uint32*)(p))

// 二进制文件头标志,统一使用4字节
const char g_szBlockCacheFileFlag[4] = { 'X','L','F','A' };	// 板块缓存文件
const char g_szBlockGroupFileFlag[4] = { 'X','L','F','B' };	// 板块分组文件



////////////////////////////////////////////////////////////////////////////////////////
// begin stock and block define 
typedef xlf::CUnt32Array BlockIDArray;

// block group id define
#define BLOCK_GROUP_ID_SELF			0x0001
#define BLOCK_GROUP_ID_STRATEGY 	0x0002
#define BLOCK_GROUP_BEGIN			0x0010
#define BLOCK_GROUP_END				0x8FFF


// block id define
#define BLOCK_ID_SELF_BEGIN			0x0001
#define BLOCK_ID_SELF_END			0x8fff

#define STOCK_CONFIG_ROOT_PATH				(_T("stock\\"))
#define STOCK_CONFIG_BLOCK_CACHE_PATH		(_T("stock\\blockcache.dat"))
#define STOCK_CONFIG_BLOCK_GROUP_PATH		(_T("stock\\blockgroup.dat"))


struct _block_draw_item_it
{
	uint32 m_u32BlockID;
	std::string m_strName;
	int m_nStockCnt;
	uint32 m_uParam;
	_block_draw_item_it()
	{
		m_u32BlockID = 0;
		m_uParam = 0;
	}
};

