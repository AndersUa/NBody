#ifndef IAPPCALLBACKS_H
#define IAPPCALLBACKS_H

#include "Keys.h"
#include "types.h"

class IAppCallbacks
{
public:
	virtual void OnKeyPressed(KEY OgldevKey) = 0;
	virtual void OnMouseMove(int x, int y) = 0;
	virtual void OnMouseKey(MOUSE_KEY key, MOUSE_STATE state, int x, int y) = 0;
	virtual void OnCapturedMouseMove(int dx, int dy) = 0;
	virtual void WindowsSizeChanged(int width, int height) = 0;
	virtual void OnRender(ulong time) = 0;
	virtual void OnClose() = 0;
};

#endif