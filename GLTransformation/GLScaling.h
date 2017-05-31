#ifndef _H_GL_SCALING_H
#define _H_GL_SCALING_H

#include "GLTransformation.h"

class GLScaling : public GLTransformation
{
public:
	GLScaling(double _x, double _y, double _z)
		: x(_x)
		, y(_y)
		, z(_z)
	{
	}
	GLScaling()
		: x(1)
		, y(1)
		, z(1)
	{
	}

	virtual void apply();

public:
	double x;
	double y;
	double z;
};
#endif