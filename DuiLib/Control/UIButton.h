#ifndef __UIBUTTON_H__
#define __UIBUTTON_H__

#pragma once

namespace DuiLib
{
	class UILIB_API CButtonUI : public CLabelUI
	{
		DECLARE_DUICONTROL(CButtonUI)

	public:
		CButtonUI();

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);
		UINT GetControlFlags() const;

		bool Activate();
		void SetEnabled(bool bEnable = true);
		void DoEvent(TEventUI& event);

		virtual LPCTSTR GetNormalImage();
		virtual void SetNormalImage(LPCTSTR pStrImage);
		virtual LPCTSTR GetHotImage();
		virtual void SetHotImage(LPCTSTR pStrImage);
		virtual LPCTSTR GetPushedImage();
		virtual void SetPushedImage(LPCTSTR pStrImage);
		virtual LPCTSTR GetFocusedImage();
		virtual void SetFocusedImage(LPCTSTR pStrImage);
		virtual LPCTSTR GetDisabledImage();
		virtual void SetDisabledImage(LPCTSTR pStrImage);
		virtual LPCTSTR GetHotForeImage();
		virtual void SetHotForeImage(LPCTSTR pStrImage);
		void SetStateCount(int nCount);
		int GetStateCount() const;
		virtual LPCTSTR GetStateImage();
		virtual void SetStateImage(LPCTSTR pStrImage);

		void BindTabIndex(int _BindTabIndex);
		void BindTabLayoutName(LPCTSTR _TabLayoutName);
		void BindTriggerTabSel(int _SetSelectIndex = -1);
		void RemoveBindTabIndex();
		int	 GetBindTabLayoutIndex();
		LPCTSTR GetBindTabLayoutName();

		void SetHotBkColor(DWORD dwColor);
		DWORD GetHotBkColor() const;
		void SetPushedBkColor(DWORD dwColor);
		DWORD GetPushedBkColor() const;
		void SetHotTextColor(DWORD dwColor);
		DWORD GetHotTextColor() const;
		void SetPushedTextColor(DWORD dwColor);
		DWORD GetPushedTextColor() const;
		void SetFocusedTextColor(DWORD dwColor);
		DWORD GetFocusedTextColor() const;
		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

		void PaintText(HDC hDC);

		void PaintBkColor(HDC hDC);
		void PaintStatusImage(HDC hDC);
		void PaintForeImage(HDC hDC);

		DWORD GetPaintBorderColor();

	protected:
		UINT m_uButtonState;

		DWORD m_dwHotBkColor;
		DWORD m_dwPushedBkColor;
		DWORD m_dwHotTextColor;
		DWORD m_dwPushedTextColor;
		DWORD m_dwFocusedTextColor;

		CDuiString m_sNormalImage;
		CDuiString m_sHotImage;
		CDuiString m_sHotForeImage;
		CDuiString m_sPushedImage;
		CDuiString m_sPushedForeImage;
		CDuiString m_sFocusedImage;
		CDuiString m_sDisabledImage;
		int m_nStateCount;
		CDuiString m_sStateImage;

		int			m_iBindTabIndex;
		CDuiString	m_sBindTabLayoutName;
	};

	class UILIB_API CCompareValueButtonUI : public CButtonUI
	{
		DECLARE_DUICONTROL(CCompareValueButtonUI)

	public:
		CCompareValueButtonUI();

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);
	
		void PaintText(HDC hDC);

		virtual CDuiString GetPaintText();
		virtual DWORD GetPaintTextColor();

		// 设置数据比较的颜色，可以指设置up颜色作为数据的颜色，并且可以不设置比较
		void SetCompareColor(DWORD dwColorUp, DWORD dwColorDown=0, DWORD dwZero=0);
		void SetDecimal(int nDecimal);

		void EnablePaintText(bool bEnable);
		void EnableCompare(bool bEnable);
		void EnableFabs(bool bEnable);
		void EnableSign(bool bEnable);
		void SetDataSuffix(LPCTSTR lpszText);

		void SetDataValue(double dbValue);
		void SetDataZero(double dbValue);
		void SetDataUnit(double dbUnit);

		void SetDataColorWidth(int width);
		void SetDataColorHeight(int height);
		void SetColorLeftPadding(int nPadding);

		void SetDataFmt(LPCTSTR lpszFmtText);
		void SetDataPrefix(LPCTSTR lpszPrefix);

		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);


	protected:
		bool m_enablePaintText;
		bool m_enableCompare;
		bool m_enableFabs;
		bool m_enableSign;

		int m_nColorWidth;
		int m_nColorHeight;
		int m_nColorLeftPadding;
		double m_dbValue;
		double m_dbZero;
		double m_dbValueUnit;

		DWORD m_dwCompareColor[3];
		int m_nDecimal;

		CDuiString m_strSuffix;
		CDuiString m_strPrefix;
		CDuiString m_strDataFmt;
	};

}	// namespace DuiLib

#endif // __UIBUTTON_H__