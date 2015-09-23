#include <stdio.h>
#include <GL/freeglut.h>

#include "GLUTBackend.h"
#include "Keys.h"

GLUTBackend* GLUTBackend::m_instance = nullptr;

KEY GLUTBackend::GLUTKeyToKey(uint Key)
{
	switch (Key) {
	case GLUT_KEY_F1:
		return KEY_F1;
	case GLUT_KEY_F2:
		return KEY_F2;
	case GLUT_KEY_F3:
		return KEY_F3;
	case GLUT_KEY_F4:
		return KEY_F4;
	case GLUT_KEY_F5:
		return KEY_F5;
	case GLUT_KEY_F6:
		return KEY_F6;
	case GLUT_KEY_F7:
		return KEY_F7;
	case GLUT_KEY_F8:
		return KEY_F8;
	case GLUT_KEY_F9:
		return KEY_F9;
	case GLUT_KEY_F10:
		return KEY_F10;
	case GLUT_KEY_F11:
		return KEY_F11;
	case GLUT_KEY_F12:
		return KEY_F12;
	case GLUT_KEY_LEFT:
		return KEY_LEFT;
	case GLUT_KEY_UP:
		return KEY_UP;
	case GLUT_KEY_RIGHT:
		return KEY_RIGHT;
	case GLUT_KEY_DOWN:
		return KEY_DOWN;
	case GLUT_KEY_PAGE_UP:
		return KEY_PAGE_UP;
	case GLUT_KEY_PAGE_DOWN:
		return KEY_PAGE_DOWN;
	case GLUT_KEY_HOME:
		return KEY_HOME;
	case GLUT_KEY_END:
		return KEY_END;
	case GLUT_KEY_INSERT:
		return KEY_INSERT;
	case GLUT_KEY_DELETE:
		return KEY_DELETE;
	
	//default:
	//	//ERROR("Unimplemented GLUT key");
	//	//exit(1);
	}

	return KEY_UNDEFINED;
}


void GLUTBackend::SpecialKeyboardCB(int Key, int x, int y)
{
	KEY OgldevKey = GLUTKeyToKey(Key);
	m_instance->s_pCallbacks->OnKeyPressed(OgldevKey);
}


void GLUTBackend::KeyboardCB(unsigned char Key, int x, int y)
{
	if (Key == 27)
	{
		m_instance->s_pCallbacks->OnKeyPressed(KEY_ESCAPE);
		return;
	}
	if (
		((Key >= '0') && (Key <= '9')) ||
		((Key >= 'A') && (Key <= 'Z')) ||
		((Key >= 'a') && (Key <= 'z'))
		) 
	{
		KEY key = (KEY)Key;
		m_instance->s_pCallbacks->OnKeyPressed(key);
	}
}


void GLUTBackend::PassiveMouseCB(int x, int y)
{
	m_instance->s_pCallbacks->OnMouseMove(x, y);
}


/*void GLUTBackend::MouseCB(int button, int state, int x, int y)
{
	m_instance->s_pCallbacks->OnMouseMove(x, y);
}*/

void GLUTBackend::RenderSceneCB()
{
	m_instance->s_pCallbacks->OnRender(glutGet(GLUT_ELAPSED_TIME));
}


void GLUTBackend::IdleCB()
{
	m_instance->s_pCallbacks->OnRender(glutGet(GLUT_ELAPSED_TIME));
}

void GLUTBackend::CloseCB()
{
	m_instance->s_pCallbacks->OnClose();
}

void GLUTBackend::InitCallbacks()
{
	glutDisplayFunc(GLUTBackend::RenderSceneCB);
	glutIdleFunc(this->IdleCB);
	glutSpecialFunc(this->SpecialKeyboardCB);
	glutPassiveMotionFunc(this->PassiveMouseCB);
	//glutmouse
	glutKeyboardFunc(this->KeyboardCB);
	glutCloseFunc(this->CloseCB);
}

void GLUTBackend::Init(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
}

void GLUTBackend::Run(IAppCallbacks* callbacks)
{
	if (!callbacks) {
		fprintf(stderr, "%s : callbacks not specified!\n", __FUNCTION__);
		return;
	}


	s_pCallbacks = callbacks;
	InitCallbacks();
	glutMainLoop();
}

bool GLUTBackend::CreateWin(uint Width, uint Height, bool isFullScreen, const char* Title)
{
	m_winHeight = Height;
	m_winWidth = Width;
	glutInitWindowSize(Width, Height);
	glutCreateWindow(Title);
	glutWarpPointer(Width / 2, Height / 2);
	return true;
}

void GLUTBackend::SwapBuffers()
{
	glutSwapBuffers();
}

void GLUTBackend::Terminate()
{
	glutLeaveMainLoop();
}