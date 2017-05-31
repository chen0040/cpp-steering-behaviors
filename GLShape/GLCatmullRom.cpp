#include "GLCatmullRom.h"
#include <glut.h>
#include <iostream>

GLCatmullRom::GLCatmullRom()
: GLCurve()
{

}

GLCatmullRom::~GLCatmullRom()
{
}

void GLCatmullRom::OnNewControlPointAdded()
{
	if(m_points.size() < 4)
	{
		return;
	}

	int new_control_point_index=m_points.size() - 1;
	int pt=new_control_point_index - 2;
	for(int i=0; i<=m_steps; i++)
	{
		double u=(double)i / (double)m_steps;

		AddNode(GetPoint(u, m_points[pt-1], m_points[pt], m_points[pt+1], m_points[pt+2]));
	}
}

GLVector GLCatmullRom::GetPoint(double u, const GLVector& P0, const GLVector& P1, const GLVector& P2, const GLVector& P3)
{
	GLVector point;
	point=u*u*u*((-1) * P0 + 3 * P1 - 3 * P2 + P3) / 2;
	point+=u*u*(2*P0 - 5 * P1+ 4 * P2 - P3) / 2;
	point+=u*((-1) * P0 + P2) / 2;
	point+=P1;

	return point;
}

