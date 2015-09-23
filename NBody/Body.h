#ifndef BODY_H
#define BODY_H

#include "Math_3d.h"

struct Body
{
	Vector2f position;
	Vector2f r;
	Vector2f v;
	Vector2f f;


	Body() : position(0.5, 0.5), r(5.0,0.0), v(0.0, 0.0), f(0.0,0.0)
	{

	}

	Body(Vector2f pos, Vector2f _r, Vector2f _v, Vector2f _f) : position(pos), r(_r), v(_v), f(_f)
	{

	}
};

#endif