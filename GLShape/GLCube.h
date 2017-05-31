#ifndef _H_GL_CUBE_H
#define _H_GL_CUBE_H

#include "../GLVector/GLVector.h"

class GLCube
{
public:
	GLCube();

	double minx;
	double miny;
	double minz;
	double maxx;
	double maxy;
	double maxz;

public:
	void Expand(const GLVector& pt);
	void Expand(double amount);
};

#endif