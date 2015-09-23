#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <GL\glew.h>
#include "Math_3d.h"
#include "types.h"
#include <string>
#include <stdio.h>

#define INVALID_UNIFORM_LOCATION 0xffffffff

class ShaderProgram
{
private:
	std::string m_vsFname;
	std::string m_fsFname;
	std::string m_gsFname;
protected:
	GLuint m_shaderProgram;

	bool AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType);
	GLint GetUniformLocation(const char* pUniformName);
public:
	const std::string& GetVSFileName()
	{
		return m_vsFname;
	}

	const std::string& GetFSFileName()
	{
		return m_fsFname;
	}

	virtual void SetWorldMatrix(Matrix4f mat){};
	virtual void SetWorldVPMatrix(Matrix4f mat){};
	virtual void SetCameraPos(Vector3f cameraPos){};
	virtual void SetIsSelected(bool val){};
	virtual void SetScale(float scale){};
	virtual void SetOffset(Vector2f scale){};
	virtual void SetAspectRatio(float ar){ };

	ShaderProgram(const std::string& vsFname, const std::string& fsFname)
	{
		m_vsFname = vsFname;
		m_fsFname = fsFname;
	}

	ShaderProgram(const std::string& vsFname, const std::string& fsFname, const std::string& gsFname)
	{
		m_vsFname = vsFname;
		m_fsFname = fsFname;
		m_gsFname = gsFname;
	}

	virtual bool Compile();
	virtual void Apply();
	void Delete();
	virtual ~ShaderProgram(){};
};

#endif