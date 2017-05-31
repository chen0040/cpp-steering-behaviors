#ifndef _H_GL_SPHERE_H
#define _H_GL_SPHERE_H

#include "../GLVector/GLVector.h"

class GLSphere
{
public:
	GLSphere();
	GLSphere(const GLVector& c, double r);
	virtual ~GLSphere() { }

public:
	GLSphere(const GLSphere& rhs);
	GLSphere& operator=(const GLSphere& rhs);

public:
	double radius;
	GLVector center;
};
#endif