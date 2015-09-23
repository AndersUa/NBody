#include "ShaderProgram.h"

#include <GL\glew.h>
#include <stdio.h>
#include "IOUtil.h"

bool ShaderProgram::AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
	GLuint ShaderObj = glCreateShader(ShaderType);

	if (ShaderObj == 0) {
		fprintf(stderr, "Error creating shader type %d\n", ShaderType);
		return false;
	}

	const GLchar* p[1];
	p[0] = pShaderText;
	GLint Lengths[1];
	Lengths[0] = strlen(pShaderText);
	glShaderSource(ShaderObj, 1, p, Lengths);
	glCompileShader(ShaderObj);
	GLint success;
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		std::string shaderType;
		switch (ShaderType)
		{
		case GL_VERTEX_SHADER:
			shaderType = "GL_VERTEX_SHADER";
			break;
		case GL_GEOMETRY_SHADER:
			shaderType = "GL_GEOMETRY_SHADER";
			break;
		case GL_FRAGMENT_SHADER:
			shaderType = "GL_FRAGMENT_SHADER";
			break;
		default:
			shaderType = "Unknown";
			break;
		}
		fprintf(stderr, "Error compiling shader type %s: '%s'\n", shaderType.c_str(), InfoLog);
		return false;
	}

	glAttachShader(ShaderProgram, ShaderObj);
	return true;
}

bool ShaderProgram::Compile()
{
	m_shaderProgram = glCreateProgram();

	if (m_shaderProgram == 0) {
		fprintf(stderr, "Error creating shader program\n");
		return false;
	}

	std::string vs, gs, fs;

	if (!ReadFile(m_vsFname, vs))
	{
		fprintf(stderr, "File not exists %s \n", m_vsFname.c_str());
		return false;
	}

	if (!ReadFile(m_fsFname, fs))
	{
		fprintf(stderr, "File not exists %s \n", m_vsFname.c_str());
		return false;
	}

	if (!AddShader(m_shaderProgram, vs.c_str(), GL_VERTEX_SHADER))
	{
		return false;
	}
	if (!AddShader(m_shaderProgram, fs.c_str(), GL_FRAGMENT_SHADER))
	{
		return false;
	}

	if (!m_gsFname.empty())
	{
		if (!ReadFile(m_gsFname, gs))
		{
			fprintf(stderr, "File not exists %s \n", m_gsFname.c_str());
			return false;
		}
		if (!AddShader(m_shaderProgram, gs.c_str(), GL_GEOMETRY_SHADER))
		{
			return false;
		}
	}

	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };

	glLinkProgram(m_shaderProgram);
	glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &Success);
	if (Success == 0) {
		glGetProgramInfoLog(m_shaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
		return false;
	}

	glValidateProgram(m_shaderProgram);
	glGetProgramiv(m_shaderProgram, GL_VALIDATE_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(m_shaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
		return false;
	}

	return true;
}

void ShaderProgram::Apply()
{
	glUseProgram(m_shaderProgram);
}

GLint ShaderProgram::GetUniformLocation(const char* pUniformName)
{
	GLuint Location = glGetUniformLocation(m_shaderProgram, pUniformName);

	if (Location == INVALID_UNIFORM_LOCATION) {
		fprintf(stderr, "Warning! Unable to get the location of uniform '%s'\n", pUniformName);
	}

	return Location;
}

void ShaderProgram::Delete()
{
	glDeleteShader(m_shaderProgram);
}