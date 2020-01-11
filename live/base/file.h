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
			mode_create		= 0x0004, // 打开一个文件，不存在则新建
			mode_exist		= 0x0008, // 打开一个已经存在文件， 不存在则打开会失败

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