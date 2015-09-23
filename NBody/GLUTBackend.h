#ifndef GLUTBACKEND_H
#define GLUTBACKEND_H

#include "IBackend.h"

class GLUTBackend : public IBackend
{
private:
	IAppCallbacks* s_pCallbacks;
	static GLUTBackend* m_instance;

	int m_winWidth;
	int m_winHeight;
	int m_prevMouseX;
	int m_prevMouseY;

	void InitCallbacks();
	static void IdleCB();
	static void RenderSceneCB();
	static void PassiveMouseCB(int x, int y);
	static void KeyboardCB(unsigned char Key, int x, int y);
	static void SpecialKeyboardCB(int Key, int x, int y);
	static void GLUTBackend::CloseCB();
	static KEY GLUTKeyToKey(uint Key);
	


	GLUTBackend()
	{
		m_winWidth = 0;
		m_winHeight = 0;
	}


	GLUTBackend(GLUTBackend& other)
	{

	}

	GLUTBackend & operator= (const GLUTBackend & other)
	{

	}

public:
	virtual void Init(int argc, char** argv);
	virtual void Run(IAppCallbacks* callbacks);
	virtual bool CreateWin(uint Width, uint Height, bool isFullScreen, const char* Title);
	virtual void SwapBuffers();
	virtual void Terminate();
	virtual void CaptureMouse(){};
	virtual void ReleaseMouse(){};

	static IBackend* GetInstance()
	{
		if (m_instance == nullptr)
		{
			m_instance = new GLUTBackend();
		}
		return (IBackend*)m_instance;
	}

};

#endif