#ifndef _H_GL_TRANSLATION_H
#define _H_GL_TRANSLATION_H

#include "GLTransformation.h"

class GLTranslation : public GLTransformation
{
public:
	GLTranslation(double _x, double _y, double _z)
		: x(_x)
		, y(_y)
		, z(_z)
	{
	}
	GLTranslation()
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