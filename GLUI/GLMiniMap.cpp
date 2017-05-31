#include "GLMiniMap.h"
#include <sdl.h>
#include "../GLExtension/glxs.h"
#include <cassert>
#include <iostream>
#include <glut.h>
#include "../GLCamera/GLSphericalCamera.h"
#include "../GameWorld/GameWorld.h"

GLMiniMap::GLMiniMap()
: GLUIObj()
, m_pCamera(NULL)
, m_pWorld(NULL)
, m_alpha(0.1f)
{
	m_inset.top=1;
	m_inset.bottom=1;
	m_inset.left=1;
	m_inset.right=1;	
}

GLMiniMap::~GLMiniMap()
{
	
}

void GLMiniMap::Render()
{
	GLRect rect=GetInsettedRect();
	double dWidth1=rect.Width();
	double dHeight1=rect.Height();
	double dWidth2=m_pWorld->GetEnvWidth();
	double dLength2=m_pWorld->GetEnvLength();

	glDisable(GL_LIGHTING);

	glBegin(GL_LINE_LOOP);
	glVertex2d(rect.left, rect.top);
	glVertex2d(rect.right, rect.top);
	glVertex2d(rect.right, rect.bottom);
	glVertex2d(rect.left, rect.bottom);
	glEnd();

	const AgentGroup& agents=m_pWorld->GetConstAgents();
	AgentGroup::const_iterator pos_agent;
	for(pos_agent=agents.begin(); pos_agent != agents.end(); ++pos_agent)
	{
		double dX2=(*pos_agent)->get_position().x + dWidth2 / 2;
		double dZ2=(*pos_agent)->get_position().z + dLength2 / 2;
		double dX1=dX2 / dWidth2 * dWidth1 + rect.left;
		double dY1=dZ2 / dLength2 * dHeight1 + rect.top;
		
		glPushMatrix();
		glTranslated(dX1, dY1, 0);
		(*pos_agent)->RenderDot();
		glPopMatrix();
	}

	const ObstacleGroup& obs=m_pWorld->GetConstObstacles();
	ObstacleGroup::const_iterator pos_obs;
	for(pos_obs=obs.begin(); pos_obs != obs.end(); ++pos_obs)
	{
		double dX2=(*pos_obs)->get_position().x + dWidth2 / 2;
		double dZ2=(*pos_obs)->get_position().z + dLength2 / 2;
		double dX1=dX2 / dWidth2 * dWidth1 + rect.left;
		double dY1=dZ2 / dLength2 * dHeight1 + rect.top;
	
		glPushMatrix();
		glTranslated(dX1, dY1, 0);
		(*pos_obs)->RenderDot();
		glPopMatrix();
	}

	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glColor4f(1, 1, 1, m_alpha);
	glBegin(GL_QUADS);
	glVertex2d(rect.left, rect.top);
	glVertex2d(rect.right, rect.top);
	glVertex2d(rect.right, rect.bottom);
	glVertex2d(rect.left, rect.bottom);
	glEnd();
	glDisable(GL_BLEND);


	/*
	glBegin(GL_LINE_LOOP);
	glVertex2d(rect.left, rect.top);
	glVertex2d(rect.right, rect.top);
	glVertex2d(rect.right, rect.bottom);
	glVertex2d(rect.left, rect.bottom);
	glEnd();
	*/

	glEnable(GL_LIGHTING);
}

void GLMiniMap::MouseMoved(const int& iButton, const int& iX, const int& iY, const int& iXRel, const int& iYRel)
{
	if(! this->IsMouseEventCaptured(iX, iY))
	{
		m_alpha=0.1f;
		return;
	}

	GLRect rect=GetInsettedRect();
	int iX1=iX - rect.left;
	int iY1=iY - rect.top;

	m_alpha=0.3f;

	std::list<GLActionListener*>::iterator pos_listener;
	for(pos_listener=m_listeners.begin(); pos_listener != m_listeners.end(); ++pos_listener)
	{
		(*pos_listener)->MouseMoved(iButton, iX1, iY1, iXRel, iYRel);
	}

	std::list<GLUIObj*>::iterator pos;
	for(pos=m_obs.begin(); pos!=m_obs.end(); ++pos)
	{
		(*pos)->EventNotified_MouseMoved(this);
	}
}


void GLMiniMap::MouseButtonDown(const int& iButton, const int& iX, const int& iY, const int& iXRel, const int& iYRel)
{
	if(! this->IsMouseEventCaptured(iX, iY))
	{
		return;
	}

	GLRect rect=GetInsettedRect();
	int iX1=iX-rect.left;
	int iY1=iY - rect.top;
	double dWidth1=rect.Width();
	double dHeight1=rect.Height();

	double dWidth2=m_pWorld->GetEnvWidth();
	double dLength2=m_pWorld->GetEnvLength();

	double dX2=dWidth2 * iX1 / dWidth1 - dWidth2 / 2;
	double dZ2=dLength2 * iY1 / dHeight1 - dLength2 / 2;

	//m_pCamera->SetRadius(20);
	m_pCamera->SetFocus(GLVector(dX2, 0, dZ2));
	

	std::list<GLActionListener*>::iterator pos_listener;
	for(pos_listener=m_listeners.begin(); pos_listener != m_listeners.end(); ++pos_listener)
	{
		(*pos_listener)->MouseButtonDown(iButton, iX1, iY1, iXRel, iYRel);
	}

	std::list<GLUIObj*>::iterator pos;
	for(pos=m_obs.begin(); pos!=m_obs.end(); ++pos)
	{
		(*pos)->EventNotified_MouseButtonDown(this);
	}
}

void GLMiniMap::MouseButtonUp(const int& iButton, const int& iX, const int& iY, const int& iXRel, const int& iYRel)
{
	if(! this->IsMouseEventCaptured(iX, iY))
	{
		return;
	}

	GLRect rect=GetInsettedRect();
	int iX1=iX - rect.left;
	int iY1=iY - rect.top;

	std::list<GLActionListener*>::iterator pos_listener;
	for(pos_listener=m_listeners.begin(); pos_listener != m_listeners.end(); ++pos_listener)
	{
		(*pos_listener)->MouseButtonUp(iButton, iX1, iY1, iXRel, iYRel);
	}

	std::list<GLUIObj*>::iterator pos;
	for(pos=m_obs.begin(); pos!=m_obs.end(); ++pos)
	{
		(*pos)->EventNotified_MouseButtonUp(this);
	}
}

bool GLMiniMap::IsMouseEventCaptured(const int& iX, const int& iY)
{
	GLRect rect=GetInsettedRect();
	return rect.PtInRect(iX, iY);
}