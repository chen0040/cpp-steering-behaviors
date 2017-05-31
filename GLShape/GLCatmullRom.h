#ifndef _H_GL_CATMULL_ROM_H
#define _H_GL_CATMULL_ROM_H

#include "GLCurve.h"

class GLCatmullRom : public GLCurve
{
public:
	GLCatmullRom();
	virtual ~GLCatmullRom();

protected:
	virtual void OnNewControlPointAdded();

protected:
	GLVector GetPoint(double u, const GLVector& P0, const GLVector& P1, const GLVector& P2, const GLVector& P3);
};

#endif