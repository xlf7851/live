#include "stdafx.h"
#include "utl.h"
#include "buffer.h"
#include "file.h"

namespace xlf
{
	int Utf8ToUnicode(const char* buf, int len, wchar_t* outbuf, int outbuflen)
	{
		return ::MultiByteToWideChar(CP_UTF8, 0, buf, len, outbuf, outbuflen);
	}

	int UnicodeToUtf8(const wchar_t* buf, int len, char* out, int outbuflen)
	{
		return ::WideCharToMultiByte(CP_UTF8, 0, buf, len, out, outbuflen, NULL, NULL);
	}

	int Utf8ToUnicodeBufLength(const char* buf, int len)
	{
		return ::MultiByteToWideChar(CP_UTF8, 0, buf, len, NULL, 0);
			
	}
	int UnicodeToUtf8BufLength(const wchar_t* buf, int len)
	{
		return ::WideCharToMultiByte(CP_UTF8, 0, buf, len, NULL, 0, NULL, NULL);
	}

	int Utf8ToWstring(const char* buf, int len, wstring& outbuf)
	{
		int nLen = Utf8ToUnicodeBufLength(buf, len);
		if (nLen <= 0)
		{
			return 0;
		}
		outbuf.reserve(nLen + 1);
		outbuf.resize(nLen);
		wchar_t* out = (wchar_t*)outbuf.c_str();
		
		nLen = Utf8ToUnicode(buf, len, out, nLen);
		outbuf.resize(nLen);
		return nLen;
	}

	int WstringToUtf8(const wchar_t* buf, int len, CBuffer& out)
	{
		int nLen = UnicodeToUtf8BufLength(buf, len);
		if (nLen <= 0)
		{
			return 0;
		}
	
		out.Reserve(nLen + 1);
		out.Resize(nLen);

		nLen = UnicodeToUtf8(buf, len, (char*)out.GetBuffer(), nLen);
		out.Resize(nLen);
		return nLen;
	}

	int AnsiToUnicode(const char* buf, int len, wchar_t* outbuf, int outbuflen)
	{
		return ::MultiByteToWideChar(CP_ACP, 0, buf, len, outbuf, outbuflen);
	}

	int UnicodeToAnsi(const wchar_t* buf, int len, char* out, int outbuflen)
	{
		return ::WideCharToMultiByte(CP_ACP, 0, buf, len, out, outbuflen, NULL, NULL);
	}
	int AnsiToUnicodeBufLength(const char* buf, int len)
	{
		return ::MultiByteToWideChar(CP_ACP, 0, buf, len, NULL, 0);
	}

	int UnicodeToAnsiBufLength(const wchar_t* buf, int len)
	{
		return ::WideCharToMultiByte(CP_ACP, 0, buf, len, NULL, 0, NULL, NULL);
	}

	int AnsiToWstring(const char* buf, int len, wstring&outbuf)
	{
		int nLen = AnsiToUnicodeBufLength(buf, len);
		if (nLen <= 0)
		{
			return 0;
		}
		outbuf.reserve(nLen + 1);
		wchar_t* out = (wchar_t*)outbuf.c_str();
		outbuf.resize(nLen);

		nLen = AnsiToUnicode(buf, len, out, nLen);
		outbuf.resize(nLen);
		return nLen;
	}

	int WstringToAnsi(const wchar_t* buf, int len, CBuffer& out)
	{
		int nLen = UnicodeToAnsiBufLength(buf, len);
		if (nLen <= 0)
		{
			return 0;
		}

		out.Reserve(nLen + 1);
		out.Resize(nLen);


		nLen = UnicodeToAnsi(buf, len, (char*)out.GetBuffer(), nLen);
		out.Resize(nLen);
		return nLen;
	}

	// utf8 and ansi
	int Utf8ToAnsi(const char* buf, int nLen, CBuffer& out)
	{
		wstring str;
		Utf8ToWstring(buf, nLen, str);
		return WstringToAnsi(str.c_str(), str.size(), out);
	}

