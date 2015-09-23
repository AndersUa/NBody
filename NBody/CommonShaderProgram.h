#ifndef COMMONSHADERPROGRAM_H
#define COMMONSHADERPROGRAM_H

#include "ShaderProgram.h"

class CommonShaderProgram : public ShaderProgram
{
private:
	//World matrix (transition+rotation+scale)
	GLuint WMatLocation;
	//world view prespective matrix (prespective+camera+transition+rotation+scale)
	GLuint VPWMatLocation;
	GLuint AmbientIntensityLocation;
	GLuint DiffuseIntensityLocation;
	GLuint ColorLocation;
	GLuint DirectionLocation;
	GLuint SmaplerLocation;
	GLuint SpecularPowerLocation;
	GLuint SpecularIntensityLocation;
	GLuint CameraPosLocation;
	GLuint IsSelectedLoacation;

	GLuint ScaleLocation;
	GLuint OffsetLocation;
	GLuint AspectRatioLocation;
public:
	CommonShaderProgram(const std::string& vsFname, const std::string& fsFname) : ShaderProgram(vsFname, fsFname)
	{

	}

	CommonShaderProgram(const std::string& vsFname, const std::string& fsFname, const std::string& gsFname) : ShaderProgram(vsFname, fsFname, gsFname)
	{

	}

	virtual void Apply()
	{
		ShaderProgram::Apply();

		if (SmaplerLocation != INVALID_UNIFORM_LOCATION)
		{
			glUniform1i(SmaplerLocation, 0);
		}
	}

	virtual bool Compile()
	{
		if (!ShaderProgram::Compile())
		{
			return false;
		}
		WMatLocation = GetUniformLocation("World");
		VPWMatLocation = GetUniformLocation("WorldVP");
		AmbientIntensityLocation = GetUniformLocation("light.AmbientIntensity");
		DiffuseIntensityLocation = GetUniformLocation("light.DiffuseIntensity");
		ColorLocation = GetUniformLocation("light.Color");
		DirectionLocation = GetUniformLocation("light.Direction");
		SmaplerLocation = GetUniformLocation("Sampler");
		SpecularPowerLocation = GetUniformLocation("light.SpecularPower");
		SpecularIntensityLocation = GetUniformLocation("light.SpecularIntensity");
		CameraPosLocation = GetUniformLocation("CameraPos");
		IsSelectedLoacation = GetUniformLocation("IsSelected");
		ScaleLocation = GetUniformLocation("Scale");
		OffsetLocation = GetUniformLocation("Offset");
		AspectRatioLocation = GetUniformLocation("AspectRatio");
		return true;
	}

	virtual void SetScale(float scale)
	{
		if (ScaleLocation != INVALID_UNIFORM_LOCATION)
			glUniform1f(ScaleLocation, scale);
	}
	virtual void SetOffset(Vector2f offset)
	{
		if (OffsetLocation != INVALID_UNIFORM_LOCATION)
			glUniform2f(OffsetLocation, offset.x, offset.y);
	}

	virtual void SetAspectRatio(float ar)
	{
		if (AspectRatioLocation != INVALID_UNIFORM_LOCATION)
			glUniform1f(AspectRatioLocation, ar);
	}

	virtual void SetWorldMatrix(Matrix4f mat)
	{
		if (WMatLocation != INVALID_UNIFORM_LOCATION)
			glUniformMatrix4fv(WMatLocation, 1, GL_TRUE, (const GLfloat*)mat.m);
	}

	virtual void SetWorldVPMatrix(Matrix4f mat)
	{
		if (VPWMatLocation != INVALID_UNIFORM_LOCATION)
			glUniformMatrix4fv(VPWMatLocation, 1, GL_TRUE, (const GLfloat*)mat.m);
	}

	virtual void SetIsSelected(bool val)
	{
		if (IsSelectedLoacation != INVALID_UNIFORM_LOCATION)
		{
			glUniform1i(IsSelectedLoacation, val);
		}
	}

	virtual void SetCameraPos(Vector3f cameraPos)
	{
		if (CameraPosLocation != INVALID_UNIFORM_LOCATION)
		{
			glUniform3f(CameraPosLocation, cameraPos.x, cameraPos.y, cameraPos.z);
		}
	}
};

#endif