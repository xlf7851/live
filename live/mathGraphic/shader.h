#pragma once

/* opengl 着色器
 * 1、顶点着色器
 * 2、片段着色器
 */
namespace ns_opengl
{
	class CShader
	{
	public:
		// 构造器读取并构建着色器
		CShader(bool bAutoRelease = true);
		CShader(const TCHAR* vertexPath, const TCHAR* fragmentPath, bool bAutoRelease = true);
		~CShader();

		void LoadFromFile(const TCHAR* vertexPath, const TCHAR* fragmentPath);
		void LoadFromData(const TCHAR* vertexData, const TCHAR* fragmentData);

		void ReleaseProgram();
		unsigned int GetProgram();
		void Use();

		static unsigned int BuildShader(const TCHAR* data, unsigned int tp);

		// uniform工具函数
		void SetBool(const TCHAR* name, bool value) const;
		void SetInt(const TCHAR* name, int value) const;
		void SetFloat(const TCHAR* name, float value) const;
	protected:
		
		
		

	protected:
		unsigned int m_ID;		// 着色器id
		bool m_bAutoRelease;
	};
}
