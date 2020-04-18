#include "stdafx.h"
#include "stockInterface.h"
#include "synCode.h"

namespace stock_wrapper
{
	// code interface
	static inline SynCode* _SynCodePtr(HSynCode hSynCode)
	{
		return (SynCode*)hSynCode;
	}

	HSynCode NewSynCode(uint32 id/* =0 */)
	{
		SynCode* pCode = new SynCode;
		pCode->SetDataID(id);
		return pCode;
	}

	void ReleaseSynCode(HSynCode hSynCode)
	{
		delete _SynCodePtr(hSynCode);
	}

	void SetCodeDataID(HSynCode hSynCode, uint32 id)
	{
		_SynCodePtr(hSynCode)->SetDataID(id);
	}

	uint32 GetCodeDataID(HSynCode hSynCode)
	{
		return _SynCodePtr(hSynCode)->GetDataID();
	}

	int GetCodeSize(HSynCode hSynCode)
	{
		return _SynCodePtr(hSynCode)->GetCodeSize();
	}

	StockCode& GetCodeAt(HSynCode hSynCode, int index)
	{
		return _SynCodePtr(hSynCode)->GetCodeAt(index);
	}

	bool GetCodeAt(HSynCode hSynCode, int index, StockCode& code)
	{
		code.Clear();
		SynCode* pCode = _SynCodePtr(hSynCode);
		int s = pCode->GetCodeSize();
		if (index >= 0 && index < s)
		{
			code = pCode->GetCodeAt(index);
		}

		return !code.Empty();
	}

	StockArray& GetCodeArray(HSynCode hSynCode)
	{
		return _SynCodePtr(hSynCode)->GetCodeArray();
	}

	void GetCodeArray(HSynCode hSyncode, StockArray& ayCode)
	{
		_SynCodePtr(hSyncode)->GetCodeArray(ayCode);
	}

	int GetCodeScrollIndex(HSynCode hSynCode)
	{
		return _SynCodePtr(hSynCode)->GetScrollIndex();
	}

	int GetCodeCurIndex(HSynCode hSynCode)
	{
		return _SynCodePtr(hSynCode)->GetCurIndex();
	}

	int GetCodePage(HSynCode hSynCode)
	{
		return _SynCodePtr(hSynCode)->GetCodePage();
	}

	void SetCodePage(HSynCode hSynCode, int nPage)
	{
		_SynCodePtr(hSynCode)->SetCodePage(nPage);
	}

	void GetCodeScrollInfo(HSynCode hSynCode, int* nScroll, int* nPage, int* nTotal)
	{
		_SynCodePtr(hSynCode)->GetScrollInfo(nScroll, nPage, nTotal);
	}

	void SetCodeList(HSynCode hSynCode, StockArray& ayCode, uint64 uMode, int nSeltectedIndex, StockCode* pSelectedCode)
	{
		_SynCodePtr(hSynCode)->SetCodeList(ayCode, uMode, nSeltectedIndex, pSelectedCode);
	}

	void RegisterStockChangeEventHandler(HSynCode hSynCode, IStockChangeEventHandler* pHandler)
	{
		_SynCodePtr(hSynCode)->RegisterStockChangeEventHandler(pHandler);
	}

	void UnRegisterStockChangeEventHandler(HSynCode hSynCode, IStockChangeEventHandler* pHandler)
	{
		_SynCodePtr(hSynCode)->UnRegisterStockChangeEventHandler(pHandler);
	}
}