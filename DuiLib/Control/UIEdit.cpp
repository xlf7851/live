#include "StdAfx.h"
#include "UIEdit.h"

namespace DuiLib
{
	class CEditWnd : public CWindowWnd
	{
	public:
		CEditWnd();

		void Init(CEditUI* pOwner);
		RECT CalPos();

		LPCTSTR GetWindowClassName() const;
		LPCTSTR GetSuperClassName() const;
		void OnFinalMessage(HWND hWnd);

		LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
		LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		LRESULT OnEditChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	protected:
		enum { 
			DEFAULT_TIMERID = 20,
		};

		CEditUI* m_pOwner;
		HBRUSH m_hBkBrush;
		bool m_bInit;
		bool m_bDrawCaret;
	};


	CEditWnd::CEditWnd() : m_pOwner(NULL), m_hBkBrush(NULL), m_bInit(false), m_bDrawCaret(false)
	{
	}

	void CEditWnd::Init(CEditUI* pOwner)
	{
		m_pOwner = pOwner;
		RECT rcPos = CalPos();
		UINT uStyle = 0;
		if(m_pOwner->GetManager()->IsLayered()) {
			uStyle = WS_POPUP | ES_AUTOHSCROLL | WS_VISIBLE;
			RECT rcWnd={0};
			::GetWindowRect(m_pOwner->GetManager()->GetPaintWindow(), &rcWnd);
			rcPos.left += rcWnd.left;
			rcPos.right += rcWnd.left;
			rcPos.top += rcWnd.top - 1;
			rcPos.bottom += rcWnd.top - 1;
		}
		else {
			uStyle = WS_CHILD | ES_AUTOHSCROLL;
		}
		UINT uTextStyle = m_pOwner->GetTextStyle();
		if(uTextStyle & DT_LEFT) uStyle |= ES_LEFT;
		else if(uTextStyle & DT_CENTER) uStyle |= ES_CENTER;
		else if(uTextStyle & DT_RIGHT) uStyle |= ES_RIGHT;
		if( m_pOwner->IsPasswordMode() ) uStyle |= ES_PASSWORD;
		Create(m_pOwner->GetManager()->GetPaintWindow(), NULL, uStyle, 0, rcPos);
		HFONT hFont=NULL;
		int iFontIndex=m_pOwner->GetFont();
		if (iFontIndex!=-1)
			hFont = m_pOwner->GetManager()->GetFont(iFontIndex);
		if (hFont == NULL)
			hFont = m_pOwner->GetManager()->GetDefaultFontInfo()->hFont;

		SetWindowFont(m_hWnd, hFont, TRUE);
		Edit_LimitText(m_hWnd, m_pOwner->GetMaxChar());
		if( m_pOwner->IsPasswordMode() ) Edit_SetPasswordChar(m_hWnd, m_pOwner->GetPasswordChar());
		Edit_SetText(m_hWnd, m_pOwner->GetText());
		Edit_SetModify(m_hWnd, FALSE);
		SendMessage(EM_SETMARGINS, EC_LEFTMARGIN | EC_RIGHTMARGIN, MAKELPARAM(0, 0));
		Edit_Enable(m_hWnd, m_pOwner->IsEnabled() == true);
		Edit_SetReadOnly(m_hWnd, m_pOwner->IsReadOnly() == true);

		//Styls
		LONG styleValue = ::GetWindowLong(m_hWnd, GWL_STYLE);
		styleValue |= pOwner->GetWindowStyls();
		::SetWindowLong(GetHWND(), GWL_STYLE, styleValue);
		//Styls
		::ShowWindow(m_hWnd, SW_SHOWNOACTIVATE);
		::SetFocus(m_hWnd);
		if (m_pOwner->IsAutoSelAll()) {
			int nSize = GetWindowTextLength(m_hWnd);
			if( nSize == 0 ) nSize = 1;
			Edit_SetSel(m_hWnd, 0, nSize);
		}
		else {
			int nSize = GetWindowTextLength(m_hWnd);
			Edit_SetSel(m_hWnd, nSize, nSize);
		}

		m_bInit = true;    
	}

	RECT CEditWnd::CalPos()
	{
		CDuiRect rcPos = m_pOwner->GetPos();
		RECT rcInset = m_pOwner->GetTextPadding();
		rcPos.left += rcInset.left;
		rcPos.top += rcInset.top;
		rcPos.right -= rcInset.right;
		rcPos.bottom -= rcInset.bottom;
		LONG lEditHeight = m_pOwner->GetManager()->GetFontInfo(m_pOwner->GetFont())->tm.tmHeight;
		if( lEditHeight < rcPos.GetHeight() ) {
			rcPos.top += (rcPos.GetHeight() - lEditHeight) / 2;
			rcPos.bottom = rcPos.top + lEditHeight;
		}

		CControlUI* pParent = m_pOwner;
		RECT rcParent;
		while( pParent = pParent->GetParent() ) {
			if( !pParent->IsVisible() ) {
				rcPos.left = rcPos.top = rcPos.right = rcPos.bottom = 0;
				break;
			}
			rcParent = pParent->GetClientPos();
			if( !::IntersectRect(&rcPos, &rcPos, &rcParent) ) {
				rcPos.left = rcPos.top = rcPos.right = rcPos.bottom = 0;
				break;
			}
		}

		return rcPos;
	}

	LPCTSTR CEditWnd::GetWindowClassName() const
	{
		return _T("EditWnd");
	}

	LPCTSTR CEditWnd::GetSuperClassName() const
	{
		return WC_EDIT;
	}

	void CEditWnd::OnFinalMessage(HWND hWnd)
	{
		m_pOwner->Invalidate();
		// Clear reference and die
		if( m_hBkBrush != NULL ) ::DeleteObject(m_hBkBrush);
		if (m_pOwner->GetManager()->IsLayered()) {
			m_pOwner->GetManager()->RemoveNativeWindow(hWnd);
		}
		m_pOwner->m_pWindow = NULL;
		delete this;
	}

	LRESULT CEditWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		if (m_pOwner)
		{
			bool bPreHandle = false;
			LRESULT l = m_pOwner->PreHandleMessage(uMsg, wParam, lParam, bPreHandle);
			if (bPreHandle)
			{
				return l;
			}
		}
		LRESULT lRes = 0;
		BOOL bHandled = TRUE;
		if( uMsg == WM_CREATE ) {
			//m_pOwner->GetManager()->AddNativeWindow(m_pOwner, m_hWnd);
			//if( m_pOwner->GetManager()->IsLayered() ) {
			//	::SetTimer(m_hWnd, DEFAULT_TIMERID, ::GetCaretBlinkTime(), NULL);
			//}
			bHandled = FALSE;
		}
		else if( uMsg == WM_KILLFOCUS ) lRes = OnKillFocus(uMsg, wParam, lParam, bHandled);
		else if( uMsg == OCM_COMMAND ) {
			if( GET_WM_COMMAND_CMD(wParam, lParam) == EN_CHANGE ) lRes = OnEditChanged(uMsg, wParam, lParam, bHandled);
			else if( GET_WM_COMMAND_CMD(wParam, lParam) == EN_UPDATE ) {
				RECT rcClient;
				::GetClientRect(m_hWnd, &rcClient);
				::InvalidateRect(m_hWnd, &rcClient, FALSE);
			}
		}
		else if( uMsg == WM_KEYDOWN && TCHAR(wParam) == VK_RETURN ){
			m_pOwner->GetManager()->SendNotify(m_pOwner, DUI_MSGTYPE_RETURN);
		}
		else if( uMsg == WM_KEYDOWN && TCHAR(wParam) == VK_TAB ){
			if (m_pOwner->GetManager()->IsLayered()) {
				m_pOwner->GetManager()->SetNextTabControl();
			}
		}
		else if( uMsg == OCM__BASE + WM_CTLCOLOREDIT  || uMsg == OCM__BASE + WM_CTLCOLORSTATIC ) {
			//if (m_pOwner->GetManager()->IsLayered() && !m_pOwner->GetManager()->IsPainting()) {
			//	m_pOwner->GetManager()->AddNativeWindow(m_pOwner, m_hWnd);
			//}
			DWORD clrColor = m_pOwner->GetNativeEditBkColor();
			if( clrColor == 0xFFFFFFFF ) return 0;
			::SetBkMode((HDC)wParam, TRANSPARENT);
			DWORD dwTextColor = m_pOwner->GetTextColor();
			::SetTextColor((HDC)wParam, RGB(GetBValue(dwTextColor),GetGValue(dwTextColor),GetRValue(dwTextColor)));
			if (clrColor < 0xFF000000) {
				if (m_hBkBrush != NULL) ::DeleteObject(m_hBkBrush);
				RECT rcWnd = m_pOwner->GetManager()->GetNativeWindowRect(m_hWnd);
				HBITMAP hBmpEditBk = CRenderEngine::GenerateBitmap(m_pOwner->GetManager(), rcWnd, m_pOwner, clrColor);
				m_hBkBrush = ::CreatePatternBrush(hBmpEditBk);
				::DeleteObject(hBmpEditBk);
			}
			else {
				if (m_hBkBrush == NULL) {
					m_hBkBrush = ::CreateSolidBrush(RGB(GetBValue(clrColor), GetGValue(clrColor), GetRValue(clrColor)));
				}
			}
			return (LRESULT)m_hBkBrush;
		}
		else if( uMsg == WM_PAINT) {
			//if (m_pOwner->GetManager()->IsLayered()) {
			//	m_pOwner->GetManager()->AddNativeWindow(m_pOwner, m_hWnd);
			//}
			bHandled = FALSE;
		}
		else if( uMsg == WM_PRINT ) {
			//if (m_pOwner->GetManager()->IsLayered()) {
			//	lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
			//	if( m_pOwner->IsEnabled() && m_bDrawCaret ) {
			//		RECT rcClient;
			//		::GetClientRect(m_hWnd, &rcClient);
			//		POINT ptCaret;
			//		::GetCaretPos(&ptCaret);
			//		RECT rcCaret = { ptCaret.x, ptCaret.y, ptCaret.x, ptCaret.y+rcClient.bottom-rcClient.top };
			//		CRenderEngine::DrawLine((HDC)wParam, rcCaret, 1, 0xFF000000);
			//	}
			//	return lRes;
			//}
			bHandled = FALSE;
		}
		else if( uMsg == WM_TIMER ) {
			if (wParam == CARET_TIMERID) {
				m_bDrawCaret = !m_bDrawCaret;
				RECT rcClient;
				::GetClientRect(m_hWnd, &rcClient);
				::InvalidateRect(m_hWnd, &rcClient, FALSE);
				return 0;
			}
			bHandled = FALSE;
		}
		else bHandled = FALSE;

