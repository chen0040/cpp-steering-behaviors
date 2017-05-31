#ifndef _H_GL_RAY_H
#define _H_GL_RAY_H

#include "../GLVector/GLVector.h"
#include <sdl.h>
#include "../GLExtension/glxs.h"

class GLFloor;
class GLSphere;

class GLRay
{
public:
	GLRay();
	virtual ~GLRay();

public:
	GLRay(const GLRay& rhs);
	GLRay& operator=(const GLRay& rhs);

public:
	virtual bool intersect(const GLFloor* plane, GLVector& intersection_point) const;
	virtual bool intersect(const GLSphere& sphere, GLVector& ip1, GLVector& ip2, double& closest_distance) const;
	virtual bool intersect(const GLSphere& sphere, double& closest_distance) const;

public:
	GLVector origin;
	GLVector direction;

};
#endif