	int AnsiToUtf8(const char* buf, int nLen, CBuffer& out)
	{
		wstring str;
		AnsiToWstring(buf, nLen, str);
		return WstringToUtf8(str.c_str(), str.size(), out);
	}

	std::wstring AutoToWString(LPCTSTR lpszText)
	{
		std::wstring str;
#ifdef _UNICODE
		wstring = lpszText;
#else
		AnsiToWstring(lpszText, -1, str);
#endif
		return str;
	}

	static const unsigned char Base64EncodeMap[64] =
	{
		'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
		'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
		'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
		'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
		'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
		'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
		'w', 'x', 'y', 'z', '0', '1', '2', '3',
		'4', '5', '6', '7', '8', '9', '+', '/'
	};

	bool Base64Encode(const char* data, int nLen, CBuffer& buffer)
	{
		buffer.Clear();
		if (data == NULL || data[0] == 0 || nLen < 1)
		{
			return false;
		}

		int nGroup = (nLen + 2) / 3;
		int nMode = nGroup * 3 - nLen;
		// 编码后的总长度
		int nEncodeSize = nGroup * 4;
		buffer.Reserve(nEncodeSize + 10);
		buffer.Resize(nEncodeSize);
		unsigned char* dst = buffer.GetBuffer();
		const unsigned char* src = (const unsigned char*)data;
		for (int i = 0; i < nGroup; i++)
		{
			if (nMode != 0 && i == nGroup -1)	// 最后一组，需要判断是否填充
			{
				if (nMode == 2)
				{
					// 0 index byte
					dst[0] = Base64EncodeMap[src[0] >> 2];
					// 1 index byte
					dst[1] = Base64EncodeMap[(src[0] & 0x03) << 4];
					// 2 index byte
					dst[2] = '=';
					// 3 index byte
					dst[3] = '=';
				}
				else if (nMode == 1)
				{
					// 0 index byte
					dst[0] = Base64EncodeMap[src[0] >> 2];
					
					// 1 index byte
					dst[1] = Base64EncodeMap[((src[0] & 0x03) << 4) | (src[1] >> 4)];
		
					// 2 index byte
					dst[2] = Base64EncodeMap[((src[1] & 0x0F) << 2)];
					
					// 3 index byte
					dst[3] = '=';
				}

				break;
			}
			// 0 index byte
			dst[0] = Base64EncodeMap[src[0] >> 2];

			// 1 index byte
			dst[1] = Base64EncodeMap[((src[0] & 0x03) << 4) | (src[1] >> 4)];

			// 2 index byte
			dst[2] = Base64EncodeMap[((src[1] & 0x0F) << 2) | (src[2] >> 6)];
		
			// 3 index byte
			dst[3] = Base64EncodeMap[src[2] & 0x3F];

			src += 3;
			dst += 4;
		}

		return true;
	}

	static unsigned char Base64DecodeMap[256] =
	{
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0x3E, 0xFF, 0xFF, 0xFF, 0x3F,
		0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B,
		0x3C, 0x3D, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF,
		0xFF, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
		0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E,
		0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16,
		0x17, 0x18, 0x19, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20,
		0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28,
		0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30,
		0x31, 0x32, 0x33, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
	};

