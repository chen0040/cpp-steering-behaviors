#include "GLSphericalCamera.h"
#include "../GLMath/GLMath.h"
#include <cmath>
#include <iostream>
#include "../GLExtension/glxs.h"

GLSphericalCamera::GLSphericalCamera()
: GLCamera()
, m_min_radius(5)
, m_radius(20)
{
	m_max_beta=glMath.PI() * 2.0 - glMath.Epsilon();
	m_max_theta=glMath.PI() / 2.0 - glMath.Epsilon();

	m_min_theta=glMath.Epsilon();
	m_min_beta=glMath.Epsilon();

	m_theta=glMath.PI() / 2.0 * 0.5;
	m_beta=glMath.PI() / 2.0 * 0.5;

	calculate_vectors();
}

GLSphericalCamera::~GLSphericalCamera()
{

}

void GLSphericalCamera::calculate_vectors()
{
	m_eye.y=m_radius * sin(m_theta);
	
	m_eye.x=m_radius * cos(m_theta) * cos(m_beta);
	m_eye.z=m_radius * cos(m_theta) * sin(m_beta);

	
	GLVector v=m_focus - m_eye;

	GLVector u(0, 1, 0);
	GLVector w=v.cross(u);
	m_up_direction=w.cross(v);

	m_up_direction=m_up_direction.normalize();

	m_right_direction=w.normalize();
	m_look_direction=v.normalize();
}

void GLSphericalCamera::PreTransform()
{
	glPushMatrix();
	glLoadIdentity();
}

void GLSphericalCamera::Transform()
{
	gluLookAt(m_eye.x, m_eye.y, m_eye.z, m_focus.x, m_focus.y, m_focus.z, m_up_direction.x, m_up_direction.y, m_up_direction.z);
}

void GLSphericalCamera::PostTransform()
{
	glPopMatrix();
}

void GLSphericalCamera::Scroll(const GLVector& delta)
{
	if(m_bLocked)
	{
		return;
	}

	GLVector f=m_focus;
	f+=delta;
	f.y=0;
	if(f.x >= m_min_focus.x && f.z >= m_min_focus.z && f.z <= m_max_focus.z && f.x <= m_max_focus.x)
	{
		m_focus=f;
	}
	calculate_vectors();
}

void GLSphericalCamera::Reset()
{
	if(m_bLocked)
	{
		return;
	}

	m_theta=glMath.PI() / 2.0 * 0.5;
	m_beta=glMath.PI() / 2.0 * 0.5;

	m_radius=20;

	m_focus=GLVector(0.0, 0.0, 0.0);

	calculate_vectors();
}

void GLSphericalCamera::SetFocus(const GLVector& focus)
{
	if(m_bLocked)
	{
		return;
	}

	SetCenter(focus);
	calculate_vectors();
}

void GLSphericalCamera::SetCameraCenterConstraints(const GLVector& min, const GLVector& max)
{
	m_min_focus=min;
	m_max_focus=max;
}

void GLSphericalCamera::IncrementBeta(double db)
{
	if(m_bLocked)
	{
		return;
	}

	m_beta+=db;
	if(m_beta > m_max_beta)
	{
		m_beta -= (m_max_beta - m_min_beta);
	}
	else if(m_beta < m_min_beta)
	{
		m_beta += (m_max_beta - m_min_beta);
	}
	calculate_vectors();
}

void GLSphericalCamera::IncrementRadius(double dr)
{
	if(m_bLocked)
	{
		return;
	}

	m_radius+=dr;
	if(m_radius < m_min_radius)
	{
		m_radius=m_min_radius;
	}
	calculate_vectors();
}

void GLSphericalCamera::IncrementTheta(double dt)
{
	if(m_bLocked)
	{
		return;
	}
	m_theta+=dt;

	if(m_theta >= m_max_theta)
	{
		m_theta = m_max_theta - glMath.Epsilon();
	}
	else if(m_theta <= m_min_theta)
	{
		m_theta = m_min_theta + glMath.Epsilon();
	}
	calculate_vectors();
}
