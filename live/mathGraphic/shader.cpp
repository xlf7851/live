#include "stdafx.h"
#include "shader.h"
#include <glad/glad.h>
#include "../base//utl.h"
#include "../base/buffer.h"
namespace ns_opengl
{

	CShader::CShader(bool bAutoRelease)
	{
		m_ID = 0;
		m_bAutoRelease = bAutoRelease;
	}

	CShader::CShader(const TCHAR* vertexPath, const TCHAR* fragmentPath, bool bAutoRelease)
	{
		m_ID = 0;
		m_bAutoRelease = bAutoRelease;
		LoadFromFile(vertexPath, fragmentPath);
	}

	CShader::~CShader()
	{
		if (m_bAutoRelease)
		{
			ReleaseProgram();
		}
		
	}

	void CShader::ReleaseProgram()
	{
		if (m_ID != 0)
		{
			glDeleteProgram(m_ID);
			m_ID = 0;
		}
	}

	unsigned int CShader::GetProgram()
	{
		return m_ID;
	}

	void CShader::Use()
	{
		if (m_ID)
		{
			glUseProgram(m_ID);
		}
	}

	void CShader::LoadFromFile(const TCHAR* vertexPath, const TCHAR* fragmentPath)
	{
		xlf::CBuffer bufVertex;
		xlf::CBuffer bufFragment;

		xlf::ReadBufferFromFile(bufVertex, vertexPath);
		xlf::ReadBufferFromFile(bufFragment, fragmentPath);

		if (bufVertex.GetSize() == 0 || bufFragment.GetSize() == 0)
		{
			return;
		}

		LoadFromData((TCHAR*)bufVertex.GetBuffer(), (TCHAR*)bufFragment.GetBuffer());

	}

	void CShader::LoadFromData(const TCHAR* vertexData, const TCHAR* fragmentData)
	{
		ReleaseProgram();
		// 顶点着色器
		GLuint vertex = BuildShader(vertexData, GL_VERTEX_SHADER);
		if (vertex == 0)
		{
			return;
		}
		GLuint fragment = BuildShader(fragmentData, GL_FRAGMENT_SHADER);
		if (fragment == 0)
		{
			glDeleteShader(vertex);
			return;
		}

		// 着色器程序
		m_ID = glCreateProgram();
		if (m_ID != 0)
		{
			glAttachShader(m_ID, vertex);
			glAttachShader(m_ID, fragment);
			glLinkProgram(m_ID);

			int success = 0;
			glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
			if (!success)
			{
				glDeleteProgram(m_ID);
				m_ID = 0;
			}
		}
		

		glDeleteShader(vertex);
		glDeleteShader(fragment);

	}

	unsigned int CShader::BuildShader(const TCHAR* data, unsigned int tp)
	{
		// 创建着色器对象
		GLuint id = glCreateShader(tp);
		if (id == 0)
		{
			return id;
		}

		//编译着色器源码
		glShaderSource(id, 1, &data, NULL);
		glCompileShader(id);

		int success = 0;
		glGetShaderiv(id, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			char infoLog[513] = { 0 };
			glGetShaderInfoLog(id, 512, NULL, infoLog);
			glDeleteShader(id);
			id = 0;
		}

		return id;
	}

	void CShader::SetBool(const TCHAR* name, bool value) const
	{
		ASSERT(m_ID > 0);
		glUniform1i(glGetUniformLocation(m_ID, name), (int)value);
	}

	void CShader::SetFloat(const TCHAR* name, float value) const
	{
		ASSERT(m_ID > 0);
		glUniform1i(glGetUniformLocation(m_ID, name), value);
	}

	void CShader::SetInt(const TCHAR* name, int value) const
	{
		ASSERT(m_ID > 0);
		glUniform1f(glGetUniformLocation(m_ID, name), value);
	}
}