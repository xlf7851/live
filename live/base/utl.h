#pragma once

struct _xlf_common_binary_file_header_t;
namespace xlf
{
	class CBuffer;
	

	// utf8 和unicode
	int Utf8ToUnicode(const char* buf, int len,wchar_t* outbuf, int outbuflen);
	int UnicodeToUtf8(const wchar_t* buf, int len, char* out, int outbuflen);
	int Utf8ToUnicodeBufLength(const char* buf, int len);
	int UnicodeToUtf8BufLength(const wchar_t* buf, int len);
	int Utf8ToWstring(const char* buf, int len, wstring& out);
	int WstringToUtf8(const wchar_t* buf, int len, CBuffer& out);


	// ansi 和 unicode
	int AnsiToUnicode(const char* buf, int len, wchar_t* outbuf, int outbuflen);
	int UnicodeToAnsi(const wchar_t* buf, int len, char* out, int outbuflen);
	int AnsiToUnicodeBufLength(const char* buf, int len);
	int UnicodeToAnsiBufLength(const wchar_t* buf, int len);
	int AnsiToWstring(const char* buf, int len, wstring&out);
	int WstringToAnsi(const wchar_t* buf, int nLen, CBuffer& out);
	std::wstring AutoToWString(LPCTSTR lpszText);

	// utf8 and ansi
	int Utf8ToAnsi(const char* buf, int nLen, CBuffer& out);
	int AnsiToUtf8(const char* buf, int nLen, CBuffer& out);

	// base64
	bool Base64Encode(const char* data, int nLen, CBuffer& buffer);
	bool Base64Decode(const char* data, int nLen, CBuffer& buffer);

	
	int CompareStringNoCase(const _tstring& src, const _tstring& dest);
	void TrimStringLeft(_tstring& str, TCHAR ch);
	void TrimStringRight(_tstring& str, TCHAR ch);
	int SplitString(std::vector<_tstring>& rst, LPCTSTR lpszText, int nLen, TCHAR ch, BOOL bEmpty = FALSE);

	// url code
	std::string UrlEncode(const std::string& str);
	std::string UrlDecode(const std::string& str);

	DWORD ReadBufferFromFile(CBuffer& buf, LPCTSTR lpszFile);
	DWORD WriteBufferToFile(const CBuffer& buf, LPCTSTR lpszFile);


	// huffman 
	bool HuffmanEncode(const char* data, int nLen, CBuffer& buffer);
	bool HuffmanDecode(const char* data, int nLen, CBuffer& buffer);

	bool ParseKeyValue(const _tstring& strText, _tstring& strKey, _tstring& strValue, TCHAR ch = _T('='));

	void FindDirAllFile(LPCTSTR lpszDir, std::vector<_tstring>& vcFile);
	void FindDirAllDir(LPCTSTR lpszDir, std::vector<_tstring>& vcFile);
	void MakeDir(LPCTSTR lpszDir);

	bool ReadUInt16FromBuffer(const char* & data, int& len, uint16& ret);
	bool ReadUInt32FromBuffer(const char* & data, int& len, uint32& ret);
	// 返回读取的字节长度，包括字符串结束符，如果没有结束符，str为空，返回值为size
	bool ReadStringFromBuffer(const char*& data, int& len, _tstring& str);

	void InitXlfBinaryFileHeader(_xlf_common_binary_file_header_t* header, uint32 flag, int version = 0, int nHeadSize = 0);
	bool IsErrorXlfBinaryFileHeader(_xlf_common_binary_file_header_t* header, uint32 flag, int total, int version = 0, int nHeadSize = 0);

}