#ifndef _H_GL_LINE_SEGMENT_H
#define _H_GL_LINE_SEGMENT_H

#include "../GLVector/GLVector.h"

class GLPlane;

class GLLineSegment
{
public:
	GLLineSegment() { }
	GLLineSegment(const GLVector& _from, const GLVector& _to);
	virtual ~GLLineSegment() { }

public:
	GLLineSegment(const GLLineSegment& rhs);
	GLLineSegment& operator=(const GLLineSegment& rhs);

public:
	bool intersect(const GLLineSegment& line, GLVector& intersection_point, double& distance);
	bool intersect(const GLPlane& plane, GLVector& intersection_point, double & distance);
	

	GLVector from;
	GLVector to;
};
#endif