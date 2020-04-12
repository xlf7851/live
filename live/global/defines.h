#pragma once

////////////////////////////////////////////////////////////////////////////////////////
// begin �Զ�����Ϣ
#define UWM_BEGIN (WM_USER + 0x1234)
#define UWM_SHOW_PAGE	(UWM_BEGIN + 0x0001)
#define UWM_SKIN_CHANGED (UWM_BEGIN + 0x0002)
#define UWM_HTTP_REVEIVE (UWM_BEGIN + 0X0003)
#define UWM_BLOCK_ASK_RET	(UWM_BEGIN + 0x0004)


////////////////////////////////////////////////////////////////////////////////////////
// begin duilib control
/* �ؼ��Զ�����Ϣ
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



////////////////////////////////////////////////////////////////////////////////////////
// begin binary file header
// ͨ�õĶ������ļ�ͷ
#pragma pack(1)
struct _xlf_common_binary_file_header_t
{
	uint32 m_nFlag;		// ���
	uint8 m_nSize;		// �ṹ��С
	uint8 m_version;	// �汾��
	uint16 m_unuse;		// 
	uint32 m_param;		// �Զ������
	uint32 m_ntotal;	// �ܵĴ�С
};
#pragma pack()

#define GET_XLF_BINARY_FILE_HEAD_FLAG(p) (*(uint32*)(p))

// �������ļ�ͷ��־,ͳһʹ��4�ֽ�
const char g_szBlockCacheFileFlag[4] = { 'X','L','F','A' };	// ��黺���ļ�
const char g_szBlockGroupFileFlag[4] = { 'X','L','F','B' };	// �������ļ�



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

