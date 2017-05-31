#ifndef _H_GL_PLANE_H
#define _H_GL_PLANE_H

#include "../GLVector/GLVector.h"

class GLPlane
{
public:
	GLPlane() { }
	virtual ~GLPlane() { }
	GLPlane(const GLVector& _normal, double _D) { normal=_normal; D=_D; }

	GLVector normal;
	double D;
};
#endif