#include "GLMouse.h"
#include <cassert>
#include <iostream>

GLMouse::GLMouse()
: m_bLeftButtonDown(false)
, m_bRightButtonDown(false)
, m_iX(0)
, m_iY(0)
, m_iXRel(0)
, m_iYRel(0)
, m_iButton(-1)
{

}

GLMouse::~GLMouse()
{

}

void GLMouse::MouseButtonDown(const int& iButton, const int& iX, const int& iY, const int& iXRel, const int& iYRel)
{
	if(iButton==SDL_BUTTON_LEFT)
	{
		m_bLeftButtonDown=true;
	}
	if(iButton==SDL_BUTTON_RIGHT)
	{
		m_bRightButtonDown=true;
	}

	m_iX=iX;
	m_iY=iY;
	m_iXRel=m_iXRel;
	m_iYRel=m_iYRel;
	m_iButton=iButton;
}

void GLMouse::MouseButtonUp(const int& iButton, const int& iX, const int& iY, const int& iXRel, const int& iYRel)
{
	if(iButton==1)
	{
		m_bLeftButtonDown=false;
	}
	if(iButton==3)
	{
		m_bRightButtonDown=false;
	}

	m_iX=iX;
	m_iY=iY;
	m_iXRel=m_iXRel;
	m_iYRel=m_iYRel;
	m_iButton=iButton;
}

void GLMouse::MouseMoved(const int& iButton, const int& iX, const int& iY, const int& iXRel, const int& iYRel)
{	
	m_iX=iX;
	m_iY=iY;
	m_iXRel=m_iXRel;
	m_iYRel=m_iYRel;

	m_iButton=iButton;
}

GLRay GLMouse::GetRay() const
{
	GLRay ray;

	assert(glIsEnabled(GL_DEPTH_TEST)==GL_TRUE);

	double window_x=static_cast<double>(m_iX);
	double window_y=static_cast<double>(m_iY);

	GLdouble matModelView[16], matProjection[16];
	GLint matViewport[4];

	glGetDoublev(GL_MODELVIEW_MATRIX, matModelView);
	glGetDoublev(GL_PROJECTION_MATRIX, matProjection);
	glGetIntegerv(GL_VIEWPORT, matViewport);

	window_y=matViewport[3] - window_y;
	
	double startx, starty, startz;
	double endx, endy, endz;
	gluUnProject(window_x, window_y, 0.0, matModelView, matProjection, matViewport, &startx, &starty, &startz);
	gluUnProject(window_x, window_y, 1.0, matModelView, matProjection, matViewport, &endx, &endy, &endz);

	ray.origin.x=startx;
	ray.origin.y=starty;
	ray.origin.z=startz;

	ray.direction.x=endx - startx;
	ray.direction.y=endy - starty;
	ray.direction.z=endz - startz;

	ray.direction=ray.direction.normalize();

	return ray;
}

GLVector GLMouse::Coordinate3D(double window_x, double window_y) const
{
	double window_z;

	glReadPixels(static_cast<int>(window_x), static_cast<int>(window_y), 1, 1, GL_DEPTH_COMPONENT, GL_DOUBLE, &window_z);

	GLVector pt3D=Unproject(window_x, window_y, window_z);

	return pt3D;
}

GLVector GLMouse::Unproject(double window_x, double window_y, double window_z) const
{
	assert(glIsEnabled(GL_DEPTH_TEST)==GL_TRUE);

	GLdouble matProjection[16], matModelView[16];
	GLint matViewport[4];

	glGetDoublev(GL_PROJECTION_MATRIX, matProjection);
	glGetDoublev(GL_MODELVIEW_MATRIX, matModelView);
	glGetIntegerv(GL_VIEWPORT, matViewport);

	window_y=matViewport[3] - window_y;

	GLVector pt;
	gluUnProject(window_x, window_y, window_z, matModelView, matProjection, matViewport, &(pt.x), &(pt.y), &(pt.z));

	return pt;
}
