#ifndef __UILIST_H__
#define __UILIST_H__

namespace DuiLib {
	/////////////////////////////////////////////////////////////////////////////////////
	//

	typedef int (CALLBACK *PULVCompareFunc)(UINT_PTR, UINT_PTR, UINT_PTR);

	class CListHeaderUI;

#define UILIST_MAX_COLUMNS 32

	typedef struct tagTListInfoUI
	{
		int nColumns;
		RECT rcColumn[UILIST_MAX_COLUMNS];
		int nFont;
		UINT uTextStyle;
		RECT rcTextPadding;
		DWORD dwTextColor;
		DWORD dwBkColor;
		CDuiString sBkImage;
		bool bAlternateBk;
		DWORD dwSelectedTextColor;
		DWORD dwSelectedBkColor;
		CDuiString sSelectedImage;
		DWORD dwHotTextColor;
		DWORD dwHotBkColor;
		CDuiString sHotImage;
		DWORD dwDisabledTextColor;
		DWORD dwDisabledBkColor;
		CDuiString sDisabledImage;
		DWORD dwLineColor;
		bool bShowRowLine;
		bool bShowColumnLine;
		bool bShowHtml;
		bool bMultiExpandable;
		bool bRSelected;
	} TListInfoUI;


	/////////////////////////////////////////////////////////////////////////////////////
	//

	class IListCallbackUI
	{
	public:
		virtual LPCTSTR GetItemText(CControlUI* pList, int iItem, int iSubItem) = 0;
	};

	class IListOwnerUI
	{
	public:
		virtual UINT GetListType() = 0;
		virtual TListInfoUI* GetListInfo() = 0;
		virtual int GetCurSel() const = 0;
		virtual bool SelectItem(int iIndex, bool bTakeFocus = false) = 0;
		virtual bool SelectMultiItem(int iIndex, bool bTakeFocus = false) = 0;
		virtual bool UnSelectItem(int iIndex, bool bOthers = false) = 0;
		virtual void DoEvent(TEventUI& event) = 0;
	};

	class IListUI : public IListOwnerUI
	{
	public:
		virtual CListHeaderUI* GetHeader() const = 0;
		virtual CContainerUI* GetList() const = 0;
		virtual IListCallbackUI* GetTextCallback() const = 0;
		virtual void SetTextCallback(IListCallbackUI* pCallback) = 0;
		virtual bool ExpandItem(int iIndex, bool bExpand = true) = 0;
		virtual int GetExpandedItem() const = 0;

		virtual void SetMultiSelect(bool bMultiSel) = 0;
		virtual bool IsMultiSelect() const = 0;
		virtual void SelectAllItems() = 0;
		virtual void UnSelectAllItems() = 0;
		virtual int GetSelectItemCount() const = 0;
		virtual int GetNextSelItem(int nItem) const = 0;
	};

	class IListItemUI
	{
	public:
		virtual int GetIndex() const = 0;
		virtual void SetIndex(int iIndex) = 0;
		virtual IListOwnerUI* GetOwner() = 0;
		virtual void SetOwner(CControlUI* pOwner) = 0;
		virtual bool IsSelected() const = 0;
		virtual bool Select(bool bSelect = true) = 0;
		virtual bool SelectMulti(bool bSelect = true) = 0;
		virtual bool IsExpanded() const = 0;
		virtual bool Expand(bool bExpand = true) = 0;
		virtual void DrawItemText(HDC hDC, const RECT& rcItem) = 0;
	};


	/////////////////////////////////////////////////////////////////////////////////////
	//

	class CListBodyUI;
	class CListHeaderUI;
	class CEditUI;
	class CComboBoxUI;
	class UILIB_API CListUI : public CVerticalLayoutUI, public IListUI
	{
		DECLARE_DUICONTROL(CListUI)

	public:
		CListUI();

		void DoInit();
		void InitDefaultColorInfo();

