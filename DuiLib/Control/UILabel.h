#ifndef __UILABEL_H__
#define __UILABEL_H__

#pragma once

namespace DuiLib
{
	class UILIB_API CLabelUI : public CControlUI
	{
		DECLARE_DUICONTROL(CLabelUI)
	public:
		CLabelUI();
		~CLabelUI();

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);
		UINT GetControlFlags() const;

		void SetTextStyle(UINT uStyle);
		UINT GetTextStyle() const;
		void SetTextColor(DWORD dwTextColor);
		DWORD GetTextColor() const;
		void SetDisabledTextColor(DWORD dwTextColor);
		DWORD GetDisabledTextColor() const;
		void SetFont(int index);
		int GetFont() const;
		RECT GetTextPadding() const;
		void SetTextPadding(RECT rc);
		bool IsShowHtml();
		void SetShowHtml(bool bShowHtml = true);

		SIZE EstimateSize(SIZE szAvailable);
		void DoEvent(TEventUI& event);
		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

		void PaintText(HDC hDC);

		virtual bool GetAutoCalcWidth() const;
		virtual void SetAutoCalcWidth(bool bAutoCalcWidth);
		virtual void SetText(LPCTSTR pstrText);
		
	protected:
		DWORD	m_dwTextColor;
		DWORD	m_dwDisabledTextColor;
		int		m_iFont;
		UINT	m_uTextStyle;
		RECT	m_rcTextPadding;
		bool	m_bShowHtml;
		bool	m_bAutoCalcWidth;
	};

	class UILIB_API CPieChartUI : public CLabelUI
	{
	public:
		struct _data_item_t
		{
			DWORD dwDataID;
			DWORD m_dwColor;
			double m_dbValue;
			int m_dbAngle;	// œ‡∂‘Ω«∂»
			CDuiString m_strPercent;
			_data_item_t()
			{
				m_dbAngle = 0;
				m_dbValue = 0.0;
				dwDataID = 0;
			}
		};

		typedef std::vector<_data_item_t*> _data_vectior;

		DECLARE_DUICONTROL(CPieChartUI)
	public:
		CPieChartUI();
		~CPieChartUI();

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);

		void PaintText(HDC hDC);
		void AddDataItem(DWORD dwDataID, DWORD dwColor, double dbValue);
		void UpdateDataItem(DWORD dwDataID, double dbValue);
		double GetValueTotal();
		void CalcData();
		bool IsDataNull();
		void SetPieBorderColor(DWORD dwColor);
		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

		void SetRadius(int radius);
		void SetInnerRadius(int r);
		void SetPieBkColor(DWORD dwColor);
		void SetDecimal(int nDecimal);
		void SetMinDrawPercent(int percent);
		void SetStartAngle(int nAngle);
		void SetPieFont(int iFont);

	protected:
		_data_item_t* _FindDataItem(DWORD dwID, bool bNew = false);
		void ClearDataItem();
	protected:
		int m_nRadius;
		int m_nInnerRadius;
		int m_nStartAngle;
		int m_nDrawMinPercent;
		int m_nDecimal;
		 
		_data_vectior  m_data;
		bool m_bCalcData;
		DWORD m_dwPieBorderColor;
		DWORD m_dwPieBkColor;
		int m_iPieFont;
	};
}

#endif // __UILABEL_H__