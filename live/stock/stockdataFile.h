#pragma once

namespace stock_wrapper
{

	class TradeDate : public xlf::FmtObjArray<long>
	{
	public:
		void SetFilePath(LPCTSTR lpszFilePath);
		void Read();
		void Write();

		void SortDate();
		void UpdateDate(const long* pDate, int len);

	protected:
		_tstring m_strFilePath;
	};
}