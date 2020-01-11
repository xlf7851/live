#pragma once
namespace xlf
{
	class CBuffer;
	/**
	* @brief ini �ļ�����
	* ������ļ���ȡ����Ҫ֪���ļ������ʽ��֧��utf8��gbk��Ĭ����utf8
	* ʵ�ַ���
	* 1��ʹ�û���������ķ�ʽ
	* 2��section��������¼�ε���ʼ�ֽ�ƫ�ƣ�k=@secton, sectionʹ��˳�������洢
	* 3��k-v��������¼���section���ֽ�ƫ�� k=@section@key
	*/
	class CIniFile
	{
	public:
		enum ini_file_type
		{
			ini_file_utf8 = 1,
			ini_file_gbk  = 2,
		};
	public:
		typedef std::map<std::string, std::string> _key_value_map_t;
		typedef std::map<std::string, _key_value_map_t*> _sec_dict_t;
	public:
		CIniFile(LPCTSTR filePath = NULL, int codeType = ini_file_utf8);
		virtual ~CIniFile();

		void ParseFromFile(const std::string& filePath, int codeType = ini_file_utf8);
#ifdef UNICODE
		void ParseFromMem(const wchar_t* buf, int nLen);
#else
		void ParseFromMem(const char* buf, int nLen);
#endif
		void Clear();
		void SetData(LPCTSTR sec, LPCTSTR key, LPCTSTR value);
		bool GetData(LPCTSTR sec, LPCTSTR key, std::string& value);
		void WriteFile(LPCTSTR filePath = NULL);

		bool IsModify();

	protected:
		void Parse(const TCHAR* buf, int len);

		void _ParseLine(int nType, const TCHAR* buf, int len, std::string& strLastSec);
		void _AddSection(const std::string& sec);
		void _AddKV(const std::string& sec, const std::string& key, const std::string& value);
		void _ParseKV(const TCHAR* buf, int len, const std::string& sec);
	
		void _ClearSection(const std::string& sec);
		void _ClearKey(const std::string& sec, const std::string& key);
		
		_key_value_map_t* _Find(const std::string& sec, bool bCreate = false);

		void _BuildToBuffer(CBuffer& buf);
	protected:
		_sec_dict_t m_data;
		std::string m_filePath;
		int		m_fileType;
		bool	m_bModify;
	};
}