		if( !bHandled ) return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
		return lRes;
	}

	LRESULT CEditWnd::OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		LRESULT lRes = ::DefWindowProc(m_hWnd, uMsg, wParam, lParam);
		//if ((HWND)wParam != m_pOwner->GetManager()->GetPaintWindow()) {
		//	::SendMessage(m_pOwner->GetManager()->GetPaintWindow(), WM_KILLFOCUS, wParam, lParam);
		//}
		PostMessage(WM_CLOSE);
		return lRes;
	}

	LRESULT CEditWnd::OnEditChanged(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		if( !m_bInit ) return 0;
		if( m_pOwner == NULL ) return 0;
		// Copy text back
		int cchLen = ::GetWindowTextLength(m_hWnd) + 1;
		LPTSTR pstr = static_cast<LPTSTR>(_alloca(cchLen * sizeof(TCHAR)));
		ASSERT(pstr);
		if( pstr == NULL ) return 0;
		::GetWindowText(m_hWnd, pstr, cchLen);
		m_pOwner->m_sText = pstr;
		m_pOwner->GetManager()->SendNotify(m_pOwner, DUI_MSGTYPE_TEXTCHANGED);
		if( m_pOwner->GetManager()->IsLayered() ) m_pOwner->Invalidate();
		return 0;
	}


	/////////////////////////////////////////////////////////////////////////////////////
	//
	//
	IMPLEMENT_DUICONTROL(CEditUI)

	CEditUI::CEditUI() : m_pWindow(NULL), m_uMaxChar(255), m_bReadOnly(false), 
		m_bPasswordMode(false), m_cPasswordChar(_T('*')), m_bAutoSelAll(false), m_uButtonState(0), 
		m_dwEditbkColor(0xFFFFFFFF), m_dwEditTextColor(0x00000000), m_iWindowStyls(0),m_dwTipValueColor(0xFFBAC0C5), m_pPreMessageHandler(NULL)
	{
		SetTextPadding(CDuiRect(4, 3, 4, 3));
		SetBkColor(0xFFFFFFFF);
		m_nSetDefaultBkAndTextColorStatus = -1;
	}

	LPCTSTR CEditUI::GetClass() const
	{
		return _T("EditUI");
	}

	LPVOID CEditUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcsicmp(pstrName, DUI_CTR_EDIT) == 0 ) return static_cast<CEditUI*>(this);
		return CLabelUI::GetInterface(pstrName);
	}

	UINT CEditUI::GetControlFlags() const
	{
		if( !IsEnabled() ) return CControlUI::GetControlFlags();

		return UIFLAG_SETCURSOR | UIFLAG_TABSTOP;
	}

	void CEditUI::DoEvent(TEventUI& event)
	{
		if( !IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND ) {
			if( m_pParent != NULL ) m_pParent->DoEvent(event);
			else CLabelUI::DoEvent(event);
			return;
		}

		if( event.Type == UIEVENT_SETCURSOR && IsEnabled() )
		{
			::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_IBEAM)));
			return;
		}
		if( event.Type == UIEVENT_WINDOWSIZE )
		{
			if( m_pWindow != NULL ) m_pManager->SetFocusNeeded(this);
		}
		if( event.Type == UIEVENT_SCROLLWHEEL )
		{
			if( m_pWindow != NULL ) return;
		}
		if( event.Type == UIEVENT_SETFOCUS && IsEnabled() ) 
		{
			if( m_pWindow ) return;
			m_pWindow = new CEditWnd();
			ASSERT(m_pWindow);
			m_pWindow->Init(this);
			Invalidate();
		}
		if( event.Type == UIEVENT_KILLFOCUS && IsEnabled() ) 
		{
			Invalidate();
		}
		if( event.Type == UIEVENT_BUTTONDOWN || event.Type == UIEVENT_DBLCLICK || event.Type == UIEVENT_RBUTTONDOWN) 
		{
			if( IsEnabled() ) {
				GetManager()->ReleaseCapture();
				if( IsFocused() && m_pWindow == NULL )
				{
					m_pWindow = new CEditWnd();
					ASSERT(m_pWindow);
					m_pWindow->Init(this);

					if( PtInRect(&m_rcItem, event.ptMouse) )
					{
						int nSize = GetWindowTextLength(*m_pWindow);
						if( nSize == 0 ) nSize = 1;
						Edit_SetSel(*m_pWindow, 0, nSize);
					}
				}
				else if( m_pWindow != NULL )
				{
					if (!m_bAutoSelAll) {
						POINT pt = event.ptMouse;
						pt.x -= m_rcItem.left + m_rcTextPadding.left;
						pt.y -= m_rcItem.top + m_rcTextPadding.top;
						Edit_SetSel(*m_pWindow, 0, 0);
						::SendMessage(*m_pWindow, WM_LBUTTONDOWN, event.wParam, MAKELPARAM(pt.x, pt.y));
					}
				}
			}
			return;
		}
		if( event.Type == UIEVENT_MOUSEMOVE ) 
		{
			return;
		}
		if( event.Type == UIEVENT_BUTTONUP ) 
		{
			return;
		}
		if( event.Type == UIEVENT_CONTEXTMENU )
		{
			return;
		}
		if( event.Type == UIEVENT_MOUSEENTER )
		{
			if( ::PtInRect(&m_rcItem, event.ptMouse ) ) {
                if( IsEnabled() ) {
                    if( (m_uButtonState & UISTATE_HOT) == 0  ) {
                        m_uButtonState |= UISTATE_HOT;
                        Invalidate();
                    }
                }
            }
		}
		if( event.Type == UIEVENT_MOUSELEAVE )
		{
			if( IsEnabled() ) {
				m_uButtonState &= ~UISTATE_HOT;
				Invalidate();
			}
			return;

			//if( !::PtInRect(&m_rcItem, event.ptMouse ) ) {
   //             if( IsEnabled() ) {
   //                 if( (m_uButtonState & UISTATE_HOT) != 0  ) {
   //                     m_uButtonState &= ~UISTATE_HOT;
   //                     Invalidate();
   //                 }
   //             }
   //             if (m_pManager) m_pManager->RemoveMouseLeaveNeeded(this);
   //         }
   //         else {
   //             if (m_pManager) m_pManager->AddMouseLeaveNeeded(this);
   //             return;
   //         }
		}
		CLabelUI::DoEvent(event);
	}

	void CEditUI::SetEnabled(bool bEnable)
	{
		CControlUI::SetEnabled(bEnable);
		if( !IsEnabled() ) {
			m_uButtonState = 0;
		}
	}

	void CEditUI::SetText(LPCTSTR pstrText)
	{
		m_sText = pstrText;
		if( m_pWindow != NULL ) Edit_SetText(*m_pWindow, m_sText);
		Invalidate();
	}

	void CEditUI::SetMaxChar(UINT uMax)
	{
		m_uMaxChar = uMax;
		if( m_pWindow != NULL ) Edit_LimitText(*m_pWindow, m_uMaxChar);
	}

	UINT CEditUI::GetMaxChar()
	{
		return m_uMaxChar;
	}

	void CEditUI::SetReadOnly(bool bReadOnly)
	{
		if( m_bReadOnly == bReadOnly ) return;

		m_bReadOnly = bReadOnly;
		if( m_pWindow != NULL ) Edit_SetReadOnly(*m_pWindow, m_bReadOnly);
		Invalidate();
	}

	bool CEditUI::IsReadOnly() const
	{
		return m_bReadOnly;
	}

	void CEditUI::SetNumberOnly(bool bNumberOnly)
	{
		if( bNumberOnly )
		{
			m_iWindowStyls |= ES_NUMBER;
		}
		else
		{
			m_iWindowStyls &= ~ES_NUMBER;
		}
	}

	bool CEditUI::IsNumberOnly() const
	{
		return (m_iWindowStyls & ES_NUMBER) ? true:false;
	}

	int CEditUI::GetWindowStyls() const 
	{
		return m_iWindowStyls;
	}

	void CEditUI::SetPasswordMode(bool bPasswordMode)
	{
		if( m_bPasswordMode == bPasswordMode ) return;
		m_bPasswordMode = bPasswordMode;
		Invalidate();
		if( m_pWindow != NULL ) {
			LONG styleValue = ::GetWindowLong(*m_pWindow, GWL_STYLE);
			bPasswordMode ? styleValue |= ES_PASSWORD : styleValue &= ~ES_PASSWORD;
			::SetWindowLong(*m_pWindow, GWL_STYLE, styleValue);
		}
	}

	bool CEditUI::IsPasswordMode() const
	{
		return m_bPasswordMode;
	}

	void CEditUI::SetPasswordChar(TCHAR cPasswordChar)
	{
		if( m_cPasswordChar == cPasswordChar ) return;
		m_cPasswordChar = cPasswordChar;
		if( m_pWindow != NULL ) Edit_SetPasswordChar(*m_pWindow, m_cPasswordChar);
		Invalidate();
	}

	TCHAR CEditUI::GetPasswordChar() const
	{
		return m_cPasswordChar;
	}

	LPCTSTR CEditUI::GetNormalImage()
	{
		return m_sNormalImage;
	}

	void CEditUI::SetNormalImage(LPCTSTR pStrImage)
	{
		m_sNormalImage = pStrImage;
		Invalidate();
	}

	LPCTSTR CEditUI::GetHotImage()
	{
		return m_sHotImage;
	}

	void CEditUI::SetHotImage(LPCTSTR pStrImage)
	{
		m_sHotImage = pStrImage;
		Invalidate();
	}

	LPCTSTR CEditUI::GetFocusedImage()
	{
		return m_sFocusedImage;
	}

	void CEditUI::SetFocusedImage(LPCTSTR pStrImage)
	{
		m_sFocusedImage = pStrImage;
		Invalidate();
	}

	LPCTSTR CEditUI::GetDisabledImage()
	{
		return m_sDisabledImage;
	}

	void CEditUI::SetDisabledImage(LPCTSTR pStrImage)
	{
		m_sDisabledImage = pStrImage;
		Invalidate();
	}

	void CEditUI::SetNativeEditBkColor(DWORD dwBkColor)
	{
		m_dwEditbkColor = dwBkColor;
	}

	DWORD CEditUI::GetNativeEditBkColor() const
	{
		return m_dwEditbkColor;
	}

	void CEditUI::SetNativeEditTextColor( LPCTSTR pStrColor )
	{
		if( *pStrColor == _T('#')) pStrColor = ::CharNext(pStrColor);
		LPTSTR pstr = NULL;
		DWORD clrColor = _tcstoul(pStrColor, &pstr, 16);

		m_dwEditTextColor = clrColor;
	}

	DWORD CEditUI::GetNativeEditTextColor() const
	{
		return m_dwEditTextColor;
	}
	
	bool CEditUI::IsAutoSelAll()
	{
		return m_bAutoSelAll;
	}

	void CEditUI::SetAutoSelAll(bool bAutoSelAll)
	{
		m_bAutoSelAll = bAutoSelAll;
	}

	void CEditUI::SetSel(long nStartChar, long nEndChar)
	{
		if( m_pWindow != NULL ) Edit_SetSel(*m_pWindow, nStartChar,nEndChar);
	}

	void CEditUI::GetSel(long &nStartChar, long& nEndChar)
	{
		if (m_pWindow)
		{
			DWORD dwRet	= Edit_GetSel(*m_pWindow);
			nStartChar = LOWORD(dwRet);
			nEndChar = HIWORD(dwRet);
		}
	}

	void CEditUI::SetSelAll()
	{
		SetSel(0,-1);
	}

	void CEditUI::SetReplaceSel(LPCTSTR lpszReplace)
	{
		if( m_pWindow != NULL ) Edit_ReplaceSel(*m_pWindow, lpszReplace);
	}

	void CEditUI::SetTipValue( LPCTSTR pStrTipValue )
	{
		m_sTipValue	= pStrTipValue;
	}

	LPCTSTR CEditUI::GetTipValue()
	{
		if (!IsResourceText()) return m_sTipValue;
		return CResourceManager::GetInstance()->GetText(m_sTipValue);
	}
	                    
	void CEditUI::SetTipValueColor( LPCTSTR pStrColor )
	{
		if( *pStrColor == _T('#')) pStrColor = ::CharNext(pStrColor);
		LPTSTR pstr = NULL;
		DWORD clrColor = _tcstoul(pStrColor, &pstr, 16);

		m_dwTipValueColor = clrColor;
	}

	DWORD CEditUI::GetTipValueColor()
	{
		return m_dwTipValueColor;
	}
	
	
	void CEditUI::SetPos(RECT rc, bool bNeedInvalidate)
	{
		CControlUI::SetPos(rc, bNeedInvalidate);
		if( m_pWindow != NULL ) {
			RECT rcPos = m_pWindow->CalPos();
			::SetWindowPos(m_pWindow->GetHWND(), NULL, rcPos.left, rcPos.top, rcPos.right - rcPos.left, 
				rcPos.bottom - rcPos.top, SWP_NOZORDER | SWP_NOACTIVATE);        
		}
	}

	void CEditUI::Move(SIZE szOffset, bool bNeedInvalidate)
	{
		CControlUI::Move(szOffset, bNeedInvalidate);
		if( m_pWindow != NULL ) {
			RECT rcPos = m_pWindow->CalPos();
			::SetWindowPos(m_pWindow->GetHWND(), NULL, rcPos.left, rcPos.top, rcPos.right - rcPos.left, 
				rcPos.bottom - rcPos.top, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);        
		}
	}

	void CEditUI::SetVisible(bool bVisible)
	{
		CControlUI::SetVisible(bVisible);
		if( !IsVisible() && m_pWindow != NULL ) m_pManager->SetFocus(NULL);
	}

	void CEditUI::SetInternVisible(bool bVisible)
	{
		if( !IsVisible() && m_pWindow != NULL ) m_pManager->SetFocus(NULL);
	}

	SIZE CEditUI::EstimateSize(SIZE szAvailable)
	{
		if( m_cxyFixed.cy == 0 ) return CDuiSize(m_cxyFixed.cx, m_pManager->GetFontInfo(GetFont())->tm.tmHeight + 6);
		return CControlUI::EstimateSize(szAvailable);
	}

	void CEditUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if( _tcsicmp(pstrName, _T("readonly")) == 0 ) SetReadOnly(_tcsicmp(pstrValue, _T("true")) == 0);
		else if( _tcsicmp(pstrName, _T("numberonly")) == 0 ) SetNumberOnly(_tcsicmp(pstrValue, _T("true")) == 0);
		else if( _tcscmp(pstrName, _T("autoselall")) == 0 ) SetAutoSelAll(_tcscmp(pstrValue, _T("true")) == 0);	
		else if( _tcsicmp(pstrName, _T("password")) == 0 ) SetPasswordMode(_tcsicmp(pstrValue, _T("true")) == 0);
		else if( _tcsicmp(pstrName, _T("passwordchar")) == 0 ) SetPasswordChar(*pstrValue);
		else if( _tcsicmp(pstrName, _T("maxchar")) == 0 ) SetMaxChar(_ttoi(pstrValue));
		else if( _tcsicmp(pstrName, _T("normalimage")) == 0 ) SetNormalImage(pstrValue);
		else if( _tcsicmp(pstrName, _T("hotimage")) == 0 ) SetHotImage(pstrValue);
		else if( _tcsicmp(pstrName, _T("focusedimage")) == 0 ) SetFocusedImage(pstrValue);
		else if( _tcsicmp(pstrName, _T("disabledimage")) == 0 ) SetDisabledImage(pstrValue);
		else if( _tcsicmp(pstrName, _T("tipvalue")) == 0 ) SetTipValue(pstrValue);
		else if( _tcsicmp(pstrName, _T("tipvaluecolor")) == 0 ) SetTipValueColor(pstrValue);
		else if( _tcsicmp(pstrName, _T("nativetextcolor")) == 0 ) SetNativeEditTextColor(pstrValue);
		else if( _tcsicmp(pstrName, _T("nativebkcolor")) == 0 ) {
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetNativeEditBkColor(clrColor);
		}
		else if (_tcsicmp(pstrName, _T("usedefaultbkandtextcolor")) == 0) {
			if (_tcsicmp(pstrValue, _T("true")) == 0)
			{
				SetUseDefaultBkAndTextColor();
			}
			
		}
		else CLabelUI::SetAttribute(pstrName, pstrValue);
	}

	void CEditUI::PaintStatusImage(HDC hDC)
	{
		if( IsFocused() ) m_uButtonState |= UISTATE_FOCUSED;
		else m_uButtonState &= ~ UISTATE_FOCUSED;
		if( !IsEnabled() ) m_uButtonState |= UISTATE_DISABLED;
		else m_uButtonState &= ~ UISTATE_DISABLED;

		if( (m_uButtonState & UISTATE_DISABLED) != 0 ) {
			if( !m_sDisabledImage.IsEmpty() ) {
				if( !DrawImage(hDC, (LPCTSTR)m_sDisabledImage) ) {}
				else return;
			}
		}
		else if( (m_uButtonState & UISTATE_FOCUSED) != 0 ) {
			if( !m_sFocusedImage.IsEmpty() ) {
				if( !DrawImage(hDC, (LPCTSTR)m_sFocusedImage) ) {}
				else return;
			}
		}
		else if( (m_uButtonState & UISTATE_HOT) != 0 ) {
			if( !m_sHotImage.IsEmpty() ) {
				if( !DrawImage(hDC, (LPCTSTR)m_sHotImage) ) {}
				else return;
			}
		}

		if( !m_sNormalImage.IsEmpty() ) {
			if( !DrawImage(hDC, (LPCTSTR)m_sNormalImage) ) {}
			else return;
		}
	}

	void CEditUI::PaintText(HDC hDC)
	{
		DWORD mCurTextColor = m_dwTextColor;

		if( m_dwTextColor == 0 ) mCurTextColor = m_dwTextColor = m_pManager->GetDefaultFontColor();		
		if( m_dwDisabledTextColor == 0 ) m_dwDisabledTextColor = m_pManager->GetDefaultDisabledColor();

		CDuiString sDrawText = GetText();
		CDuiString sTipValue = GetTipValue();
		if(sDrawText == sTipValue || sDrawText == _T("")) {
			mCurTextColor = m_dwTipValueColor;
			sDrawText = sTipValue;
		}
		else {
			CDuiString sTemp = sDrawText;
			if( m_bPasswordMode ) {
				sDrawText.Empty();
				LPCTSTR pStr = sTemp.GetData();
				while( *pStr != _T('\0') ) {
					sDrawText += m_cPasswordChar;
					pStr = ::CharNext(pStr);
				}
			}
		}

		RECT rc = m_rcItem;
		rc.left += m_rcTextPadding.left;
		rc.right -= m_rcTextPadding.right;
		rc.top += m_rcTextPadding.top;
		rc.bottom -= m_rcTextPadding.bottom;
		if( IsEnabled() ) {
			CRenderEngine::DrawText(hDC, m_pManager, rc, sDrawText, mCurTextColor, \
				m_iFont, DT_SINGLELINE | m_uTextStyle);
		}
		else {
			CRenderEngine::DrawText(hDC, m_pManager, rc, sDrawText, m_dwDisabledTextColor, \
				m_iFont, DT_SINGLELINE | m_uTextStyle);
		}
	}

	void CEditUI::PaintBkColor(HDC hDC)
	{
		if (m_nSetDefaultBkAndTextColorStatus == 0)
		{
			SetUseDefaultBkAndTextColor();
		}

		CLabelUI::PaintBkColor(hDC);
	}

	LRESULT CEditUI::PreHandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
	{
		if (m_pPreMessageHandler)
		{
			return m_pPreMessageHandler->EditMessageHandler(uMsg, wParam, lParam, bHandled);
		}
		return 0;
	}

	void CEditUI::SetPreMessageHandler(IEditPreMessageHandler* pHandler)
	{
		m_pPreMessageHandler = pHandler;
	}

	void CEditUI::SetUseDefaultBkAndTextColor()
	{
		if (m_nSetDefaultBkAndTextColorStatus > 0)
		{
			return;
		}
		m_nSetDefaultBkAndTextColorStatus = 0;
		if (m_pManager)
		{
			m_nSetDefaultBkAndTextColorStatus = 1;
			DWORD dwBkColor = 0;
			CControlUI* pParent = GetParent();
			while (pParent)
			{
				dwBkColor = pParent->GetBkColor();
				if (dwBkColor != 0)
				{
					break;
				}
				pParent = pParent->GetParent();
			}
			if (dwBkColor == 0)
			{
				dwBkColor = m_pManager->GetDefaultBkColor();
			}

			if (dwBkColor != 0)
			{
				m_dwEditbkColor = dwBkColor;
				SetBkColor(m_dwEditbkColor);
			}
		

			DWORD dwTextColor = m_pManager->GetDefaultFontColor();
			if (dwTextColor != 0)
			{
				m_dwEditTextColor = dwTextColor;
				SetTextColor(dwTextColor);
			}
		}
	}


	IMPLEMENT_DUICONTROL(CEditContainerItemLabelUI)
		CEditContainerItemLabelUI::CEditContainerItemLabelUI()
	{
		
	}

	CEditContainerItemLabelUI::~CEditContainerItemLabelUI()
	{

	}

	LPCTSTR CEditContainerItemLabelUI::GetClass() const
	{
		return _T("EditContainerItemLabelUI");
	}

	LPVOID CEditContainerItemLabelUI::GetInterface(LPCTSTR pstrName)
	{
		if (_tcsicmp(pstrName, _T("EditContainerItemLabel")) == 0) return static_cast<CEditContainerItemLabelUI*>(this);
		return CLabelUI::GetInterface(pstrName);
	}



	void CEditContainerItemLabelUI::DoEvent(TEventUI& event)
	{
		if (m_pParent)
		{
			m_pParent->DoEvent(event);
		}
	}


	IMPLEMENT_DUICONTROL(CEditContainerItemUI)
	CEditContainerItemUI::CEditContainerItemUI()
	{
		m_bEditMode = false;
		m_pEditContainer = NULL;
		m_nBackEditWidth = 10;
		m_pEdit = new CEditUI;
		m_pEdit->SetPreMessageHandler(this);
		m_pEdit->SetReadOnly(true);
		m_pEdit->SetFixedWidth(m_nBackEditWidth);
		Add(m_pEdit);
	}

	void CEditContainerItemUI::SetEditWidth(int nWidth)
	{
		m_nBackEditWidth = nWidth;
		if (m_pEdit)
		{
			m_pEdit->SetFixedWidth(nWidth);
		}
	}

	void CEditContainerItemUI::EnableEdit(bool bEnable)
	{
		if (m_pEdit)
		{
			m_pEdit->SetReadOnly(!bEnable);
		}
	}

	void CEditContainerItemUI::SetEditFocus()
	{
		if (m_pEdit)
		{
			m_pEdit->SetFocus();
		}
	}

	CEditUI* CEditContainerItemUI::GetEdit()
	{
		return m_pEdit;
	}

	void CEditContainerItemUI::SetEditContainer(IEditContainerUI* pContainer)
	{
		m_pEditContainer = pContainer;
	}
	
	LRESULT CEditContainerItemUI::EditMessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
	{
		if (m_pEditContainer)
		{
			return m_pEditContainer->EditMessageHandler(this, uMsg, wParam, lParam, bHandled);
		}

		return 0;
	}

	CEditContainerItemUI::~CEditContainerItemUI()
	{

	}

	LPCTSTR CEditContainerItemUI::GetClass() const
	{
		return _T("EditContainerItemUI");
	}

	LPVOID CEditContainerItemUI::GetInterface(LPCTSTR pstrName)
	{
		if (_tcsicmp(pstrName, DUI_CTR_EDIT_CONTAINER_ITEM) == 0) return static_cast<CEditContainerItemUI*>(this);
		return CHorizontalLayoutUI::GetInterface(pstrName);
	}

	void CEditContainerItemUI::SetControl(CLabelUI* pControl)
	{
		if (GetCount() > 1)
		{
			RemoveAt(0);
		}

		AddAt(pControl, 0);
	}

	void CEditContainerItemUI::ClearControl(bool bCopyTextToEdit)
	{
		CDuiString strText;
		if (bCopyTextToEdit)
		{
			strText = GetItemText();
		}
		if (GetCount() > 1)
		{
			RemoveAt(0);
		}

		if (m_pEdit)
		{
			if (bCopyTextToEdit)
			{
				m_pEdit->SetText(strText);
			}
			m_pEdit->SetReadOnly(false);
			m_pEdit->SetFixedWidth(0);
		}
	}

	void CEditContainerItemUI::DoEvent(TEventUI& event)
	{
		if (event.Type == UIEVENT_DBLCLICK)
		{
			if (m_pEditContainer)
			{
				m_pEditContainer->OnDbClickItem(this);
			}
		}
		else
		{
			CHorizontalLayoutUI::DoEvent(event);
		}
	}

	void CEditContainerItemUI::SetEditMode()
	{
		if (m_bEditMode)
		{
			return;
		}

		CLabelUI* pLabel = GetLabel();
		if (NULL == pLabel || NULL == m_pEdit)
		{
			return;
		}

		
		m_bEditMode = true;
		

		SIZE szV = { m_rcItem.right - m_rcItem.left - m_rcInset.left - m_rcInset.right, m_rcItem.bottom - m_rcItem.top - m_rcInset.top - m_rcInset.bottom };
		if (szV.cx < 0)
		{
			szV.cx = 0;
		}

		if (szV.cy < 0)
		{
			szV.cy = 0;
		}
		SIZE sz = EstimateSize(szV);
		CDuiString strText = pLabel->GetText();
		pLabel->SetVisible(false);
		m_pEdit->SetFixedWidth(sz.cx);
		m_pEdit->SetReadOnly(false);
		

		RECT rc = m_rcItem;
		rc.left += m_rcInset.left;
		rc.right = rc.left + szV.cx;
		rc.top += m_rcInset.top;
		rc.bottom = rc.top + szV.cy;

		m_pEdit->SetPos(rc, false);
		m_pEdit->SetText(strText);
		m_pEdit->SetFocus();
		m_pEdit->SetSelAll();
		
		
	}

	void CEditContainerItemUI::ClearEditMode()
	{
		m_bEditMode = false;
	}

	void CEditContainerItemUI::EndEditMode()
	{
		if (!m_bEditMode)
		{
			return;
		}
		m_bEditMode = false;

		CLabelUI* pLabel = GetLabel();
		if (NULL == pLabel || NULL == m_pEdit)
		{
			return;
		}

		
		pLabel->SetVisible(true);
		m_pEdit->SetFixedWidth(m_nBackEditWidth);
		m_pEdit->SetReadOnly(true);
		CDuiString strText = m_pEdit->GetText();
		m_pEdit->SetText(_T(""));
		pLabel->SetText(strText);
	}

	bool CEditContainerItemUI::IsEditMode()
	{
		return m_bEditMode;
	}

	bool CEditContainerItemUI::IsOnlyEdit()
	{
		return GetCount() == 1;
	}

	void CEditContainerItemUI::AutoEditWidth()
	{
		if (m_pEdit)
		{
			if (m_pEdit->IsReadOnly())
			{
				m_pEdit->SetFixedWidth(m_nBackEditWidth);
			}
		}
	}

	void CEditContainerItemUI::SetLabelMode()
	{
		if (m_pEdit)
		{
			m_pEdit->SetText(_T(""));
			m_pEdit->SetReadOnly(true);
			m_pEdit->SetFixedWidth(m_nBackEditWidth);
		}
	}

	SIZE CEditContainerItemUI::EstimateSize(SIZE szAvailable)
	{
		CControlUI* pFirst = NULL;
		if (GetCount() > 1)
		{
			pFirst = GetItemAt(0);
		}

		SIZE sz = {0, 0};

		if (pFirst && pFirst->IsVisible())
		{
			sz = pFirst->EstimateSize(szAvailable);
		}

		
		if (m_pEdit)
		{
			SIZE  szEdit = m_pEdit->EstimateSize(szAvailable);
			sz.cx += szEdit.cx;
		}

		if (sz.cx == 0)
		{
			sz.cx = m_cxyFixed.cx;
		}
		sz.cy = m_cxyFixed.cy;

		if (m_pManager != NULL)
			return m_pManager->GetDPIObj()->Scale(sz);
		
		return sz;
	}

	CLabelUI* CEditContainerItemUI::GetLabel()
	{
		CControlUI* pControl = NULL;
		if (GetCount() > 1)
		{
			pControl = GetItemAt(0);
		}

		return pControl ? static_cast<CLabelUI*>(pControl->GetInterface(DUI_CTR_LABEL)) : NULL;
	}

	CDuiString CEditContainerItemUI::GetItemText()
	{
		CLabelUI* pLabel = GetLabel();
		if (pLabel)
		{
			return pLabel->GetText();
		}

		return _T("");
	}

	void CEditContainerItemUI::SetItemText(LPCTSTR lpszText)
	{
		CLabelUI* pLabel = GetLabel();
		if (pLabel)
		{
			pLabel->SetText(lpszText);
		}
	}

	void CEditContainerItemUI::SetEditStyle(LPCTSTR lpszStyle)
	{
		if (m_pEdit)
		{
			m_pEdit->ApplyAttributeList(lpszStyle);
		}
	}

	void CEditContainerItemUI::SetControlStyle(LPCTSTR lpszStyle)
	{
		if (GetCount() > 1)
		{
			CControlUI* pItem = GetItemAt(0);
			if (pItem)
			{
				pItem->ApplyAttributeList(lpszStyle);
			}
		}
	}

	void CEditContainerItemUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if (_tcsicmp(pstrName, _T("eci_editwidth")) == 0)
		{
			SetEditWidth(_ttoi(pstrValue));
		}
		else
		{
			CHorizontalLayoutUI::SetAttribute(pstrName, pstrValue);
		}
	}

	IMPLEMENT_DUICONTROL(CEditContainerUI)
		CEditContainerUI::CEditContainerUI()
	{
		SetChildVAlign(DT_VCENTER);
		m_pCurrentEditItem = NULL;
		m_nMinLastEditWidth = 36;
		m_pPreEditMessageHandler = NULL;
		m_pItemControlCreater = NULL;
		m_bCanItemTextRepeat = true;
		m_bContainerModify = false;
		m_pCurrentEditItem = NULL;
		AddEditControlItem(_T(""));
	}

	CEditContainerUI::~CEditContainerUI()
	{
		CPaintManagerUI* pManager = GetManager();
		if (pManager)
		{
			pManager->RemoveNotifier(this);
		}
	}

	LPCTSTR CEditContainerUI::GetClass() const
	{
		return _T("EditContainerUI");
	}

	LPVOID CEditContainerUI::GetInterface(LPCTSTR pstrName)
	{
		if (_tcsicmp(pstrName, DUI_CTR_EDIT_CONTAINER) == 0) return static_cast<CEditContainerUI*>(this);
		return CHorizontalLayoutUI::GetInterface(pstrName);
	}

	void CEditContainerUI::DoEvent(TEventUI& event)
	{
		CHorizontalLayoutUI::DoEvent(event);
	}

	void CEditContainerUI::DoInit()
	{
		CPaintManagerUI* pManager = GetManager();
		if (pManager)
		{
			pManager->AddNotifier(this);
		}

		CHorizontalLayoutUI::DoInit();
	}

	void CEditContainerUI::Notify(TNotifyUI& msg)
	{
		if (msg.pSender == this)
		{
			if (msg.sType == _T("editContainerItemKillFocus"))
			{
				CEditContainerItemUI* pItem = (CEditContainerItemUI*)msg.wParam;
				if (pItem)
				{
					OnAfterEditContainerItemKillFocus(pItem, (bool)LOBYTE(msg.lParam), (bool)HIBYTE(msg.lParam));
				}
			}
			else if (msg.sType == _T("editContainerDeleteItem"))
			{
				CEditContainerItemUI* pItem = (CEditContainerItemUI*)msg.wParam;
				if (pItem)
				{
					RemoveEditContainerItem(pItem);
				}
			}
		}
	}

	void CEditContainerUI::OnDbClickItem(CEditContainerItemUI* pItem)
	{
		int nIndex = GetEditContainerItemIndex(pItem);
		if (nIndex < 0)
		{
			return;
		}

		m_pCurrentEditItem = pItem;

		if (nIndex == GetCount() -1)	// the last one
		{
			pItem->ClearControl(true);
			pItem->SetEditFocus();
		}
		else
		{
			pItem->SetEditMode();
		}
	}

	void CEditContainerUI::SetItemControlCreater(IEditContainerItemCreater* pCreater)
	{
		m_pItemControlCreater = pCreater;
	}

	void CEditContainerUI::SetPreEditMessageHandler(IEditContainerEditMessageHandler* pHandler)
	{
		m_pPreEditMessageHandler = pHandler;
	}

	LRESULT CEditContainerUI::EditMessageHandler(CEditContainerItemUI* pItem, UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
	{
		int nIndex = GetEditContainerItemIndex(pItem);
		if (nIndex < 0)
		{
			return 0;
		}

		CEditUI* pEdit = pItem->GetEdit();
		if (NULL == pEdit)
		{
			return 0;
		}

		int nCount = GetCount();
		LRESULT lRes = 0;

		if (uMsg == WM_KEYDOWN)
		{
			TCHAR cVK = TCHAR(wParam);

			bHandled = OnEditKeydown(nIndex, pItem, pEdit, cVK);
		}
		else if (uMsg == WM_KILLFOCUS)
		{
			OnEditKillFocus(nIndex, pItem, pEdit);
		}
		

		return lRes;
	}

	void CEditContainerUI::OnEditKillFocus(int index, CEditContainerItemUI* pItem, CEditUI* pEdit)
	{
		bool bEditMode = pItem->IsEditMode();
		if (NULL == m_pPreEditMessageHandler || m_pPreEditMessageHandler->OnEditKillFocus(this, index, pItem, pEdit) == false)
		{
			if (bEditMode && pItem == m_pCurrentEditItem)
			{
				pItem->EndEditMode();
			}
		}
		

		if (m_pManager)
		{
			LPARAM lParam = MAKEWORD((BYTE)bEditMode, (BYTE)(pItem == m_pCurrentEditItem));
			m_pManager->SendNotify(this, _T("editContainerItemKillFocus"), (WPARAM)pItem, lParam, true);
		}
		m_pCurrentEditItem = NULL;
	}

	bool CEditContainerUI::OnEditKeydown(int nIndex, CEditContainerItemUI* pItem, CEditUI* pEdit, TCHAR uKey)
	{
		bool bRet = false;
		switch (uKey)
		{
		case VK_RETURN:
			bRet = OnEditReturnKeydown(nIndex, pItem, pEdit);
			break;
		case VK_BACK:
			bRet = OnEditBackKeydown(nIndex, pItem, pEdit);
			break;
		case VK_LEFT:
			bRet = OnEditLeftKeydown(nIndex, pItem, pEdit);
			break;
		case VK_RIGHT:
			bRet = OnEditRightKeydown(nIndex, pItem, pEdit);
			break;
		case VK_UP:
			bRet = OnEditUpKeydown(nIndex, pItem, pEdit);
			break;
		case VK_DOWN:
			bRet = OnEditDownKeydown(nIndex, pItem, pEdit);
			break;
		case VK_TAB:
			bRet = true;
			break;
		default:
			break;
		}

		return bRet;
	}

	bool CEditContainerUI::OnEditReturnKeydown(int index, CEditContainerItemUI* pItem, CEditUI* pEdit)
	{
		bool bHandle = false;
		if (m_pPreEditMessageHandler)
		{
			bHandle = m_pPreEditMessageHandler->OnEditReturnKeydown(this, index, pItem, pEdit);
		}

		if (!bHandle)
		{
			CDuiString strText = pEdit->GetText();
			int nCount = GetCount();
			if (pItem->IsEditMode())
			{
				if (!strText.IsEmpty())
				{
					bHandle = true;
					pItem->EndEditMode();
					CEditContainerItemUI* pLast = GetEditContainerItemAt(nCount - 1);
					if (pLast)
					{
						pLast->SetEditFocus();
					}
					Invalidate();
				}
			}
			else
			{
				if (!strText.IsEmpty() && nCount > 0)
				{
					bHandle = true;
					AddEditControlItem(strText);
					pEdit->SetText(_T(""));
					CEditContainerItemUI* pLast = GetEditContainerItemAt(GetCount() - 1);
					if (pLast)
					{
						pLast->SetEditFocus();
					}
				}
			}
		}

		return bHandle;
	}

	bool CEditContainerUI::OnEditBackKeydown(int index, CEditContainerItemUI* pItem, CEditUI* pEdit)
	{
		bool bHandle = false;
		if (m_pPreEditMessageHandler)
		{
			bHandle = m_pPreEditMessageHandler->OnEditBackKeydown(this, index, pItem, pEdit);
		}

		if (!bHandle)
		{
			CDuiString strText = pEdit->GetText();
			int nCount = GetCount();

			
			if (index >= 0 && strText.IsEmpty())
			{
				bHandle = true;
				if (nCount == 1)
				{
					pItem->ClearControl();
				}
				else
				{
					CEditContainerItemUI* pFind = NULL;
					GetPreEditContainerItem(index, &pFind);
					if (pFind == NULL)
					{
						GetNextEditContainerItem(index, &pFind);
					}
					if (pFind == NULL)
					{
						GetPreEditContainerItem(index, &pFind, false);
					}

					if (pFind)
					{
						pFind->SetVisible(true);
						pFind->SetEditFocus();
						if (m_pManager)
						{
							m_pManager->SendNotify(this, _T("editContainerDeleteItem"), (WPARAM)pItem, 0, true);
						}
						
					}
				}

			}
		}

		return bHandle;
	}

	bool CEditContainerUI::OnEditLeftKeydown(int index, CEditContainerItemUI* pItem, CEditUI* pEdit)
	{
		bool bHandle = false;
		if (m_pPreEditMessageHandler)
		{
			bHandle = m_pPreEditMessageHandler->OnEditLeftKeydown(this, index, pItem, pEdit);
		}

		if (!bHandle && !pItem->IsEditMode())
		{
			CDuiString strText = pEdit->GetText();
			int nCount = GetCount();

			if (nCount > 0)
			{
				if (strText.IsEmpty())
				{
					bHandle = true;
				}
				else
				{
					long lStart = 0;
					long lEnd = 0;
					pEdit->GetSel(lStart, lEnd);

					if (lStart == 0)
					{
						bHandle = true;
					}
				}

				if (bHandle && index > 0)
				{
					CEditContainerItemUI* pFind = NULL;
					GetPreEditContainerItem(index, &pFind);
					if (pFind)
					{
						pFind->SetEditFocus();
					}
				}
			}

		}

		return bHandle;
	}

	bool CEditContainerUI::OnEditRightKeydown(int index, CEditContainerItemUI* pItem, CEditUI* pEdit)
	{
		bool bHandle = false;
		if (m_pPreEditMessageHandler)
		{
			bHandle = m_pPreEditMessageHandler->OnEditRightKeydown(this, index, pItem, pEdit);
		}

		if (!bHandle && !pItem->IsEditMode())
		{
			CDuiString strText = pEdit->GetText();
			int nCount = GetCount();
		
			if (strText.IsEmpty() && nCount > 1)
			{
				bHandle = true;
				if (index < nCount - 1)
				{
					CEditContainerItemUI* pFind = NULL;
					GetNextEditContainerItem(index, &pFind);
					if (pFind)
					{
						pFind->SetEditFocus();
						CEditUI* pEt = pFind->GetEdit();
						pEt->SetSel(0, 0);
					}
				}
			}
		}

		return bHandle;
	}

	bool CEditContainerUI::OnEditUpKeydown(int index, CEditContainerItemUI* pItem, CEditUI* pEdit)
	{
		bool bHandle = false;
		if (m_pPreEditMessageHandler)
		{
			bHandle = m_pPreEditMessageHandler->OnEditUpKeydown(this, index, pItem, pEdit);
		}

		return bHandle;
	}

	bool CEditContainerUI::OnEditDownKeydown(int index, CEditContainerItemUI* pItem, CEditUI* pEdit)
	{
		bool bHandle = false;
		if (m_pPreEditMessageHandler)
		{
			bHandle = m_pPreEditMessageHandler->OnEditDownKeydown(this, index, pItem, pEdit);
		}

		return bHandle;
	}

	CEditContainerItemUI* CEditContainerUI::GetEditContainerItemAt(int index)
	{
		CControlUI* pControl = GetItemAt(index);
		if (pControl)
		{
			return static_cast<CEditContainerItemUI*>(pControl->GetInterface(_T("EditContainerItem")));
		}

		return NULL;
	}

	bool CEditContainerUI::AddEditContainerItem(CEditContainerItemUI* pControl)
	{
		SetEditContainerModify(true);
		pControl->SetEditContainer(this);
		return Add(pControl);
	}

	bool CEditContainerUI::AddEditContainerItemAt(CEditContainerItemUI* pControl, int iIndex)
	{
		SetEditContainerModify(true);
		pControl->SetEditContainer(this);
		return AddAt(pControl, iIndex);
	}

	bool CEditContainerUI::RemoveEditContainerItem(CEditContainerItemUI* pControl)
	{
		SetEditContainerModify(true);
		pControl->SetEditContainer(NULL);
		return Remove(pControl);
	}

	bool CEditContainerUI::RemoveEditContainerItemAt(int iIndex)
	{
		SetEditContainerModify(true);
		CEditContainerItemUI* pFind = GetEditContainerItemAt(iIndex);
		if (pFind)
		{
			pFind->SetEditContainer(NULL);
		}
		return RemoveAt(iIndex);
	}

	void CEditContainerUI::RemoveAllEditContainerItem(bool bDeleteDefault)
	{
		int nCount = GetCount();
		for (int i = nCount-1; i > 0; i--)
		{
			RemoveEditContainerItemAt(i);
		}

		if (GetCount() == 1)
		{
			if (bDeleteDefault)
			{
				RemoveEditContainerItemAt(0);
			}
			else
			{
				CEditContainerItemUI* pItem = GetEditContainerItemAt(0);
				if (pItem && !pItem->IsOnlyEdit())
				{
					pItem->ClearControl();
					SetEditContainerModify(true);
				}
			}
		}
	}

	bool CEditContainerUI::IsEditContainerEmpty()
	{
		int nCount = GetCount();
		if (nCount == 0)
		{
			return true;
		}

		if (nCount > 1)
		{
			return false;
		}

		CEditContainerItemUI* pItem = GetEditContainerItemAt(0);
		if (pItem && pItem->IsOnlyEdit())
		{
			return true;
		}

		return false;
	}

	CEditUI* CEditContainerUI::GetCurrentEdit()
	{
		CEditUI* pEdit = NULL;
		if (m_pCurrentEditItem)
		{
			int nIndex = GetEditContainerItemIndex(m_pCurrentEditItem);
			if (nIndex >= 0)
			{
				pEdit = m_pCurrentEditItem->GetEdit();
			}
		}

		if (pEdit == NULL && GetCount() > 0)
		{
			CEditContainerItemUI* pItem = GetEditContainerItemAt(GetCount() - 1);
			if (pItem)
			{
				pEdit = pItem->GetEdit();
			}
		}

		return pEdit;
	}

	void CEditContainerUI::SetLastItemFocus()
	{
		if (GetCount() > 0)
		{
			CEditContainerItemUI* pItem = GetEditContainerItemAt(GetCount() - 1);
			if (pItem)
			{
				pItem->SetEditFocus();
			}
		}
	}

	void CEditContainerUI::OnEditContainerChanged()
	{
		if (m_pManager)
		{
			m_pManager->SendNotify(this, _T("editContainerChanged"), 0, 0, true);
		}
	}

	int CEditContainerUI::GetEditContainerItemIndex(CControlUI* pItem)
	{
		int nIndex = GetItemIndex(pItem);
		if (nIndex >= 0)
		{
			CEditContainerItemUI* pControl = static_cast<CEditContainerItemUI*>(pItem->GetInterface(DUI_CTR_EDIT_CONTAINER_ITEM));
			if (pControl)
			{
				return nIndex;
			}
		}
		

		return -1;
	}

	void CEditContainerUI::SetMinLastEditWidth(int width)
	{
		m_nMinLastEditWidth = width;
	}

	void CEditContainerUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if (_tcsicmp(pstrName, _T("ec_mineditwidth")) == 0)
		{
			SetMinLastEditWidth(_ttoi(pstrValue));
		}
		else if (_tcsicmp(pstrName, _T("lec_editstylename")) == 0)
		{
			m_strEditStyleName = pstrValue;
		}
		else if (_tcsicmp(pstrName, _T("lec_itemstylename")) == 0)
		{
			m_strItemStyleName = pstrValue;
		}
		else if (_tcsicmp(pstrName, _T("lec_labeltylename")) == 0)
		{
			m_strLabelStyleName = pstrValue;
		}
		else
		{
			CHorizontalLayoutUI::SetAttribute(pstrName, pstrValue);
		}
	}

	int CEditContainerUI::GetPreEditContainerItem(int nIndex, CEditContainerItemUI** pFind, bool bCheckVisible /* = true */)
	{
		int nCount = GetCount();
		if (nIndex < 1 || nIndex > nCount)
		{
			return -1;
		}

		nIndex--;
		while (nIndex >= 0)
		{
			CEditContainerItemUI* pItem = GetEditContainerItemAt(nIndex);
			if (pItem && (!bCheckVisible || pItem->IsVisible()))
			{
				if (pFind)
				{
					*pFind = pItem;
				}
				break;
			}

			nIndex--;
		}

		return nIndex;
	}

	int CEditContainerUI::GetNextEditContainerItem(int nIndex, CEditContainerItemUI** pFind, bool bCheckVisible /* = true */)
	{
		int nCount = GetCount();
		if (nIndex < 0 || nIndex >= nCount)
		{
			return -1;
		}

		nIndex++;
		while (nIndex < nCount)
		{
			CEditContainerItemUI* pItem = GetEditContainerItemAt(nIndex);
			if (pItem && (!bCheckVisible || pItem->IsVisible()))
			{
				if (pFind)
				{
					*pFind = pItem;
				}
				break;
			}

			nIndex++;
		}

		return nIndex;
	}

	int CEditContainerUI::FindEditContainerItemByText(LPCTSTR lpszText, CEditContainerItemUI** pFind /* = NULL */, int nSkipIndex /* = -1 */, CEditContainerItemUI* pSkipItem /* = NULL */)
	{
		int nCount = GetCount();
		for (int i = 0; i < nCount; i++)
		{
			if (nSkipIndex != -1 && nSkipIndex == i)
			{
				continue;
			}

			CEditContainerItemUI* pItem = GetEditContainerItemAt(i);
			if (pItem == NULL || pSkipItem == pItem)
			{
				continue;
			}

			CDuiString str = pItem->GetItemText();
			if (str == lpszText)
			{
				if (pFind)
				{
					*pFind = pItem;
				}
				return i;
			}
		}

		return -1;
	}

	int CEditContainerUI::FindEidtContainerItemByChildControl(CControlUI* pChild, CEditContainerItemUI** pFind /* = NULL */)
	{
		if (pChild == NULL)
		{
			return -1;
		}
		int nCount = GetCount();
		for (int i = 0; i < nCount; i++)
		{
			CEditContainerItemUI* pItem = GetEditContainerItemAt(i);
			if (pItem == NULL)
			{
				continue;
			}

			if (pChild == pItem || pItem->GetLabel() == pChild || pItem->GetEdit() == pChild)
			{
				if (pFind)
				{
					*pFind = pItem;
				}
				return i;
			}
		}

		return -1;
	}

	void CEditContainerUI::SetPos(RECT rc, bool bNeedInvalidate)
	{
		CControlUI::SetPos(rc, bNeedInvalidate);
		rc = m_rcItem;

		// Adjust for inset
		RECT m_rcInset = CHorizontalLayoutUI::m_rcInset;
		GetManager()->GetDPIObj()->Scale(&m_rcInset);
		rc.left += m_rcInset.left;
		rc.top += m_rcInset.top;
		rc.right -= m_rcInset.right;
		rc.bottom -= m_rcInset.bottom;

		int nCount = GetCount();
		if (nCount == 0) {
			return;
		}
		SIZE szAvailable = { rc.right - rc.left, rc.bottom - rc.top };
		// last once
		CEditContainerItemUI* pLast = GetEditContainerItemAt(nCount - 1);
		pLast->EnableEdit(true);
		pLast->GetEdit()->SetFixedWidth(0);
		SIZE szLast = pLast->EstimateSize(szAvailable);
		int minLastMinWidth = szLast.cx + m_nMinLastEditWidth;

		int nChildPadding = GetChildPadding();
		int i = 0;
		// check is all visible
		int nWidth = szAvailable.cx - minLastMinWidth;
		int nLastUnVisible = -1;
		if (nWidth < 0)
		{
			nLastUnVisible = nCount - 1;
		}
		else
		{
			for (i = nCount - 2; i >= 0; i--)
			{
				CEditContainerItemUI* pItem = GetEditContainerItemAt(i);
				pItem->AutoEditWidth();
				SIZE sz = pItem->EstimateSize(szAvailable);
				RECT rcItem = pItem->GetPadding();
				sz.cx += rcItem.left + rcItem.right;
				if (i != 0)
				{
					sz.cx += nChildPadding;
				}
				nWidth -= sz.cx;
				if (nWidth < 0)
				{
					nLastUnVisible = i;
					break;
				}
			}
		}

		if (nLastUnVisible == nCount - 1) // last is un visible
		{
			nLastUnVisible = nCount - 2;
		}

		// set visible
		for (i = 0; i < nLastUnVisible + 1; i++)
		{
			GetItemAt(i)->SetVisible(false);
		}


		int iPosX = rc.left;
		SIZE szRemaining = szAvailable;
		SIZE szControlAvailable = { 0 };
		for (i = nLastUnVisible + 1; i < nCount; i++)
		{
			CEditContainerItemUI* pItem = GetEditContainerItemAt(i);
			pItem->SetVisible(true);

			RECT rcPadding = pItem->GetPadding();
			szRemaining.cx -= rcPadding.left;

			szControlAvailable = szRemaining;
			szControlAvailable.cy -= rcPadding.top + rcPadding.bottom;
			SIZE sz = pItem->EstimateSize(szControlAvailable);
			if (i == nCount-1)
			{
				sz.cx = szControlAvailable.cx - rcPadding.right;
			}
			//sz.cy = pItem->GetFixedHeight();
			if (sz.cy == 0) sz.cy = szAvailable.cy - rcPadding.top - rcPadding.bottom;
			if (sz.cy < 0) sz.cy = 0;
			if (sz.cy > szControlAvailable.cy) sz.cy = szControlAvailable.cy;
			

			UINT iChildAlign = GetChildVAlign();
			if (iChildAlign == DT_VCENTER) {
				int iPosY = (rc.bottom + rc.top) / 2;
				RECT rcCtrl = { iPosX + rcPadding.left, iPosY - sz.cy / 2, iPosX + sz.cx + rcPadding.left, iPosY + sz.cy - sz.cy / 2 };
				pItem->SetPos(rcCtrl, false);
			}
			else if (iChildAlign == DT_BOTTOM) {
				int iPosY = rc.bottom;
				RECT rcCtrl = { iPosX + rcPadding.left, iPosY - rcPadding.bottom - sz.cy, iPosX + sz.cx + rcPadding.left, iPosY - rcPadding.bottom };
				pItem->SetPos(rcCtrl, false);
			}
			else {
				int iPosY = rc.top;
				RECT rcCtrl = { iPosX + rcPadding.left, iPosY + rcPadding.top, iPosX + sz.cx + rcPadding.left, iPosY + sz.cy + rcPadding.top };
				pItem->SetPos(rcCtrl, false);
			}

			iPosX += sz.cx + nChildPadding  + rcPadding.left + rcPadding.right;
			szRemaining.cx -= sz.cx + nChildPadding + rcPadding.right;

		}
	}


	void CEditContainerUI::ApplyAllItemStyle()
	{
		int nCount = GetCount();
		for (int i = 0; i < nCount;i++)
		{
			CEditContainerItemUI* pItem = GetEditContainerItemAt(i);
			if (pItem)
			{
				ApplyItemStyle(pItem);
			}
		}
	}

	void CEditContainerUI::ApplyItemStyle(CEditContainerItemUI* pItem)
	{
		SetItemEditStyle(pItem, _T(""));
		SetItemStyle(pItem, _T(""));
		SetItemLabelStyle(pItem, _T(""));
	}

	void CEditContainerUI::SetItemLabelStyle(CEditContainerItemUI* pItem, LPCTSTR lpszStyle)
	{
		CDuiString strStyle;
		if (lpszStyle && lpszStyle[0] != 0)
		{
			strStyle = lpszStyle;
		}
		else if (m_pManager && !m_strLabelStyleName.IsEmpty())
		{
			strStyle = m_pManager->GetStyle(m_strLabelStyleName);
		}

		if (!strStyle.IsEmpty())
		{
			pItem->SetControlStyle(strStyle);
		}
	}

	void CEditContainerUI::SetItemStyle(CEditContainerItemUI* pItem, LPCTSTR lpszStyle)
	{
		CDuiString strStyle;
		if (lpszStyle && lpszStyle[0] != 0)
		{
			strStyle = lpszStyle;
		}
		else if (m_pManager && !m_strItemStyleName.IsEmpty())
		{
			strStyle = m_pManager->GetStyle(m_strItemStyleName);
		}

		if (!strStyle.IsEmpty())
		{
			pItem->ApplyAttributeList(strStyle);
		}
	}

	void CEditContainerUI::SetItemEditStyle(CEditContainerItemUI* pItem, LPCTSTR lpszStyle)
	{
		CDuiString strStyle;
		if (lpszStyle && lpszStyle[0] != 0)
		{
			strStyle = lpszStyle;
		}
		else if (m_pManager && !m_strEditStyleName.IsEmpty())
		{
			strStyle = m_pManager->GetStyle(m_strEditStyleName);
		}

		if (!strStyle.IsEmpty())
		{
			pItem->SetEditStyle(strStyle);
		}
	}

	bool CEditContainerUI::AddEditControlItem(LPCTSTR lpszText)
	{
		int nCount = GetCount();
		CEditContainerItemUI* pItem = NULL;
		if (nCount > 0)
		{
			CEditContainerItemUI* pLast = GetEditContainerItemAt(nCount - 1);
			if (pLast && pLast->IsOnlyEdit())
			{
				CEditUI* pEdit = pLast->GetEdit();
				if (pEdit)
				{
					pEdit->SetText(_T(""));
				}
				pItem = pLast;
			}
		}

		if ((pItem || nCount != 0) && (lpszText == NULL || lpszText[0] == 0))
		{
			return false;
		}

		if (pItem == NULL)
		{
			pItem = new CEditContainerItemUI;
			pItem->SetChildVAlign(DT_VCENTER);
			SetItemEditStyle(pItem, _T(""));
			SetItemStyle(pItem, _T(""));
			AddEditContainerItem(pItem);
		}

		
		if (lpszText && lpszText[0] != 0)
		{
			CLabelUI* pLabel = CreateItemControl();
			if (pLabel)
			{
				pLabel->SetText(lpszText);
				pItem->SetControl(pLabel);
				SetItemLabelStyle(pItem, _T(""));
			}
		}

		nCount = GetCount();
		if (nCount > 1)
		{
			CEditContainerItemUI* pLast = GetEditContainerItemAt(nCount - 2);
			if (pLast)
			{
				pLast->SetLabelMode();
			}
		}

		SetEditContainerModify(true);
		return true;
	}

	CLabelUI* CEditContainerUI::CreateItemControl()
	{
		return m_pItemControlCreater ? m_pItemControlCreater->CreateItemControl() : new CEditContainerItemLabelUI;
	}

	void CEditContainerUI::OnAfterEditContainerItemKillFocus(CEditContainerItemUI* pItem, bool bEditMode, bool bCheckAdd)
	{
		if (pItem == NULL )
		{
			return;
		}

		int nIndex = GetEditContainerItemIndex(pItem);
		if (nIndex < 0)
		{
			return;
		}

		CDuiString strText = pItem->GetItemText();
		if (!strText.IsEmpty())
		{
			
			return;
		}

		if (!pItem->IsOnlyEdit())
		{
			if (GetCount() == 1)
			{
				pItem->ClearControl();
			}
			else
			{
				RemoveEditContainerItemAt(nIndex);
			}
		}
		else 
		{
			if (bCheckAdd && nIndex == GetCount() - 1)
			{
				CEditUI* pEdt = pItem->GetEdit();
				if (pEdt)
				{
					CDuiString str = pEdt->GetText();
					if (!str.IsEmpty())
					{
						pEdt->SetText(_T(""));
						AddEditControlItem(str);
					}
				}
			}
		}
		
	}

	void CEditContainerUI::EnableItemTextRepeat(bool bEnable)
	{
		m_bCanItemTextRepeat = bEnable;
	}

	bool CEditContainerUI::IsEnableItemTextRepeat()
	{
		return m_bCanItemTextRepeat;
	}

	void CEditContainerUI::SetEditContainerModify(bool bModify)
	{
		m_bContainerModify = bModify;
	}

	bool CEditContainerUI::IsEditContainerModify()
	{
		return m_bContainerModify;
	}

}
