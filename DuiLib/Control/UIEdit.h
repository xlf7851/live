#ifndef __UIEDIT_H__
#define __UIEDIT_H__

#pragma once

namespace DuiLib
{
	class IEditPreMessageHandler
	{
	public:
		virtual LRESULT EditMessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled) = 0;
	};

	class CEditWnd;

	class UILIB_API CEditUI : public CLabelUI
	{
		DECLARE_DUICONTROL(CEditUI)
		friend class CEditWnd;
	public:
		CEditUI();

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);
		UINT GetControlFlags() const;

		void SetEnabled(bool bEnable = true);
		void SetText(LPCTSTR pstrText);
		void SetMaxChar(UINT uMax);
		UINT GetMaxChar();
		void SetReadOnly(bool bReadOnly);
		bool IsReadOnly() const;
		void SetPasswordMode(bool bPasswordMode);
		bool IsPasswordMode() const;
		void SetPasswordChar(TCHAR cPasswordChar);
		TCHAR GetPasswordChar() const;
		void SetNumberOnly(bool bNumberOnly);
		bool IsNumberOnly() const;
		int GetWindowStyls() const;

		LPCTSTR GetNormalImage();
		void SetNormalImage(LPCTSTR pStrImage);
		LPCTSTR GetHotImage();
		void SetHotImage(LPCTSTR pStrImage);
		LPCTSTR GetFocusedImage();
		void SetFocusedImage(LPCTSTR pStrImage);
		LPCTSTR GetDisabledImage();
		void SetDisabledImage(LPCTSTR pStrImage);
		void SetNativeEditBkColor(DWORD dwBkColor);
		DWORD GetNativeEditBkColor() const;
		void SetNativeEditTextColor( LPCTSTR pStrColor );
		DWORD GetNativeEditTextColor() const;

		bool IsAutoSelAll();
		void SetAutoSelAll(bool bAutoSelAll);
		void SetSel(long nStartChar, long nEndChar);
		void GetSel(long &nStartChar, long& nEndChar);
		void SetSelAll();
		void SetReplaceSel(LPCTSTR lpszReplace);

		void SetTipValue(LPCTSTR pStrTipValue);
		LPCTSTR GetTipValue();
		void SetTipValueColor(LPCTSTR pStrColor);
		DWORD GetTipValueColor();

		void SetPos(RECT rc, bool bNeedInvalidate = true);
		void Move(SIZE szOffset, bool bNeedInvalidate = true);
		void SetVisible(bool bVisible = true);
		void SetInternVisible(bool bVisible = true);
		SIZE EstimateSize(SIZE szAvailable);
		void DoEvent(TEventUI& event);
		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

		void PaintStatusImage(HDC hDC);
		void PaintText(HDC hDC);

		LRESULT PreHandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
		void SetPreMessageHandler(IEditPreMessageHandler* pHandler);

	protected:
		CEditWnd* m_pWindow;

		UINT m_uMaxChar;
		bool m_bReadOnly;
		bool m_bPasswordMode;
		bool m_bAutoSelAll;
		TCHAR m_cPasswordChar;
		UINT m_uButtonState;
		CDuiString m_sNormalImage;
		CDuiString m_sHotImage;
		CDuiString m_sFocusedImage;
		CDuiString m_sDisabledImage;
		CDuiString m_sTipValue;
		DWORD m_dwTipValueColor;
		DWORD m_dwEditbkColor;
		DWORD m_dwEditTextColor;
		int m_iWindowStyls;

		IEditPreMessageHandler* m_pPreMessageHandler;
	};

	class CEditContainerItemUI;
	class IEditContainerUI
	{
	public:
		virtual LRESULT EditMessageHandler(CEditContainerItemUI* pItem, UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled) = 0;
		virtual void OnDbClickItem(CEditContainerItemUI* pItem) = 0;
	};


	class UILIB_API CEditContainerItemLabelUI : public CLabelUI
	{
		DECLARE_DUICONTROL(CEditContainerItemLabelUI)
	public:
		CEditContainerItemLabelUI();
		~CEditContainerItemLabelUI();

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);

		void DoEvent(TEventUI& event);
	};

	class UILIB_API CEditContainerItemUI : public CHorizontalLayoutUI, public IEditPreMessageHandler
	{
		DECLARE_DUICONTROL(CEditContainerItemUI)
	public:
		CEditContainerItemUI();
		~CEditContainerItemUI();

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);

		void SetControl(CLabelUI* pControl);
		void ClearControl(bool bCopyTextToEdit = false);
		void DoEvent(TEventUI& event);
		SIZE EstimateSize(SIZE szAvailable);

		void SetEditWidth(int nWidth);
		CEditUI* GetEdit();
		void EnableEdit(bool bEnable);
		void SetEditFocus();

		void SetEditContainer(IEditContainerUI* pContainer);
		LRESULT EditMessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);

		void SetEditMode();
		void EndEditMode();
		bool IsEditMode();
		void ClearEditMode();

		bool IsOnlyEdit();
		void SetLabelMode();
		void AutoEditWidth();

		CLabelUI* GetLabel();
		CDuiString GetItemText();
		void SetItemText(LPCTSTR lpszText);

		void SetEditStyle(LPCTSTR lpszStyle);
		void SetControlStyle(LPCTSTR lpszStyle);

		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

	protected:
		CEditUI* m_pEdit;
		int m_nBackEditWidth;
		bool m_bEditMode;
		IEditContainerUI* m_pEditContainer;
	};

	class CEditContainerUI;
	class IEditContainerEditMessageHandler
	{
	public:
		virtual bool OnEditUpKeydown(CEditContainerUI* pContainer, int index, CEditContainerItemUI* pItem, CEditUI* pEdit) = 0;
		virtual bool OnEditDownKeydown(CEditContainerUI* pContainer, int index, CEditContainerItemUI* pItem, CEditUI* pEdit) = 0;
		virtual bool OnEditLeftKeydown(CEditContainerUI* pContainer, int index, CEditContainerItemUI* pItem, CEditUI* pEdit) = 0;
		virtual bool OnEditRightKeydown(CEditContainerUI* pContainer, int index, CEditContainerItemUI* pItem, CEditUI* pEdit) = 0;
		virtual bool OnEditReturnKeydown(CEditContainerUI* pContainer, int index, CEditContainerItemUI* pItem, CEditUI* pEdit) = 0;
		virtual bool OnEditBackKeydown(CEditContainerUI* pContainer, int index, CEditContainerItemUI* pItem, CEditUI* pEdit) = 0;
		virtual bool OnEditKillFocus(CEditContainerUI* pContainer, int index, CEditContainerItemUI* pItem, CEditUI* pEdit) = 0;
	};

	class IEditContainerItemCreater
	{
	public:
		virtual CLabelUI* CreateItemControl() = 0;
	};

	class UILIB_API CEditContainerUI : public CHorizontalLayoutUI, public IEditContainerUI, public INotifyUI
	{
	public:
		DECLARE_DUICONTROL(CEditContainerUI)
		CEditContainerUI();
		~CEditContainerUI();

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);

		virtual void DoInit();

		LRESULT EditMessageHandler(CEditContainerItemUI* pItem, UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
		virtual void OnDbClickItem(CEditContainerItemUI* pItem);

		CEditContainerItemUI* GetEditContainerItemAt(int index);

		bool AddEditContainerItem(CEditContainerItemUI* pControl);
		bool AddEditContainerItemAt(CEditContainerItemUI* pControl, int iIndex);
		bool RemoveEditContainerItem(CEditContainerItemUI* pControl);
		bool RemoveEditContainerItemAt(int iIndex);
		void RemoveAllEditContainerItem(bool bDeleteDefault = false);
		int GetEditContainerItemIndex(CControlUI* pItem);

		void SetPos(RECT rc, bool bNeedInvalidate /* = true */);

		void SetMinLastEditWidth(int width);
		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

		int GetPreEditContainerItem(int nIndex, CEditContainerItemUI** pFind, bool bCheckVisible = true);
		int GetNextEditContainerItem(int nIndex, CEditContainerItemUI** pFind, bool bCheckVisible = true);

		int FindEditContainerItemByText(LPCTSTR lpszText, CEditContainerItemUI** pFind = NULL, int nSkipIndex = -1, CEditContainerItemUI* pSkipItem = NULL);
		int FindEidtContainerItemByChildControl(CControlUI* pChild, CEditContainerItemUI** pFind = NULL);

		void SetItemEditStyle(CEditContainerItemUI* pItem, LPCTSTR lpszStyle);
		void SetItemStyle(CEditContainerItemUI* pItem, LPCTSTR lpszStyle);
		void SetItemLabelStyle(CEditContainerItemUI* pItem, LPCTSTR lpszStyle);

		void ApplyAllItemStyle();
		void ApplyItemStyle(CEditContainerItemUI* pItem);

		virtual bool AddEditControlItem(LPCTSTR lpszText);

		void SetPreEditMessageHandler(IEditContainerEditMessageHandler* pHandler);
		void SetItemControlCreater(IEditContainerItemCreater* pCreater);

		void OnAfterEditContainerItemKillFocus(CEditContainerItemUI* pItem, bool bEditMode, bool bCheckAdd);

		void EnableItemTextRepeat(bool bEnable);
		bool IsEnableItemTextRepeat();

		void SetEditContainerModify(bool bModify);
		bool IsEditContainerModify();

		bool IsEditContainerEmpty();
		CEditUI* GetCurrentEdit();
		void SetLastItemFocus();
		void OnEditContainerChanged();


		virtual bool OnEditKeydown(int index, CEditContainerItemUI* pItem, CEditUI* pEdit, TCHAR uKey);
		virtual bool OnEditUpKeydown(int index, CEditContainerItemUI* pItem, CEditUI* pEdit);
		virtual bool OnEditDownKeydown(int index, CEditContainerItemUI* pItem, CEditUI* pEdit);
		virtual bool OnEditLeftKeydown(int index, CEditContainerItemUI* pItem, CEditUI* pEdit);
		virtual bool OnEditRightKeydown(int index, CEditContainerItemUI* pItem, CEditUI* pEdit);
		virtual bool OnEditReturnKeydown(int index, CEditContainerItemUI* pItem, CEditUI* pEdit);
		virtual bool OnEditBackKeydown(int index, CEditContainerItemUI* pItem, CEditUI* pEdit);
		virtual void OnEditKillFocus(int index, CEditContainerItemUI* pItem, CEditUI* pEdit);

	protected:
		void DoEvent(TEventUI& event);
		void Notify(TNotifyUI& msg);
		CLabelUI* CreateItemControl();

	protected:
		int m_nMinLastEditWidth;
		CDuiString m_strEditStyleName;
		CDuiString m_strLabelStyleName;
		CDuiString m_strItemStyleName;

		IEditContainerEditMessageHandler* m_pPreEditMessageHandler;

		CEditContainerItemUI* m_pCurrentEditItem;
		IEditContainerItemCreater* m_pItemControlCreater;

		bool m_bCanItemTextRepeat;
		bool m_bContainerModify;
	};

}
#endif // __UIEDIT_H__