		LPCTSTR GetClass() const;
		UINT GetControlFlags() const;
		LPVOID GetInterface(LPCTSTR pstrName);

		bool GetScrollSelect();
		void SetScrollSelect(bool bScrollSelect);
		int GetCurSel() const;
		int GetCurSelActivate() const;
		bool SelectItem(int iIndex, bool bTakeFocus = false);
		bool SelectItemActivate(int iIndex);    // 双击选中

		bool SelectMultiItem(int iIndex, bool bTakeFocus = false);
		void SetMultiSelect(bool bMultiSel);
		bool IsMultiSelect() const;
		bool UnSelectItem(int iIndex, bool bOthers = false);
		void SelectAllItems();
		void UnSelectAllItems();
		int GetSelectItemCount() const;
		int GetNextSelItem(int nItem) const;

		bool IsFixedScrollbar();
		void SetFixedScrollbar(bool bFixed);

		CListHeaderUI* GetHeader() const;  
		CContainerUI* GetList() const;
		UINT GetListType();
		TListInfoUI* GetListInfo();

		CControlUI* GetItemAt(int iIndex) const;
		int GetItemIndex(CControlUI* pControl) const;
		bool SetItemIndex(CControlUI* pControl, int iIndex);
		int GetCount() const;
		bool Add(CControlUI* pControl);
		bool AddAt(CControlUI* pControl, int iIndex);
		bool Remove(CControlUI* pControl);
		bool RemoveAt(int iIndex);
		void RemoveAll();

		void EnsureVisible(int iIndex);
		void Scroll(int dx, int dy);

		bool IsDelayedDestroy() const;
		void SetDelayedDestroy(bool bDelayed);
		int GetChildPadding() const;
		void SetChildPadding(int iPadding);

		void SetItemFont(int index);
		void SetItemTextStyle(UINT uStyle);
		void SetItemTextPadding(RECT rc);
		void SetItemTextColor(DWORD dwTextColor);
		void SetItemBkColor(DWORD dwBkColor);
		void SetItemBkImage(LPCTSTR pStrImage);
		void SetAlternateBk(bool bAlternateBk);
		void SetSelectedItemTextColor(DWORD dwTextColor);
		void SetSelectedItemBkColor(DWORD dwBkColor);
		void SetSelectedItemImage(LPCTSTR pStrImage); 
		void SetHotItemTextColor(DWORD dwTextColor);
		void SetHotItemBkColor(DWORD dwBkColor);
		void SetHotItemImage(LPCTSTR pStrImage);
		void SetDisabledItemTextColor(DWORD dwTextColor);
		void SetDisabledItemBkColor(DWORD dwBkColor);
		void SetDisabledItemImage(LPCTSTR pStrImage);
		void SetItemLineColor(DWORD dwLineColor);
		void SetItemShowRowLine(bool bShowLine = false);
		void SetItemShowColumnLine(bool bShowLine = false);
		bool IsItemShowHtml();
		void SetItemShowHtml(bool bShowHtml = true);
		bool IsItemRSelected();
		void SetItemRSelected(bool bSelected = true);
		RECT GetItemTextPadding() const;
		DWORD GetItemTextColor() const;
		DWORD GetItemBkColor() const;
		LPCTSTR GetItemBkImage() const;
		bool IsAlternateBk() const;
		DWORD GetSelectedItemTextColor() const;
		DWORD GetSelectedItemBkColor() const;
		LPCTSTR GetSelectedItemImage() const;
		DWORD GetHotItemTextColor() const;
		DWORD GetHotItemBkColor() const;
		LPCTSTR GetHotItemImage() const;
		DWORD GetDisabledItemTextColor() const;
		DWORD GetDisabledItemBkColor() const;
		LPCTSTR GetDisabledItemImage() const;
		DWORD GetItemLineColor() const;

