#include "stdafx.h"
#include "synCode.h"





SynCode::SynCode()
{

}

SynCode::~SynCode()
{

}

uint32 SynCode::GetDataID()
{
	return m_nID;
}

void SynCode::SetDataID(uint32 id)
{
	m_nID = id;
}

int SynCode::GetCodeSize()
{
	return m_ayCode.GetStockCodeSize();
}

stock_wrapper::StockCode& SynCode::GetCodeAt(int index)
{
	return m_ayCode.GetStockCodeAt(index);
}

stock_wrapper::StockArray& SynCode::GetCodeArray()
{
	return m_ayCode;
}

void SynCode::GetCodeArray(stock_wrapper::StockArray& ayCode)
{
	ayCode = m_ayCode;
}

int SynCode::FindCodeIndex(const stock_wrapper::StockCode& stockCode)
{
	if (!stockCode.Empty())
	{
		return m_ayCode.FindStockCodeIndex(stockCode);
	}

	return -1;
}

int SynCode::GetScrollIndex()
{
	return m_nScrollPos;
}

int SynCode::GetCurIndex()
{
	return m_nCurpos;
}

int SynCode::GetCodePage()
{
	return m_nPage;
}

void SynCode::SetCodePage(int nPage)
{
	if (nPage <= 0)
	{
		nPage = 1;
	}
	m_nPage = nPage;
}

void SynCode::GetScrollInfo(int* nScroll, int* nPage, int* nTotal)
{
	if (nScroll)
	{
		*nScroll = m_nScrollPos;
	}

	if (nPage)
	{
		*nPage = m_nPage;
	}

	if (nTotal)
	{
		*nTotal = GetCodeSize();
	}
}

void SynCode::OnStockChange(uint64 uEvent)
{
	for (void* p : m_vcEventHandler)
	{
		IStockChangeEventHandler* pHandler = (IStockChangeEventHandler*)p;
		if (pHandler)
		{
			pHandler->OnStockChangeEvent(this, m_nID, uEvent);
		}
	}
}

int SynCode::_CalcLastPageFirstIndex()
{
	return GetCodeSize() / m_nPage * m_nPage;
}

int SynCode::_CalcScrollPos(int nPos, uint32 uPosOption, bool bCycle)
{
	int nSize = GetCodeSize();
	if (nSize == 0)
	{
		return 0;
	}

	int nCurScroll = 0;
	if (uPosOption == STOCK_CODELIST_SCROLL_POS_PAGE)
	{
		nCurScroll +=  nPos * m_nPage;
	}
	else if (uPosOption == STOCK_CODELIST_SCROLL_POS_ABS)
	{
		nCurScroll = nPos;
	}
	else
	{
		nCurScroll += nPos;
	}

	int nLastValidPos = _CalcLastPageFirstIndex();

	if (nCurScroll < 0)
	{
		if (bCycle)
		{
			int nTotal = nLastValidPos + m_nPage;
			nCurScroll = nCurScroll % nTotal + nTotal;
			if (nCurScroll > nLastValidPos)
			{
				nCurScroll = nLastValidPos;
			}
		}
		else
		{
			nCurScroll = 0;
		}
	}
	else if (nCurScroll > nLastValidPos)
	{
		if (bCycle)
		{
			nCurScroll = nCurScroll % (nLastValidPos+m_nPage);
			nCurScroll = nCurScroll / m_nPage * m_nPage;
			if (nCurScroll > nLastValidPos)
			{
				nCurScroll = nLastValidPos;
			}
		}
		else
		{
			nCurScroll = nLastValidPos;
		}
	}

	return nCurScroll;
}

bool SynCode::_ScrollCode(uint32 uMode, int nPos)
{
	int nScrollPos = -1;
	if (uMode & STOCK_CODELIST_SCROLL_LAST_PAGE) // ¹ö¶¯µ½Ò»Ò³
	{
		nScrollPos = _CalcLastPageFirstIndex();
	}
	else
	{
		nScrollPos = _CalcScrollPos(nPos, GET_STOCK_CODELIST_SCROLL_POS(uMode), IS_STOCK_CODELIST_SCROLL_CYCLE(uMode));
	}

	if (nScrollPos < 0 || nScrollPos == m_nScrollPos)
	{
		return false;
	}

	m_nScrollPos = nScrollPos;
	
	return true;
}

bool SynCode::_ScrollByCode(const stock_wrapper::StockCode& stockCode)
{
	int nPos = FindCodeIndex(stockCode);

	if (nPos < 0 || nPos == m_nScrollPos)
	{
		return false;
	}

	m_nScrollPos = nPos;
	return true;
}

void SynCode::ScrollCode(uint32 uMode, int nPos, stock_wrapper::StockCode* stockCode )
{
	bool bScroll = false;
	if ((uMode & STOCK_CODELIST_SCROLL_BYCODE))
	{
		if (stockCode)
		{
			bScroll = _ScrollByCode(*stockCode);
		}
	}
	else
	{
		bScroll = _ScrollCode(uMode, nPos);
	}

	if (bScroll)
	{
		OnStockChange(STOCK_CHANGE_EVENT_SCROLL);
	}
}

void SynCode::_FixScrollAndSelected()
{
	int size = GetCodeSize();
	int nValid = _CalcLastPageFirstIndex();
	if (m_nScrollPos > nValid)
	{
		m_nScrollPos = 0;
	}

	if (m_nCurpos >= size)
	{
		m_nCurpos = 0;
	}
}

void SynCode::SetCodeList(stock_wrapper::StockArray& ayCode, uint64 uMode, int nSeltectedIndex, stock_wrapper::StockCode* pSelectedCode)
{
	Empty();
	m_ayCode = ayCode;
	_FixScrollAndSelected();
	OnStockChange(STOCK_CHANGE_EVENT_RESET);
}

void SynCode::Empty()
{
	m_ayCode.ClearStockCode();
}

void SynCode::RegisterStockChangeEventHandler(IStockChangeEventHandler* pHandler)
{
	CPtrVector::iterator it = std::find(m_vcEventHandler.begin(), m_vcEventHandler.end(), (void*)pHandler);
	if (it == m_vcEventHandler.end())
	{
		m_vcEventHandler.push_back(pHandler);
	}
}

void SynCode::UnRegisterStockChangeEventHandler(IStockChangeEventHandler* pHandler)
{
	CPtrVector::iterator it = std::find(m_vcEventHandler.begin(), m_vcEventHandler.end(), (void*)pHandler);
	if (it == m_vcEventHandler.end())
	{
		m_vcEventHandler.erase(it);
	}
}