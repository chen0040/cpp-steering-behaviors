#include "GLSphere.h"

GLSphere::GLSphere()
: radius(10)
{
}

GLSphere::GLSphere(const GLVector& c, double r)
{
	radius=r;
	center=c;
}

GLSphere::GLSphere(const GLSphere& rhs)
{
	radius=rhs.radius;
	center=rhs.center;
}

GLSphere& GLSphere::operator =(const GLSphere &rhs)
{
	radius=rhs.radius;
	center=rhs.center;

	return *this;
}