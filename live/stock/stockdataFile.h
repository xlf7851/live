#pragma once
#include "stockdata.h"

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

#pragma pack(1)
	struct StockFileHeader 
	{
		int32 m_size;			// �ṹ��С
		uint32 m_flag;			// �ļ���ǣ�"XLFD"
		uint32 m_uDataType;		// ��������
		uint32 m_market;		// �г�
		char   m_code[8];		// ����
		uint32 m_uParam;			// ����һ������
		int32 m_totalSize;		// �ܵĴ�С���������ݺ��ļ�ͷ����
	};
#pragma pack()

	

	class BaseStockFileParse
	{
	public:
		BaseStockFileParse();

		bool ParseFromFile(LPCTSTR lpszFilePath);

	public:
		StockCode m_stock;
		uint32 m_dataType;
		uint32 m_uParam;
		xlf::CBuffer m_dataBuf;

	};

	class CLoadThsStockFile
	{
	public:
		static void Load(LPCTSTR lpDir);

	};
}