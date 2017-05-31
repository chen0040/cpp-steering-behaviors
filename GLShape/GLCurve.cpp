#include "GLCurve.h"
#include <glut.h>
#include <iostream>

GLCurve::GLCurve()
: GLObject()
, m_steps(100)
, m_bShowLineSegments(false)
{

}

GLCurve::~GLCurve()
{
}

void GLCurve::Clear()
{
	m_nodes.clear();
	m_points.clear();
	m_distances.clear();
}

void GLCurve::AddPoint(const GLVector& point)
{
	m_points.push_back(point);
	OnNewControlPointAdded();
}

void GLCurve::Render()
{
	if(m_bShowLineSegments)
	{
		RenderLineSegments();
	}

	RenderControlPoints();
	RenderSpline();

}

void GLCurve::AddNode(const GLVector& node)
{
	m_nodes.push_back(node);

	

	if(m_nodes.size()==1)
	{
		m_distances.push_back(0);
	}
	else
	{
		int new_node_index=m_nodes.size() - 1;

		double segment_distance=(m_nodes[new_node_index] - m_nodes[new_node_index-1]).length();
		m_distances.push_back(segment_distance + m_distances[new_node_index-1]);
	}
}

void GLCurve::RenderLineSegments()
{
	glLineWidth(3);

	glBegin(GL_LINE_STRIP);
	for(size_t i=0;  i<m_points.size(); i++)
	{
		glVertex3d(m_points[i].x, m_points[i].y, m_points[i].z);
	}
	glEnd();
}

void GLCurve::RenderControlPoints()
{
	glEnable(GL_BLEND);
	glEnable(GL_COLOR_MATERIAL);
	glColor4f(1.0f, 0.0f, 1.0f, 0.7f);
	for(size_t i=0; i<m_points.size(); i++)
	{
		glPushMatrix();
		glTranslated(m_points[i].x, m_points[i].y, m_points[i].z);
		glutSolidSphere(0.5, 20, 20);
		glPopMatrix();
	}
	glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_BLEND);
}

void GLCurve::RenderSpline()
{
	glColor3f(0.0f, 1.0f, 0.0f);
	glBegin(GL_LINE_STRIP);

	for(size_t i=0; i<m_nodes.size(); i++)
	{
		glVertex3d(m_nodes[i].x, m_nodes[i].y, m_nodes[i].z);
	}
	glEnd();

	glColor3f(1, 1, 1);
}

void GLCurve::PreRender()
{
#ifdef GL_OBJECT_STORE_GL_STATES
	GLObject::PreRender();
#endif

	glDisable(GL_LIGHTING);
	glPushMatrix();
}

void GLCurve::PostRender()
{
	glPopMatrix();
	glEnable(GL_LIGHTING);

#ifdef GL_OBJECT_STORE_GL_STATES
	GLObject::PostRender();
#endif
}