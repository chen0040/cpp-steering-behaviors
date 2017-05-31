#include "GLFrustum.h"
#include <cmath>
#include <vector>

void GLFrustum::MakeCameraFrustum()
{
	GLfloat projection[16];
	GLfloat modelview[16];

	glGetFloatv(GL_PROJECTION_MATRIX, projection);
	glGetFloatv(GL_MODELVIEW_MATRIX, modelview);

	glPushMatrix();
	glLoadMatrixf(projection);
	glMultMatrixf(modelview);
	glGetFloatv(GL_MODELVIEW_MATRIX, modelview);
	glPopMatrix();

	ExtractPlane(this->left, modelview, 1);
	ExtractPlane(this->right, modelview, -1);
	ExtractPlane(this->bottom, modelview, 2);
	ExtractPlane(this->top, modelview, -2);
	ExtractPlane(this->near, modelview, 3);
	ExtractPlane(this->far, modelview, -3);
}

void GLFrustum::ExtractPlane(GLPlane& plane, const GLfloat* mat, int row)
{
	int scale=(row < 0) ? -1 : 1;
	row = abs(row) - 1; 

	// calculate plane coefficients from the matrix;
	plane.normal.x=mat[3] + scale * mat[row];
	plane.normal.y=mat[7] + scale * mat[row + 4];
	plane.normal.z=mat[11] + scale * mat[row + 8];
	plane.D=mat[15] + scale * mat[row + 12];

	//normalize the plane
	double length=plane.normal.length();

	plane.normal.x /= length;
	plane.normal.y /= length;
	plane.normal.z /= length;
	plane.D /= length;
}

bool GLFrustum::PointInFrustum(const GLVector& pt) const
{
	bool point_outside_plane=false;
	point_outside_plane=PointOutsidePlane(this->left, pt);
	if(point_outside_plane==true)
	{
		return false;
	}
	point_outside_plane=PointOutsidePlane(this->right, pt);
	if(point_outside_plane==true)
	{
		return false;
	}
	point_outside_plane=PointOutsidePlane(this->top, pt);
	if(point_outside_plane==true)
	{
		return false;
	}
	point_outside_plane=PointOutsidePlane(this->bottom, pt);
	if(point_outside_plane==true)
	{
		return false;
	}
	point_outside_plane=PointOutsidePlane(this->near, pt);
	if(point_outside_plane==true)
	{
		return false;
	}
	point_outside_plane=PointOutsidePlane(this->far, pt);
	if(point_outside_plane==true)
	{
		return false;
	}

	return true;
}

bool GLFrustum::PointOutsidePlane(const GLPlane& plane, const GLVector& pt) const
{
	if((plane.normal.dot(pt) + plane.D) < 0)
	{
		return true;
	}
	return false;
}

double GLFrustum::DistFromPointToPlane(const GLPlane& plane, const GLVector& pt) const
{
	return plane.normal.dot(pt) + plane.D;
}

bool GLFrustum::SphereOutsidePlane(const GLPlane& plane, const GLSphere& sphere) const
{
	if(DistFromPointToPlane(plane, sphere.center) <= -sphere.radius)
	{
		return true;
	}
	return false;
}

bool GLFrustum::SphereInFrustum(const GLSphere& sphere) const
{
	bool sphere_outside_plane=false;
	sphere_outside_plane=SphereOutsidePlane(this->left, sphere);
	if(sphere_outside_plane==true)
	{
		return false;
	}
	sphere_outside_plane=SphereOutsidePlane(this->right, sphere);
	if(sphere_outside_plane==true)
	{
		return false;
	}
	sphere_outside_plane=SphereOutsidePlane(this->top, sphere);
	if(sphere_outside_plane==true)
	{
		return false;
	}
	sphere_outside_plane=SphereOutsidePlane(this->bottom, sphere);
	if(sphere_outside_plane==true)
	{
		return false;
	}
	sphere_outside_plane=SphereOutsidePlane(this->near, sphere);
	if(sphere_outside_plane==true)
	{
		return false;
	}
	sphere_outside_plane=SphereOutsidePlane(this->far, sphere);
	if(sphere_outside_plane==true)
	{
		return false;
	}

	return true;

}

bool GLFrustum::CubeInFrustum(const GLCube& cube) const
{
	std::vector<GLVector> pts;
	pts.push_back(GLVector(cube.minx, cube.miny, cube.minz));
	pts.push_back(GLVector(cube.maxx, cube.miny, cube.minz));
	pts.push_back(GLVector(cube.maxx, cube.miny, cube.maxz));
	pts.push_back(GLVector(cube.minx, cube.miny, cube.maxz));

	pts.push_back(GLVector(cube.minx, cube.maxy, cube.minz));
	pts.push_back(GLVector(cube.maxx, cube.maxy, cube.minz));
	pts.push_back(GLVector(cube.maxx, cube.maxy, cube.maxz));
	pts.push_back(GLVector(cube.minx, cube.maxy, cube.maxz));

	for(size_t i=0; i<pts.size(); i++)
	{
		if(PointInFrustum(pts[i]))
		{
			return true;
		}
	}
	return false;
}