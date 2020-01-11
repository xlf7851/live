#include "StdAfx.h"
#include "UILabel.h"

#include <atlconv.h>
namespace DuiLib
{
	IMPLEMENT_DUICONTROL(CLabelUI)

	CLabelUI::CLabelUI() : m_uTextStyle(DT_VCENTER | DT_SINGLELINE), m_dwTextColor(0), 
		m_dwDisabledTextColor(0),
		m_iFont(-1),
		m_bShowHtml(false),
		m_bAutoCalcWidth(false)
	{
		::ZeroMemory(&m_rcTextPadding, sizeof(m_rcTextPadding));
	}

	CLabelUI::~CLabelUI()
	{
	}

	LPCTSTR CLabelUI::GetClass() const
	{
		return _T("LabelUI");
	}

	LPVOID CLabelUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcsicmp(pstrName, _T("Label")) == 0 ) return static_cast<CLabelUI*>(this);
		return CControlUI::GetInterface(pstrName);
	}
	
	UINT CLabelUI::GetControlFlags() const
	{
		return IsEnabled() ? UIFLAG_SETCURSOR : 0;
	}
	void CLabelUI::SetTextStyle(UINT uStyle)
	{
		m_uTextStyle = uStyle;
		Invalidate();
	}

	UINT CLabelUI::GetTextStyle() const
	{
		return m_uTextStyle;
	}

	void CLabelUI::SetTextColor(DWORD dwTextColor)
	{
		m_dwTextColor = dwTextColor;
		Invalidate();
	}

	DWORD CLabelUI::GetTextColor() const
	{
		return m_dwTextColor;
	}

	void CLabelUI::SetDisabledTextColor(DWORD dwTextColor)
	{
		m_dwDisabledTextColor = dwTextColor;
		Invalidate();
	}

	DWORD CLabelUI::GetDisabledTextColor() const
	{
		return m_dwDisabledTextColor;
	}

	void CLabelUI::SetFont(int index)
	{
		m_iFont = index;
		Invalidate();
	}

	int CLabelUI::GetFont() const
	{
		return m_iFont;
	}

	RECT CLabelUI::GetTextPadding() const
	{
		return m_rcTextPadding;
	}

	void CLabelUI::SetTextPadding(RECT rc)
	{
		m_rcTextPadding = rc;
		Invalidate();
	}

	bool CLabelUI::IsShowHtml()
	{
		return m_bShowHtml;
	}

	void CLabelUI::SetShowHtml(bool bShowHtml)
	{
		if( m_bShowHtml == bShowHtml ) return;

		m_bShowHtml = bShowHtml;
		Invalidate();
	}

	SIZE CLabelUI::EstimateSize(SIZE szAvailable)
	{
		if (m_bAutoCalcWidth) {
			CDuiString sText = GetText();

			RECT rcText = {0, 0, szAvailable.cx, szAvailable.cy};
			int nLinks = 0;
			if( m_bShowHtml ) CRenderEngine::DrawHtmlText(m_pManager->GetPaintDC(), m_pManager, rcText, sText, m_dwTextColor, NULL, NULL, nLinks, m_iFont, DT_CALCRECT | m_uTextStyle);
			else CRenderEngine::DrawText(m_pManager->GetPaintDC(), m_pManager, rcText, sText, m_dwTextColor, m_iFont, DT_CALCRECT | m_uTextStyle);
			m_cxyFixed.cx = MulDiv(rcText.right - rcText.left + GetManager()->GetDPIObj()->Scale(m_rcTextPadding.left) + GetManager()->GetDPIObj()->Scale(m_rcTextPadding.right), 100, GetManager()->GetDPIObj()->GetScale());
		}

		if( m_cxyFixed.cy == 0 ) return CDuiSize(GetManager()->GetDPIObj()->Scale(m_cxyFixed.cx), m_pManager->GetFontInfo(GetFont())->tm.tmHeight + 4);
		return CControlUI::EstimateSize(szAvailable);
	}

	void CLabelUI::DoEvent(TEventUI& event)
	{
		if( event.Type == UIEVENT_SETFOCUS ) 
		{
			m_bFocused = true;
			return;
		}
		if( event.Type == UIEVENT_KILLFOCUS ) 
		{
			m_bFocused = false;
			return;
		}
		CControlUI::DoEvent(event);
	}

	void CLabelUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if( _tcsicmp(pstrName, _T("align")) == 0 ) {
			if( _tcsstr(pstrValue, _T("left")) != NULL ) {
				m_uTextStyle &= ~(DT_CENTER | DT_RIGHT);
				m_uTextStyle |= DT_LEFT;
			}
			if( _tcsstr(pstrValue, _T("center")) != NULL ) {
				m_uTextStyle &= ~(DT_LEFT | DT_RIGHT );
				m_uTextStyle |= DT_CENTER;
			}
			if( _tcsstr(pstrValue, _T("right")) != NULL ) {
				m_uTextStyle &= ~(DT_LEFT | DT_CENTER);
				m_uTextStyle |= DT_RIGHT;
			}
		}
		else if( _tcsicmp(pstrName, _T("valign")) == 0 ) {
			if( _tcsstr(pstrValue, _T("top")) != NULL ) {
				m_uTextStyle &= ~(DT_BOTTOM | DT_VCENTER | DT_WORDBREAK);
				m_uTextStyle |= (DT_TOP | DT_SINGLELINE);
			}
			if( _tcsstr(pstrValue, _T("vcenter")) != NULL ) {
				m_uTextStyle &= ~(DT_TOP | DT_BOTTOM | DT_WORDBREAK);            
				m_uTextStyle |= (DT_VCENTER | DT_SINGLELINE);
			}
			if( _tcsstr(pstrValue, _T("bottom")) != NULL ) {
				m_uTextStyle &= ~(DT_TOP | DT_VCENTER | DT_WORDBREAK);
				m_uTextStyle |= (DT_BOTTOM | DT_SINGLELINE);
			}
		}
		else if( _tcsicmp(pstrName, _T("endellipsis")) == 0 ) {
			if( _tcsicmp(pstrValue, _T("true")) == 0 ) m_uTextStyle |= DT_END_ELLIPSIS;
			else m_uTextStyle &= ~DT_END_ELLIPSIS;
		}   
		else if( _tcsicmp(pstrName, _T("wordbreak")) == 0 ) {
			if( _tcsicmp(pstrValue, _T("true")) == 0 ) {
				m_uTextStyle &= ~DT_SINGLELINE;
				m_uTextStyle |= DT_WORDBREAK | DT_EDITCONTROL;
			}
			else {
				m_uTextStyle &= ~DT_WORDBREAK & ~DT_EDITCONTROL;
				m_uTextStyle |= DT_SINGLELINE;
			}
		}
		else if( _tcsicmp(pstrName, _T("noprefix")) == 0 ) {
			if( _tcsicmp(pstrValue, _T("true")) == 0)
			{
				m_uTextStyle |= DT_NOPREFIX;
			}
			else
			{
				m_uTextStyle = m_uTextStyle & ~DT_NOPREFIX;
			}
		}
		else if( _tcsicmp(pstrName, _T("font")) == 0 ) SetFont(_ttoi(pstrValue));
		else if( _tcsicmp(pstrName, _T("textcolor")) == 0 ) {
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetTextColor(clrColor);
		}
		else if( _tcsicmp(pstrName, _T("disabledtextcolor")) == 0 ) {
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetDisabledTextColor(clrColor);
		}
		else if( _tcsicmp(pstrName, _T("textpadding")) == 0 ) {
			RECT rcTextPadding = { 0 };
			LPTSTR pstr = NULL;
			rcTextPadding.left = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
			rcTextPadding.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);    
			rcTextPadding.right = _tcstol(pstr + 1, &pstr, 10);  ASSERT(pstr);    
			rcTextPadding.bottom = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);    
			SetTextPadding(rcTextPadding);
		}
		else if( _tcsicmp(pstrName, _T("showhtml")) == 0 ) SetShowHtml(_tcsicmp(pstrValue, _T("true")) == 0);
		else if( _tcsicmp(pstrName, _T("autocalcwidth")) == 0 ) {
			SetAutoCalcWidth(_tcsicmp(pstrValue, _T("true")) == 0);
		}
		else CControlUI::SetAttribute(pstrName, pstrValue);
	}

	void CLabelUI::PaintText(HDC hDC)
	{
		if( m_dwTextColor == 0 ) m_dwTextColor = m_pManager->GetDefaultFontColor();
		if( m_dwDisabledTextColor == 0 ) m_dwDisabledTextColor = m_pManager->GetDefaultDisabledColor();

		RECT rc = m_rcItem;
		RECT m_rcTextPadding = CLabelUI::m_rcTextPadding;
		GetManager()->GetDPIObj()->Scale(&m_rcTextPadding);
		rc.left += m_rcTextPadding.left;
		rc.right -= m_rcTextPadding.right;
		rc.top += m_rcTextPadding.top;
		rc.bottom -= m_rcTextPadding.bottom;

		CDuiString sText = GetText();
		if( sText.IsEmpty() ) return;
		int nLinks = 0;
		if( IsEnabled() ) {
			if( m_bShowHtml )
				CRenderEngine::DrawHtmlText(hDC, m_pManager, rc, sText, m_dwTextColor, \
				NULL, NULL, nLinks, m_iFont, m_uTextStyle);
			else
				CRenderEngine::DrawText(hDC, m_pManager, rc, sText, m_dwTextColor, \
				m_iFont, m_uTextStyle);
		}
		else {
			if( m_bShowHtml )
				CRenderEngine::DrawHtmlText(hDC, m_pManager, rc, sText, m_dwDisabledTextColor, \
				NULL, NULL, nLinks, m_iFont, m_uTextStyle);
			else
				CRenderEngine::DrawText(hDC, m_pManager, rc, sText, m_dwDisabledTextColor, \
				m_iFont, m_uTextStyle);
		}
	}

	bool CLabelUI::GetAutoCalcWidth() const
	{
		return m_bAutoCalcWidth;
	}

	void CLabelUI::SetAutoCalcWidth(bool bAutoCalcWidth)
	{
		m_bAutoCalcWidth = bAutoCalcWidth;
	}

	void CLabelUI::SetText( LPCTSTR pstrText )
	{
		CControlUI::SetText(pstrText);
		if(GetAutoCalcWidth()) {
			NeedParentUpdate();
		}
	}


	IMPLEMENT_DUICONTROL(CPieChartUI)
	CPieChartUI::CPieChartUI()
	{
		m_nRadius = 0;
		m_nStartAngle = 270;
		m_bCalcData = false;
		m_nDrawMinPercent = 6;
		m_nDecimal = 0;
		m_dwPieBorderColor = 0;
		m_nInnerRadius = 0;
		m_dwPieBkColor = 0;
		m_iPieFont = -1;
	}

	CPieChartUI::~CPieChartUI()
	{
		ClearDataItem();
	}

	LPCTSTR CPieChartUI::GetClass() const
	{
		return _T("PieChartUI");
	}

	LPVOID CPieChartUI::GetInterface(LPCTSTR pstrName)
	{
		if (_tcsicmp(pstrName, _T("PieChart")) == 0) return static_cast<CPieChartUI*>(this);
		return CLabelUI::GetInterface(pstrName);
	}

	CPieChartUI::_data_item_t* CPieChartUI::_FindDataItem(DWORD dwID, bool bCreate)
	{
		for (int i = 0; i < m_data.size(); i++)
		{
			if (m_data[i]->dwDataID == dwID)
			{
				return m_data[i];
			}
		}

		_data_item_t* pNew = new _data_item_t;
		pNew->dwDataID = dwID;
		m_data.push_back(pNew);
		return pNew;
	}

	void CPieChartUI::AddDataItem(DWORD dwDataID, DWORD dwColor, double dbValue)
	{
		_data_item_t* newItem = _FindDataItem(dwDataID, true);
		if (newItem)
		{
			newItem->m_dwColor = dwColor;
			newItem->m_dbValue = dbValue;
			m_bCalcData = false;
		}
		
	}

	void CPieChartUI::UpdateDataItem(DWORD dwData, double dbValue)
	{
		_data_item_t* newItem = _FindDataItem(dwData, false);
		if (newItem)
		{
			newItem->m_dbValue = dbValue;
			m_bCalcData = false;
		}
	}

	void CPieChartUI::ClearDataItem()
	{
		for (int i = 0; i < m_data.size(); i++)
		{
			delete m_data[i];
		}
		m_data.clear();
	}

	double CPieChartUI::GetValueTotal()
	{
		double dbTotal = 0.0;
		for (int i = 0; i < m_data.size(); i++)
		{
			dbTotal += m_data[i]->m_dbValue;
		}

		return dbTotal;
	}

	bool CPieChartUI::IsDataNull()
	{
		return GetValueTotal() <= 0.0;
	}

	void CPieChartUI::CalcData()
	{
		if (m_bCalcData)
		{
			return;
		}
		m_bCalcData = true;

		double dbTotal = GetValueTotal();
		if (dbTotal <= 0.0)
		{
			return;
		}
		double dbCurTotal = 0.0;
		for (int i = 0; i < m_data.size(); i++)
		{
			_data_item_t* pItem = m_data[i];
			if (NULL == pItem)
			{
				continue;
			}
 			dbCurTotal += pItem->m_dbValue;
			pItem->m_dbAngle = dbCurTotal / dbTotal * 360.0 + m_nStartAngle;
 			double dbPercent = pItem->m_dbValue / dbTotal * 100.0;
 			if (dbPercent < m_nDrawMinPercent)
			{
				pItem->m_strPercent.Empty();
 			}
			else
			{
				pItem->m_strPercent.Format(_T("%.*lf%%"), m_nDecimal, dbPercent);
				
			}
		}
	}

	static const double s_pie = 3.143592653;
	static void _AnglePoint(const POINT& org, int r, int nA, double f, POINT & pt)
	{
		double l = nA * s_pie / 180.0;
		double dbr = f * r;
		pt.x = org.x + dbr * cos(l);
		pt.y = org.y + dbr * sin(l);
	}

	void CPieChartUI::PaintText(HDC hDC)
	{
		RECT rc = m_rcItem;
		int nWidth = (rc.right - rc.left) / 2;
		int nHeight = (rc.bottom - rc.top) / 2;
		int nMin = min(nWidth, nHeight);
		if (m_nRadius <= 0)
		{
			m_nRadius = nMin;
			
		}
		int r = min(m_nRadius, nMin);
		
		if (r <= 0)
		{
			return;
		}

		CalcData();
		int nFont = m_iPieFont == -1 ? m_iFont : m_iPieFont;

		POINT ptOrg = {(rc.left + rc.right)/2, (rc.top + rc.bottom) / 2};
		int nLeft = ptOrg.x - r;
		int nTop = ptOrg.y - r;
		nWidth = r * 2;
		// draw
		Gdiplus::Graphics graphic(hDC);
		graphic.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
		if (IsDataNull())
		{
			if (m_dwPieBorderColor != 0)
			{
				Gdiplus::Pen pen(m_dwPieBorderColor);
				graphic.DrawEllipse(&pen, nLeft, nTop, nWidth, nWidth);
			}
		}
		else
		{
			double f = 0.75;
			double dbStart = m_nStartAngle;
			for (int i = 0; i < m_data.size(); i++)
			{
				_data_item_t* pItem = m_data[i];
				if (NULL == pItem)
				{
					continue;
				}

				int nDAngle = pItem->m_dbAngle - dbStart;
				Gdiplus::SolidBrush brush(pItem->m_dwColor);
				graphic.FillPie(&brush, nLeft, nTop, nWidth, nWidth, (int)dbStart, nDAngle);
				if (!pItem->m_strPercent.IsEmpty())
				{
					int nA = dbStart + nDAngle / 2;
					POINT ptText = ptOrg;
					_AnglePoint(ptOrg, r, nA, f, ptText);

					RECT rcCalc = { 0,0,0,0 };
					CRenderEngine::DrawText(hDC, m_pManager, rcCalc, pItem->m_strPercent, m_dwTextColor, nFont, DT_CALCRECT | DT_SINGLELINE | DT_LEFT  );
					SIZE sizeText = {rcCalc.right-rcCalc.left, rcCalc.bottom - rcCalc.top};

					//SIZE sizeText = CRenderEngine::GetTextSize(hDC, m_pManager, pItem->m_strPercent, m_iFont, DT_SINGLELINE | DT_LEFT | DT_VCENTER);
					
					RECT rcText = {ptText.x-sizeText.cx/2, ptText.y-sizeText.cy/2, 0, 0};
					rcText.right = rcText.left + sizeText.cx;
					rcText.bottom = rcText.top + sizeText.cy;
					CRenderEngine::DrawText(hDC, m_pManager, rcText, pItem->m_strPercent, m_dwTextColor, nFont, DT_SINGLELINE | DT_LEFT );
				}

				dbStart = pItem->m_dbAngle;
			}
		}

		if (m_nInnerRadius != 0 && m_nInnerRadius < r)
		{
			DWORD dwBkColor = m_dwPieBkColor == 0 ? m_dwBackColor : m_dwPieBkColor;
			if (dwBkColor == 0)
			{
				return;
			}
			r = m_nInnerRadius;
			nLeft = ptOrg.x - r;
			nTop = ptOrg.y - r;
			nWidth = r * 2;

			Gdiplus::SolidBrush brush(dwBkColor);
			graphic.FillEllipse(&brush, nLeft, nTop, nWidth, nWidth);

		}
	}

	void CPieChartUI::SetPieBorderColor(DWORD dwColor)
	{
		m_dwPieBorderColor = dwColor;
	}

	void CPieChartUI::SetRadius(int radius)
	{
		m_nRadius = radius;
	}

	void CPieChartUI::SetInnerRadius(int r)
	{
		m_nInnerRadius = r;
	}

	void CPieChartUI::SetPieBkColor(DWORD dwColor)
	{
		m_dwPieBkColor = dwColor;
	}

	void CPieChartUI::SetMinDrawPercent(int percent)
	{
		m_nDrawMinPercent = percent;
	}

	void CPieChartUI::SetDecimal(int nDecimal)
	{
		m_nDecimal = nDecimal;
	}

	void CPieChartUI::SetStartAngle(int nAngle)
	{
		m_nStartAngle = nAngle;
	}

	void CPieChartUI::SetPieFont(int iFont)
	{
		m_iPieFont = iFont;
	}

	void CPieChartUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if (_tcsicmp(pstrName, _T("radius")) == 0)
		{
			SetRadius(_ttoi(pstrValue));
		}
		else if (_tcsicmp(pstrName, _T("innerradius")) == 0)
		{
			SetInnerRadius(_ttoi(pstrValue));
		}
		else if (_tcsicmp(pstrName, _T("mindrawpercent")) == 0)
		{
			SetMinDrawPercent(_ttoi(pstrValue));
		}
		else if (_tcsicmp(pstrName, _T("decimal")) == 0)
		{
			SetDecimal(_ttoi(pstrValue));
		}
		else if (_tcsicmp(pstrName, _T("piefont")) == 0)
		{
			SetPieFont(_ttoi(pstrValue));
		}
		else if (_tcsicmp(pstrName, _T("startangle")) == 0)
		{
			SetStartAngle(_ttoi(pstrValue));
		}
		else if (_tcsicmp(pstrName, _T("piebordercolor")) == 0)
		{
			if (*pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			SetPieBorderColor(_tcstoul(pstrValue, &pstr, 16));
		}
		else if (_tcsicmp(pstrName, _T("piebkcolor")) == 0)
		{
			if (*pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			SetPieBkColor(_tcstoul(pstrValue, &pstr, 16));
		}
		else
		{
			CLabelUI::SetAttribute(pstrName, pstrValue);
		}
	}
}