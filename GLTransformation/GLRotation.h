#ifndef _H_GL_ROTATION_H
#define _H_GL_ROTATION_H

#include "GLTransformation.h"

class GLRotation : public GLTransformation
{
public:
	GLRotation(double _angle_in_degree, double _x, double _y, double _z)
		: angle_in_degree(_angle_in_degree)
		, x(_x)
		, y(_y)
		, z(_z)
	{

	}
	GLRotation()
		: angle_in_degree(90)
		, x(1)
		, y(0)
		, z(0)
	{

	}

public:
	virtual void apply();

public:
	double angle_in_degree;
	double x;
	double y;
	double z;
};
#endif