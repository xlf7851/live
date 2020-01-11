#include "stdafx.h"
#include "file.h"

namespace xlf
{
	CTFile::CTFile()
	{
		m_hFile = INVALID_HANDLE_VALUE;
	}

	CTFile::~CTFile()
	{
		Close();
	}

	bool CTFile::Open(LPCTSTR file, DWORD mode)
	{
		if (m_hFile != INVALID_HANDLE_VALUE)
		{
			return false;
		}

		DWORD dwReadmode = 0;
		if (mode & mode_read)
		{
			dwReadmode |= GENERIC_READ;
		}
		if (mode & mode_write)
		{
			dwReadmode |= GENERIC_WRITE;
		}

		DWORD dwCreateMode = CREATE_NEW;
		if (mode & mode_create)
		{
			dwCreateMode = OPEN_ALWAYS;
		}
		else if (mode & mode_exist)
		{
			dwCreateMode = OPEN_EXISTING;
		}
		m_hFile = ::CreateFile(file, dwReadmode, 0, NULL, dwCreateMode, FILE_ATTRIBUTE_NORMAL, NULL);
		

		return INVALID_HANDLE_VALUE != m_hFile;
	}

	DWORD CTFile::Read(void* buf, DWORD len)
	{
		if (INVALID_HANDLE_VALUE == m_hFile)
		{
			return 0;
		}

		DWORD nReadLen = 0;
		::ReadFile(m_hFile, buf, len, &nReadLen, NULL);
		return nReadLen;
	}

	DWORD CTFile::Write(const void* buf, DWORD len)
	{
		if (INVALID_HANDLE_VALUE == m_hFile)
		{
			return 0;
		}

		DWORD nRet = 0;
		::WriteFile(m_hFile, buf, len, &nRet, NULL);

		return nRet;
	}

	void CTFile::Close()
	{
		if (m_hFile != INVALID_HANDLE_VALUE)
		{
			CloseHandle(m_hFile);
		}
	}

	DWORD CTFile::GetFileLength()
	{
		if (m_hFile != INVALID_HANDLE_VALUE)
		{
			return ::GetFileSize(m_hFile, NULL);
		}

		return 0;
	}

	void CTFile::SetOffSet(DWORD nOffset)
	{
		if (m_hFile != INVALID_HANDLE_VALUE)
		{
			LARGE_INTEGER  liCurrentPosition;
			liCurrentPosition.QuadPart = 0;
			liCurrentPosition.LowPart = nOffset;
			::SetFilePointerEx(m_hFile, liCurrentPosition, NULL, FILE_CURRENT);
		}
	}

	DWORD CTFile::GetOffset()
	{
		if (m_hFile != INVALID_HANDLE_VALUE)
		{
			LARGE_INTEGER  liCurrentPosition;
			liCurrentPosition.QuadPart = 0;
			::SetFilePointerEx(m_hFile, liCurrentPosition, &liCurrentPosition, FILE_CURRENT);
			return liCurrentPosition.LowPart;
		}

		return 0;
	}

	void CTFile::SetBegin()
	{
		if (m_hFile != INVALID_HANDLE_VALUE)
		{
			LARGE_INTEGER  liCurrentPosition;
			liCurrentPosition.QuadPart = 0;

			::SetFilePointerEx(m_hFile, liCurrentPosition, NULL, FILE_BEGIN);
		}

	}

	void CTFile::SetEnd()
	{
		if (m_hFile != INVALID_HANDLE_VALUE)
		{
			LARGE_INTEGER  liCurrentPosition;
			liCurrentPosition.QuadPart = 0;
			::SetFilePointerEx(m_hFile, liCurrentPosition, NULL, FILE_END);
		}
	}

	void CTFile::EndFile()
	{
		if (m_hFile != INVALID_HANDLE_VALUE)
		{
			::SetEndOfFile(m_hFile);
		}
	}
}