	bool Base64Decode(const char* data, int nLen, CBuffer& buffer)
	{
		buffer.Clear();
		if (data == NULL || data[0] == 0 || nLen < 1 || (nLen % 4 != 0)) // 必须是4的倍数
		{
			return false;
		}

		int nGroup = nLen / 4;
		// 编码后的总长度
		int top = 0;
		int nDecodeSize = nGroup * 3;
		buffer.Reserve(nDecodeSize + 10);
		//buffer.Resize(nDecodeSize);
		unsigned char* dst = buffer.GetBuffer();
		const unsigned char* src = (const unsigned char*)data;
		for (int i = 0; i < nGroup; i++)
		{
			if (src[0] == '=' || src[1] == '=')
			{
				return false;
			}

			dst[0] = (Base64DecodeMap[src[0]] << 2) | (Base64DecodeMap[src[1]] >> 4);
			if (src[2] != '=')
			{
				dst[1] = ((Base64DecodeMap[src[1]] & 0x0F) << 4) | (Base64DecodeMap[src[2]] >> 2);
				
			}
			else
			{
				if (i != nGroup - 1)
				{
					return false;
				}
				top = 2;
				break;
			}

			if (src[3] != '=')
			{
				dst[2] = (Base64DecodeMap[src[2]] << 6) | Base64DecodeMap[src[3]];
				
			}
			else
			{
				if (i != nGroup - 1)
				{
					return false;
				}

				top = 1;
				break;
			}

			src += 4;
			dst += 3;
		}

		nDecodeSize -= top;
		buffer.Resize(nDecodeSize);

		return true;
	}

	int CompareStringNoCase(const _tstring& src, const _tstring& dest)
	{
		return _tcsicmp(src.c_str(), dest.c_str());
	}

	void TrimStringLeft(_tstring& str, TCHAR ch)
	{
		if (str.size() == 0)
		{
			return;
		}

		int i = 0;
		int size = str.size();
		for (; i < size ; i++)
		{
			if (str[i] != ch)
			{
				break;
			}
		}

		if (i > 0)
		{
			if (i >= size)
			{
				str.clear();
			}
			else
			{
				_tstring strCp(str.c_str() + i, size - i);
				str = strCp;
			}
			
		}
	}

	void TrimStringRight(_tstring& str, TCHAR ch)
	{
		if (str.size() == 0)
		{
			return;
		}
		int i = str.size() - 1;
		for (; i >= 0; i--)
		{
			if (str[i] != ch)
			{
				break;
			}
		}

		if (i != str.size() -1)
		{
			if (i < 0)
			{
				str.clear();
			}
			else
			{
				str.resize(i+1);
			}
		}
	}

	int SplitString(std::vector<_tstring>& rst, LPCTSTR lpszText, int nLen, TCHAR ch, BOOL bEmpty)
	{
		if (lpszText == nullptr || lpszText[0] == 0)
		{
			return 0;
		}

		int nOldSize = rst.size();

		int nOffset = 0;
		int nBegin = nOffset;
		while (nOffset < nLen || nLen < 0 )
		{
			bool bLast = nOffset == nLen - 1 || (nLen < 0 && lpszText[nOffset] == 0);
			if (bLast || ch == lpszText[nOffset])
			{
				if (nOffset == nBegin)
				{
					if (bEmpty)
					{
						rst.push_back(_T(""));
					}
				}
				else
				{
					std::string str(&lpszText[nBegin], nOffset - nBegin);
					rst.push_back(str);
				}

				if (bLast)
				{
					break;
				}

				nBegin = nOffset + 1;
			}
			
			nOffset ++;
		}

		return rst.size() - nOldSize;
	}

	static inline unsigned char ToHex(unsigned char x)
	{
		return  x > 9 ? x + 55 : x + 48;
	}

	static inline unsigned char FromHex(unsigned char x)
	{
		unsigned char y;
		if (x >= 'A' && x <= 'Z') y = x - 'A' + 10;
		else if (x >= 'a' && x <= 'z') y = x - 'a' + 10;
		else if (x >= '0' && x <= '9') y = x - '0';
		else assert(0);
		return y;
	}

	std::string UrlEncode(const std::string& str)
	{
		std::string strTemp = "";
		size_t length = str.length();
		for (size_t i = 0; i < length; i++)
		{
			if (isalnum((unsigned char)str[i]) ||
				(str[i] == '-') ||
				(str[i] == '_') ||
				(str[i] == '.') ||
				(str[i] == '~'))
				strTemp += str[i];
			else if (str[i] == ' ')
				strTemp += "+";
			else
			{
				strTemp += '%';
				strTemp += ToHex((unsigned char)str[i] >> 4);
				strTemp += ToHex((unsigned char)str[i] % 16);
			}
		}
		return strTemp;
	}

