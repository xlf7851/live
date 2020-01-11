#pragma once


namespace xlf
{
	class CBuffer;

	// utf8 ºÍunicode
	int Utf8ToUnicode(const char* buf, int len,wchar_t* outbuf, int outbuflen);
	int UnicodeToUtf8(const wchar_t* buf, int len, char* out, int outbuflen);
	int Utf8ToUnicodeBufLength(const char* buf, int len);
	int UnicodeToUtf8BufLength(const wchar_t* buf, int len);
	int Utf8ToWstring(const char* buf, int len, wstring& out);
	int WstringToUtf8(const wchar_t* buf, int len, CBuffer& out);


	// ansi ºÍ unicode
	int AnsiToUnicode(const char* buf, int len, wchar_t* outbuf, int outbuflen);
	int UnicodeToAnsi(const wchar_t* buf, int len, char* out, int outbuflen);
	int AnsiToUnicodeBufLength(const char* buf, int len);
	int UnicodeToAnsiBufLength(const wchar_t* buf, int len);
	int AnsiToWstring(const char* buf, int len, wstring&out);
	int WstringToAnsi(const wchar_t* buf, int nLen, CBuffer& out);

	// utf8 and ansi
	int Utf8ToAnsi(const char* buf, int nLen, CBuffer& out);
	int AnsiToUtf8(const char* buf, int nLen, CBuffer& out);

	// base64
	bool Base64Encode(const char* data, int nLen, CBuffer& buffer);
	bool Base64Decode(const char* data, int nLen, CBuffer& buffer);


	int SplitString(std::vector<std::string>& rst, LPCTSTR lpszText, int nLen, TCHAR ch, BOOL bEmpty = FALSE);

	// url code
	std::string UrlEncode(const std::string& str);
	std::string UrlDecode(const std::string& str);

	DWORD ReadBufferFromFile(CBuffer& buf, LPCTSTR lpszFile);
	DWORD WriteBufferToFile(const CBuffer& buf, LPCTSTR lpszFile);

}