		void SetMultiExpanding(bool bMultiExpandable); 
		int GetExpandedItem() const;
		bool ExpandItem(int iIndex, bool bExpand = true);

		void SetPos(RECT rc, bool bNeedInvalidate = true);
		void Move(SIZE szOffset, bool bNeedInvalidate = true);
		void DoEvent(TEventUI& event);
		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

		IListCallbackUI* GetTextCallback() const;
		void SetTextCallback(IListCallbackUI* pCallback);

		SIZE GetScrollPos() const;
		SIZE GetScrollRange() const;
		void SetScrollPos(SIZE szPos, bool bMsg = true);
		void LineUp();
		void LineDown();
		void PageUp();
		void PageDown();
		void HomeUp();
		void EndDown();
		void LineLeft();
		void LineRight();
		void PageLeft();
		void PageRight();
		void HomeLeft();
		void EndRight();
		void EnableScrollBar(bool bEnableVertical = true, bool bEnableHorizontal = false);
		virtual CScrollBarUI* GetVerticalScrollBar() const;
		virtual CScrollBarUI* GetHorizontalScrollBar() const;
		BOOL SortItems(PULVCompareFunc pfnCompare, UINT_PTR dwData);

		virtual BOOL CheckColumEditable(int nColum) { return FALSE; };
		virtual CEditUI* GetEditUI() { return NULL; };
		virtual BOOL CheckColumComboBoxable(int nColum) { return FALSE; };
		virtual CComboBoxUI* GetComboBoxUI() { return NULL; };

	protected:
		int GetMinSelItemIndex();
		int GetMaxSelItemIndex();

	protected:
		bool m_bFixedScrollbar;
		bool m_bScrollSelect;
		int m_iCurSel;
		bool m_bMultiSel;
		CStdPtrArray m_aSelItems;
		int m_iCurSelActivate;  // 双击的列
		int m_iExpandedItem;
		IListCallbackUI* m_pCallback;
		CListBodyUI* m_pList;
		CListHeaderUI* m_pHeader;
		TListInfoUI m_ListInfo;

	};

	/////////////////////////////////////////////////////////////////////////////////////
	//

	class UILIB_API CListBodyUI : public CVerticalLayoutUI
	{
	public:
		CListBodyUI(CListUI* pOwner);


		int GetScrollStepSize() const;
		void SetScrollPos(SIZE szPos, bool bMsg = true);
		void SetPos(RECT rc, bool bNeedInvalidate = true);
		void DoEvent(TEventUI& event);
		BOOL SortItems(PULVCompareFunc pfnCompare, UINT_PTR dwData);
	protected:
		static int __cdecl ItemComareFunc(void *pvlocale, const void *item1, const void *item2);
		int __cdecl ItemComareFunc(const void *item1, const void *item2);
	protected:
		CListUI* m_pOwner;
		PULVCompareFunc m_pCompareFunc;
		UINT_PTR m_compareData;
	};

	/////////////////////////////////////////////////////////////////////////////////////
	//

	class UILIB_API CListHeaderUI : public CHorizontalLayoutUI
	{
		DECLARE_DUICONTROL(CListHeaderUI)
	public:
		CListHeaderUI();

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);

		SIZE EstimateSize(SIZE szAvailable);
		void SetPos(RECT rc, bool bNeedInvalidate = true);
		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

		void SetScaleHeader(bool bIsScale);
		bool IsScaleHeader() const;

