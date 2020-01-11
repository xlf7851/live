#pragma once
namespace xlf
{
	class CTFile
	{
	public:
		enum open_mode
		{
			mode_read		= 0x0001,
			mode_write		= 0x0002,
			mode_create		= 0x0004, // ��һ���ļ������������½�
			mode_exist		= 0x0008, // ��һ���Ѿ������ļ��� ��������򿪻�ʧ��

		};
	public:
		CTFile();
		~CTFile();

		bool Open(LPCTSTR filePath, DWORD mode = mode_read | mode_exist);
		void Close();

		DWORD Write(const void* buf, DWORD len);
		DWORD Read(void* buf, DWORD len);

		DWORD GetFileLength();

		void SetOffSet(DWORD nOffset);
		DWORD GetOffset();
		void SetBegin();
		void SetEnd();
		void EndFile();
	protected:
		HANDLE  m_hFile;
	};
}