#ifndef IBACKEND_H
#define IBACKEND_H

#include "types.h"
#include "IAppCallbacks.h"

class IBackend
{
public:
	virtual void Init(int argc, char** argv) = 0;
	virtual void Run(IAppCallbacks* callbacks) = 0;
	virtual bool CreateWin(uint Width, uint Height, bool isFullScreen, const char* Title) = 0;
	virtual void SwapBuffers()=0;
	virtual void Terminate() = 0;
	virtual void CaptureMouse() = 0;
	virtual void ReleaseMouse() = 0;
	virtual ~IBackend(){};
};

#endif