	private:
		bool m_bIsScaleHeader;
	};


	/////////////////////////////////////////////////////////////////////////////////////
	//

	class UILIB_API CListHeaderItemUI : public CContainerUI
	{
		DECLARE_DUICONTROL(CListHeaderItemUI)

	public:
		CListHeaderItemUI();

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);
		UINT GetControlFlags() const;

		void SetEnabled(bool bEnable = true);

		bool IsDragable() const;
		void SetDragable(bool bDragable);
		DWORD GetSepWidth() const;
		void SetSepWidth(int iWidth);
		DWORD GetTextStyle() const;
		void SetTextStyle(UINT uStyle);
		DWORD GetTextColor() const;
		void SetTextColor(DWORD dwTextColor);
		void SetTextPadding(RECT rc);
		RECT GetTextPadding() const;
		void SetFont(int index);
		bool IsShowHtml();
		void SetShowHtml(bool bShowHtml = true);
		LPCTSTR GetNormalImage() const;
		void SetNormalImage(LPCTSTR pStrImage);
		LPCTSTR GetHotImage() const;
		void SetHotImage(LPCTSTR pStrImage);
		LPCTSTR GetPushedImage() const;
		void SetPushedImage(LPCTSTR pStrImage);
		LPCTSTR GetFocusedImage() const;
		void SetFocusedImage(LPCTSTR pStrImage);
		LPCTSTR GetSepImage() const;
		void SetSepImage(LPCTSTR pStrImage);
		void SetScale(int nScale);
		int GetScale() const;

		void DoEvent(TEventUI& event);
		SIZE EstimateSize(SIZE szAvailable);
		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
		RECT GetThumbRect() const;

		void PaintText(HDC hDC);
		void PaintStatusImage(HDC hDC);

	protected:
		POINT ptLastMouse;
		bool m_bDragable;
		UINT m_uButtonState;
		int m_iSepWidth;
		DWORD m_dwTextColor;
		int m_iFont;
		UINT m_uTextStyle;
		bool m_bShowHtml;
		RECT m_rcTextPadding;
		CDuiString m_sNormalImage;
		CDuiString m_sHotImage;
		CDuiString m_sPushedImage;
		CDuiString m_sFocusedImage;
		CDuiString m_sSepImage;
		CDuiString m_sSepImageModify;
		int m_nScale;
	};


	/////////////////////////////////////////////////////////////////////////////////////
	//

	class UILIB_API CListElementUI : public CControlUI, public IListItemUI
	{
	public:
		CListElementUI();

		LPCTSTR GetClass() const;
		UINT GetControlFlags() const;
		LPVOID GetInterface(LPCTSTR pstrName);

		void SetEnabled(bool bEnable = true);

		int GetIndex() const;
		void SetIndex(int iIndex);

		IListOwnerUI* GetOwner();
		void SetOwner(CControlUI* pOwner);
		void SetVisible(bool bVisible = true);

		bool IsSelected() const;
		bool Select(bool bSelect = true);
		bool SelectMulti(bool bSelect = true);
		bool IsExpanded() const;
		bool Expand(bool bExpand = true);

		void Invalidate(); // 直接CControl::Invalidate会导致滚动条刷新，重写减少刷新区域
		bool Activate();

		void DoEvent(TEventUI& event);
		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

		void DrawItemBk(HDC hDC, const RECT& rcItem);

	protected:
		int m_iIndex;
		bool m_bSelected;
		UINT m_uButtonState;
		IListOwnerUI* m_pOwner;
	};


	/////////////////////////////////////////////////////////////////////////////////////
	//

	class UILIB_API CListLabelElementUI : public CListElementUI
	{
		DECLARE_DUICONTROL(CListLabelElementUI)
	public:
		CListLabelElementUI();

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);

		void DoEvent(TEventUI& event);
		SIZE EstimateSize(SIZE szAvailable);
		bool DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl);

		void DrawItemText(HDC hDC, const RECT& rcItem);
	};


	/////////////////////////////////////////////////////////////////////////////////////
	//

	class UILIB_API CListTextElementUI : public CListLabelElementUI
	{
		DECLARE_DUICONTROL(CListTextElementUI)
	public:
		CListTextElementUI();
		~CListTextElementUI();

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);
		UINT GetControlFlags() const;

		LPCTSTR GetText(int iIndex) const;
		void SetText(int iIndex, LPCTSTR pstrText);

		void SetOwner(CControlUI* pOwner);
		CDuiString* GetLinkContent(int iIndex);

		void DoEvent(TEventUI& event);
		SIZE EstimateSize(SIZE szAvailable);

		void DrawItemText(HDC hDC, const RECT& rcItem);

	protected:
		enum { MAX_LINK = 8 };
		int m_nLinks;
		RECT m_rcLinks[MAX_LINK];
		CDuiString m_sLinks[MAX_LINK];
		int m_nHoverLink;
		IListUI* m_pOwner;
		CStdPtrArray m_aTexts;
	};

	/////////////////////////////////////////////////////////////////////////////////////
	//

	class UILIB_API CListContainerElementUI : public CHorizontalLayoutUI, public IListItemUI
	{
		DECLARE_DUICONTROL(CListContainerElementUI)
	public:
		CListContainerElementUI();

		LPCTSTR GetClass() const;
		UINT GetControlFlags() const;
		LPVOID GetInterface(LPCTSTR pstrName);

		int GetIndex() const;
		void SetIndex(int iIndex);

		IListOwnerUI* GetOwner();
		void SetOwner(CControlUI* pOwner);
		void SetVisible(bool bVisible = true);
		void SetEnabled(bool bEnable = true);

		bool IsSelected() const;
		bool Select(bool bSelect = true);
		bool SelectMulti(bool bSelect = true);
		bool IsExpanded() const;
		bool Expand(bool bExpand = true);

		void Invalidate(); // 直接CControl::Invalidate会导致滚动条刷新，重写减少刷新区域
		bool Activate();

		void DoEvent(TEventUI& event);
		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
		bool DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl);

		virtual void DrawItemText(HDC hDC, const RECT& rcItem);    
		virtual void DrawItemBk(HDC hDC, const RECT& rcItem);

		void SetPos(RECT rc, bool bNeedInvalidate = true);

	protected:
		int m_iIndex;
		bool m_bSelected;
		UINT m_uButtonState;
		IListOwnerUI* m_pOwner;
	};


	//////////////////////////////////////////////////////////////////////////