	std::string UrlDecode(const std::string& str)
	{
		std::string strTemp = "";
		size_t length = str.length();
		for (size_t i = 0; i < length; i++)
		{
			if (str[i] == '+') strTemp += ' ';
			else if (str[i] == '%')
			{
				assert(i + 2 < length);
				unsigned char high = FromHex((unsigned char)str[++i]);
				unsigned char low = FromHex((unsigned char)str[++i]);
				strTemp += high * 16 + low;
			}
			else strTemp += str[i];
		}
		return strTemp;
	}

	DWORD ReadBufferFromFile(CBuffer& buf, LPCTSTR lpszFile)
	{
		CTFile file;
		if (!file.Open(lpszFile))
		{
			return 0;
		}

		DWORD len = file.GetFileLength();
		if (len == 0)
		{
			return 0;
		}
		buf.Reserve(len + 1);
		DWORD nRead = file.Read(buf.GetBuffer(), len);
		buf.Resize(nRead);

		return nRead;
	}

	DWORD WriteBufferToFile(const CBuffer& buf, LPCTSTR lpszFile)
	{
		if (buf.GetSize() == 0)
		{
			return 0;
		}

		CTFile file;
		if (!file.Open(lpszFile, CTFile::mode_create | CTFile::mode_write))
		{
			return 0;
		}

		return file.Write(buf.GetBuffer(), buf.GetSize());
	}

	bool HuffmanEncode(const char* data, int nLen, CBuffer& buffer)
	{
		return false;
	}

	bool HuffmanDecode(const char* data, int nLen, CBuffer& buffer)
	{
		return false;
	}

	bool ParseKeyValue(const _tstring& strText, _tstring& strKey, _tstring& strValue, TCHAR ch)
	{
		if (strText.size() > 0)
		{
			int flagIndex = strText.find(ch);
			if (flagIndex > 0)
			{
				strKey = strText.substr(0, flagIndex);
				strValue.clear();
				if (flagIndex < strText.size() -1)
				{
					int pos = flagIndex + 1;
					strValue = strText.substr(pos, strText.size() - pos);
				}

				return true;
			}
		}

		return false;
	}

	inline bool _IsDotFile(LPCTSTR lpszFile)
	{
		return _tcscmp(lpszFile, _T(".")) == 0 || _tcscmp(lpszFile, _T("..")) == 0;
	}
	void _FindDirAllFile(LPCTSTR lpszDir, std::vector<std::string>& vcFile, bool bFile)
	{
		std::string strPath = lpszDir;
		strPath += _T("*.*");

		WIN32_FIND_DATA pNextInfo;
		HANDLE hFile = FindFirstFile(strPath.c_str(), &pNextInfo);
		if (!hFile) {
			return;
		}
		string  t;

		while (FindNextFile(hFile, &pNextInfo))
		{
			if (_IsDotFile(pNextInfo.cFileName))//过滤.和..
				continue;

			if (pNextInfo.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
			{
				if (!bFile)
				{
					t = pNextInfo.cFileName;
					vcFile.push_back(t);
				}
			}
			else if (bFile)
			{
				t = pNextInfo.cFileName;
				vcFile.push_back(t);
			}
		}
		//CloseHandle(hFile);
	}

	void FindDirAllFile(LPCTSTR lpszDir, std::vector<std::string>& vcFile)
	{
		_FindDirAllFile(lpszDir, vcFile, true);
	}

	void FindDirAllDir(LPCTSTR lpszDir, std::vector<std::string>& vcFile)
	{
		_FindDirAllFile(lpszDir, vcFile, false);
	}
}