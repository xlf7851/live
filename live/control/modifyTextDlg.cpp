// MainWnd.cpp

#include "StdAfx.h"
#include "modifyTextDlg.h"


CModifyTextDlg::CModifyTextDlg()
{
	m_pErrorChecker = nullptr;
	m_pEdit = nullptr;
}

void CModifyTextDlg::InitInfo(LPCTSTR lpOldText, LPCTSTR lpszTitle /* = nullptr */, IModifyTextDlgCheckTextHandler* pErrorChecker /* = nullptr */)
{
	if (lpOldText && lpOldText[0] != 0)
	{
		m_strOldText = lpOldText;
	}

	if (lpszTitle && lpszTitle[0] != 0)
	{
		m_strTitle = lpszTitle;
	}

	m_pErrorChecker = pErrorChecker;
}

CDuiString CModifyTextDlg::GetSkinFile()
{
	return _T("xml_modify_text_dlg");
}

LPCTSTR CModifyTextDlg::GetWindowClassName() const
{
	return _T("ModifyTextDlg");
}


void CModifyTextDlg::InitWindow()
{
	m_pEdit = static_cast<CEditUI*>(m_pm.FindControl(_T("newText")));
	if (m_pEdit && !m_strOldText.IsEmpty())
	{
		m_pEdit->SetText(m_strOldText);
	}
	if (!m_strTitle.IsEmpty())
	{
		CLabelUI* pLabel = static_cast<CLabelUI*>(m_pm.FindControl(_T("labelTitle")));
		if (pLabel)
		{
			pLabel->SetText(m_strTitle);
		}
	}

}

void CModifyTextDlg::OnClick(TNotifyUI& msg)
{
	CDuiString name = msg.pSender->GetName();
	if (name.CompareNoCase(_T("btnConfirm")) == 0)
	{
		if (CheckError() == 0)
		{
			Close(IDOK);
		}
		else if (m_pEdit)
		{
			m_pEdit->SetFocus();
		}
	}
	else if (name.CompareNoCase(_T("btnCancel")) == 0)
	{
		Close(IDCANCEL);
	}
	else
	{
		WindowImplBase::OnClick(msg);
	}
}

int CModifyTextDlg::CheckError()
{
	if (m_pEdit == nullptr)
	{
		return 0;
	}
	CDuiString strNewText = m_pEdit->GetText();
	if (m_strOldText == strNewText) // 没有修改
	{
		m_strNewText = strNewText;
		return 0;
	}

	
	if (m_pErrorChecker)
	{
		int nRet = m_pErrorChecker->CheckModifyNewText(strNewText, m_strErrorMsg);
		if (nRet == 0)
		{
			m_strNewText = strNewText;
		}

		return nRet;
	}

	if (strNewText.IsEmpty())
	{
		m_strErrorMsg = _T("文本为空");
		return 1;
	}

	m_strNewText = strNewText;
	return 0;

}

CDuiString CModifyTextDlg::GetNewText()
{
	return m_strNewText;
}

int CModifyTextDlg::ShowDlg(CDuiString& strNewText, LPCTSTR lpOldText, HWND hParent, LPCTSTR lpszTitle, int xPos, int yPox, IModifyTextDlgCheckTextHandler* pErrorChecker)
{
	int nRet = IDCANCEL;
	CModifyTextDlg dlg;
	dlg.InitInfo(lpOldText, lpszTitle, pErrorChecker);
	if (dlg.CreateDuiWindow(hParent, _T("ModifyTextDlg"), UI_WNDSTYLE_DIALOG, 0))
	{
		if (xPos == -1 || yPox == -1)
		{
			dlg.CenterWindow();
		}
		else
		{
			::SetWindowPos(dlg.GetHWND(), NULL, xPos, yPox, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);
		}
		
		nRet = dlg.ShowModal();
		if (nRet == IDOK)
		{
			strNewText = dlg.GetNewText();
		}
	}

	return nRet;
}