// 虚拟列表数据提供者,抽象基类,由实际使用场景派生子类,实现对应的接口功能
//////////////////////////////////////////////////////////////////////////

/**
* @brief 虚拟列表接口类
* 提供开发者对子项数据管理个性化定制
*/
	class CVirtualListUI;
	class IVirtualDataProvider
	{
	public:
		/**
		* @brief 创建一个子项
		* @return 返回创建后的子项指针
		*/
		virtual CControlUI* CreateElement() = 0;

		/**
		* @brief 填充指定子项
		* @param[in] control 子项控件指针
		* @param[in] index 索引
		* @return 返回创建后的子项指针
		*/
		virtual void FillElement(CListUI* pList, CControlUI *pControl, int index) = 0;

		/**
		* @brief 获取子项总数
		* @return 返回子项总数
		*/
		virtual int GetElementCount() = 0;
	};

	

	

	class CVirListBodyUI : public CListBodyUI
	{
	public:
		CVirListBodyUI(CListUI* pOwner);
		~CVirListBodyUI();

		void SetVerScrollRange(int nRange);

		void SetItemCount(int nCount);

		//强制重绘,用于外部改变某些项内容
		void RedrawItems(int iItemFrom, int iItemTo);

		//void DoEvent(TEventUI& event);

		/**
		* @brief 设置数据代理对象
		* @param[in] pProvider 开发者需要重写 IVirtualDataProvider 的接口来作为数据代理对象
		* @return 无
		*/
		virtual void SetDataProvider(IVirtualDataProvider * pDataProvider);

		virtual IVirtualDataProvider * GetDataProvider();

		/**
		* @brief 设置子项高度
		* @param[in] nHeight 高度值
		* @return 无
		*/
		virtual void SetElementHeight(int nHeight);

		/**
		* @brief 初始化子项
		* @param[in] nMaxItemCount 初始化数量，默认 50
		* @return 无
		*/
		virtual void InitElement(int nMaxItemCount = 50);

		/**
		* @brief 刷新列表
		* @return 无
		*/
		virtual void Refresh();

		/**
		* @brief 删除所有子项
		* @return 无
		*/
		virtual void RemoveAll() override;

		/**
		* @brief 设置是否强制重新布局
		* @param[in] bForce 设置为 true 为强制，否则为不强制
		* @return 无
		*/
		void SetForceArrange(bool bForce);

		/**
		* @brief 获取当前所有可见控件的索引
		* @param[out] collection 索引列表
		* @return 无
		*/
		void GetDisplayCollection(std::vector<int>& collection);

		/**
		* @brief 让控件在可见范围内
		* @param[in] iIndex 控件索引
		* @param[in] bToTop 是否在最上方
		* @return 无
		*/
		void EnsureVisible(int iIndex, bool bToTop = false);

		/**
		* @brief 设置位置(子控件以及本身)
		* @param[in] rc 本身控件位置
		* @param[in] bNeedInvalidate 是否刷新
		* @return 无
		*/
		virtual void SetPos(RECT rc, bool bNeedInvalidate = true);

		/// 重写父类接口，提供个性化功能
		/**
		* @brief 滚动条改变,设置滚动条位置,做是否刷新界面
		* @param[in] szPos 滚动条位置
		* @return 无
		*/
		virtual void SetScrollPos(SIZE szPos, bool bMsg = true);

		void EnsureVisible(int iIndex);

		/**
		* @brief 重新布局子项
		* @param[in] bForce 是否强制重新布局
		* @return 无
		*/
		void ReArrangeChild(bool bForce);
	protected:

		/**
		* @brief 设置实际需要的滚动条位置
		* @param[in] rc 控件位置
		* @return 无
		*/
		void SetPosInternally(RECT rc);

		/**
		* @brief 添加一个子项
		* @param[in] iIndex 要插入的位置
		* @return 无
		*/
		void AddElement(int iIndex);//对于虚拟列表好像没什么用

		/**
		* @brief 移除一个子项
		* @param[in] iIndex 要移除的子项索引
		* @return 无
		*/
		void RemoveElement(int iIndex);

		/**
		* @brief 键盘按下通知
		* @param[in] ch 按键
		* @return 无
		*/
		virtual void OnKeyDown(TCHAR ch) {}

		/**
		* @brief 键盘弹起通知
		* @param[in] ch 按键
		* @return 无
		*/
		virtual void OnKeyUp(TCHAR ch) {}

	private:
		enum ScrollDirection
		{
			kScrollUp = -1,
			kScrollDown = 1
		};

		/**
		* @brief 创建一个子项
		* @return 返回创建后的子项指针
		*/
		CControlUI* CreateElement();

		/**
		* @brief 填充指定子项
		* @param[in] control 子项控件指针
		* @param[in] index 索引
		* @return 返回创建后的子项指针
		*/
		void FillElement(CControlUI *pControl, int iIndex);

		/**
		* @brief 获取元素总数
		* @return 返回元素总指数
		*/
		int GetElementCount();

		/**
		* @brief 使用默认布局
		* @return 成功返回 true，否则返回 false
		*/
		bool UseDefaultLayout();

		/**
		* @brief 得到n个元素对应的高度和，
		* @param[in] nCount 要得到多少元素的高度，-1表示全部元素
		* @return 返回指定数量元素的高度和
		*/
		int CalcElementsHeight(int nCount);

		/**
		* @brief 得到可见范围内第一个元素的前一个元素索引
		* @param[out] bottom 返回上一个元素的 bottom 值
		* @return 返回上一个元素的索引
		*/
		int GetTopElementIndex(int &bottom);

		/**
		* @brief 判断某个元素是否在可见范围内
		* @param[in] iIndex 元素索引
		* @return 返回 true 表示可见，否则为不可见
		*/
		bool IsElementDisplay(int iIndex);

		/**
		* @brief 判断是否要重新布局
		* @param[out] direction 向上滚动还是向下滚动
		* @return true 为需要重新布局，否则为 false
		*/
		bool NeedReArrange(ScrollDirection &direction);

		//未使用
		void Arrange();

	public:
		/**
		* @brief 每次重新刷新加载时 重新按照m_vSelIndex 中的索引初始化
		* @param[in] rc 位置
		* @param[in] cxRequired 水平需要的滚动条 Range
		* @param[in] cyRequired 垂直需要的滚动条Range
		* @return无
		*/
		void ProcessScrollBar(RECT rc, int cxRequired, int cyRequired);

		/**
		* @brief 每次重新刷新加载时 重新按照m_vSelIndex 中的索引初始化
		* @return无
		*/
		void InitSelectItem();


		/**
		* @brief 单选
		* @param[in] iIndex 选中索引
		* @param[in] bTakeFocus 是否获取焦点
		* @return true 是否选中
		*/
		bool SelectItem(int iIndex, bool bTakeFocus = false);

		/**
		* @brief 多选
		* @param[in] iIndex 选中索引
		* @param[in] bTakeFocus 是否获取焦点
		* @return true 是否选中
		*/
		bool SelectMultiItem(int iIndex, bool bTakeFocus = false);

		/// 不需要重写 begin
		// 	bool SelectItemActivate(int iIndex);    // 双击选中
		// 	void SetMultiSelect(bool bMultiSel); 
		// 	bool IsMultiSelect() const;
		//	bool UnSelectItem(int iIndex, bool bOthers = false);

		/// 不需要重写 end
			/**
			* @brief 全选
			* @return无
			*/
		void SelectAllItems();

		/**
		* @brief 清空所有选中
		* @return 无
		*/
		void UnSelectAllItems();
		//支持范围选择 add by ll  2019.04.02
		/**
		* @brief 范围选中  m_iSelIndex ~ iIndex 区域选中
		* @param[in] iIndex 选中结束索引
		* @param[in] bTakeFocus 是否获取焦点
		* @return true 是否选中
		*/
		virtual bool SelectRange(int iIndex, bool bTakeFocus = false);

		/**
		* @brief 获取选中的索引 (通过IVirtualDataProvider * 获取数据)
		* @return std::vector<int> 选中索引
		*/
		std::vector<int> GetSelectIndex();
	private:
		bool m_bIsStartBoxSel = false;//是否开始框选

		int m_iStartBoxIndex = -1; //开始框选索引
		int m_iEndBoxIndex = -1;//结束框选索引
		POINT m_BoxStartPt = { 0,0 };//框选开始起点

						   //OwnerData
		IVirtualDataProvider*  m_pDataProvider =nullptr;

		int m_nOwnerElementHeight = 20;	// 每个项的高度	
		int m_nOwnerItemCount = 0;	// 列表真实控件数量上限  
		int m_nOldYScrollPos = 0;
		bool m_bArrangedOnce = false;
		bool m_bForceArrange = false;	// 强制布局标记

		int m_iCurlShowBeginIndex = 0;
		int m_iCurlShowEndIndex = 0;

		bool m_bScrollProcess = false;//防止SetPos 重复调用, 导致死循环

		int m_iSelIndex = -1;
		std::vector<int> m_vSelIndex;//选中索引
	};

	class UILIB_API CVirtualListUI : public CListUI
	{
		DECLARE_DUICONTROL(CVirtualListUI)
	public:
		CVirtualListUI();
		~CVirtualListUI();

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);

		virtual void DoEvent(TEventUI& event);

		void SetItemCount(int nCount);

		/**
		* @brief 设置数据代理对象
		* @param[in] pProvider 开发者需要重写 IVirtualDataProvider 的接口来作为数据代理对象
		* @return 无
		*/
		virtual void SetDataProvider(IVirtualDataProvider * pDataProvider);

		virtual IVirtualDataProvider * GetDataProvider();

		/**
		* @brief 设置子项高度
		* @param[in] nHeight 高度值
		* @return 无
		*/
		virtual void SetElementHeight(int nHeight);

		/**
		* @brief 初始化子项
		* @param[in] nMaxItemCount 初始化数量，默认 50
		* @return 无
		*/
		virtual void InitElement(int nMaxItemCount = 50);

		/**
		* @brief 刷新列表
		* @return 无
		*/
		virtual void Refresh();


		/**
		* @brief 删除所有子项
		* @return 无
		*/
		virtual void RemoveAll() override;

		bool Remove(CControlUI* pControl);
		bool RemoveAt(int iIndex);

		/**
		* @brief 设置是否强制重新布局
		* @param[in] bForce 设置为 true 为强制，否则为不强制
		* @return 无
		*/
		void SetForceArrange(bool bForce);

		/**
		* @brief 获取当前所有可见控件的索引
		* @param[out] collection 索引列表
		* @return 无
		*/
		void GetDisplayCollection(std::vector<int>& collection);


		/**
		* @brief 获取选中的索引 (通过IVirtualDataProvider * 获取数据)
		* @return std::vector<int> 选中索引
		*/
		std::vector<int> GetSelectIndex();
	protected:
		/// 重写父类接口，提供个性化功能
		virtual void SetPos(RECT rc, bool bNeedInvalidate = true);
		virtual void _SetPos(bool bNeedInvalidate = true);

	private:
		enum ScrollDirection
		{
			kScrollUp = -1,
			kScrollDown = 1
		};

		/**
		* @brief 创建一个子项
		* @return 返回创建后的子项指针
		*/
		CControlUI* CreateElement();

		/**
		* @brief 填充指定子项
		* @param[in] control 子项控件指针
		* @param[in] index 索引
		* @return 返回创建后的子项指针
		*/
		void FillElement(CControlUI *pControl, int iIndex);

		/**
		* @brief 获取元素总数
		* @return 返回元素总指数
		*/
		int GetElementCount();


	private:

		void EnableScrollBar(bool bEnableVertical = true, bool bEnableHorizontal = false);


	protected:
		bool SelectItem(int iIndex, bool bTakeFocus = false);
		// 	bool SelectItemActivate(int iIndex);    // 双击选中
		// 
		bool SelectMultiItem(int iIndex, bool bTakeFocus = false);
		// 	void SetMultiSelect(bool bMultiSel);
		// 	bool IsMultiSelect() const;
		// 	bool UnSelectItem(int iIndex, bool bOthers = false);
		void SelectAllItems();
		void UnSelectAllItems();
		// 	//支持范围选择 add by ll  2019.04.02
		virtual bool SelectRange(int iIndex, bool bTakeFocus = false);

	protected:
		bool m_bIsStartBoxSel = false;//是否开始框选

		int m_iStartBoxIndex = -1; //开始框选索引
		int m_iEndBoxIndex = -1;//结束框选索引
		POINT m_BoxStartPt;//框选开始起点


		IVirtualDataProvider*  m_pDataProvider;//OwnerData

		CVirListBodyUI* m_pList;

		int m_nOwnerElementHeight;	// 每个项的高度	
		int m_nOwnerItemCount;	// 列表真实控件数量上限  
		int m_nOldYScrollPos;
		bool m_bArrangedOnce;
		bool m_bForceArrange;	// 强制布局标记

		int m_iCurlShowBeginIndex = 0;
		int m_iCurlShowEndIndex = 0;

		bool m_bScrollProcess = false;//防止SetPos 重复调用, 导致死循环
	};


} // namespace DuiLib

#endif // __UILIST_H__
