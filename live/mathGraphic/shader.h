#pragma once

/* opengl ��ɫ��
 * 1��������ɫ��
 * 2��Ƭ����ɫ��
 */
namespace ns_opengl
{
	class CShader
	{
	public:
		// ��������ȡ��������ɫ��
		CShader(bool bAutoRelease = true);
		CShader(const TCHAR* vertexPath, const TCHAR* fragmentPath, bool bAutoRelease = true);
		~CShader();

		void LoadFromFile(const TCHAR* vertexPath, const TCHAR* fragmentPath);
		void LoadFromData(const TCHAR* vertexData, const TCHAR* fragmentData);

		void ReleaseProgram();
		unsigned int GetProgram();
		void Use();

		static unsigned int BuildShader(const TCHAR* data, unsigned int tp);

		// uniform���ߺ���
		void SetBool(const TCHAR* name, bool value) const;
		void SetInt(const TCHAR* name, int value) const;
		void SetFloat(const TCHAR* name, float value) const;
	protected:
		
		
		

	protected:
		unsigned int m_ID;		// ��ɫ��id
		bool m_bAutoRelease;
	};
}
