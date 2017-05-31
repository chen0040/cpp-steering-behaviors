#ifndef _H_GL_FRUSTUM_H
#define _H_GL_FRUSTUM_H

#include "GLPlane.h"
#include "../GLVector/GLVector.h"
#include "GLCube.h"
#include "GLSphere.h"
#include "../GLExtension/glxs.h"

class GLFrustum
{
private:
	GLPlane left;
	GLPlane right;
	GLPlane top;
	GLPlane bottom;
	GLPlane near;
	GLPlane far;

public:
	void MakeCameraFrustum();

protected:
	void ExtractPlane(GLPlane& plane, const GLfloat* mat, int row);
	bool PointOutsidePlane(const GLPlane& plane, const GLVector& pt) const;
	bool SphereOutsidePlane(const GLPlane& plane, const GLSphere& sphere) const;
	double DistFromPointToPlane(const GLPlane& plane, const GLVector& pt) const;

public:
	bool PointInFrustum(const GLVector& pt) const;
	bool SphereInFrustum(const GLSphere& sphere) const;
	bool CubeInFrustum(const GLCube& cube) const;
};
#endif