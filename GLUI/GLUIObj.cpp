#include "GLUIObj.h"
#include <sdl.h>
#include "../GLExtension/glxs.h"

GLUIObj::GLUIObj()
: m_inset(0, 0, 0, 0)
, m_bVisible(true)
{
	
}

GLUIObj::~GLUIObj()
{
	
}

void GLUIObj::AddObserver(GLUIObj* pObserver)
{
	m_obs.push_back(pObserver);
}

void GLUIObj::EventNotified_MouseButtonDown(GLUIObj* pSender)
{
	std::list<GLUIObj*>::iterator pos;
	for(pos=m_obs.begin(); pos!=m_obs.end(); ++pos)
	{
		(*pos)->EventNotified_MouseButtonDown(pSender);
	}
}

void GLUIObj::EventNotified_MouseButtonUp(GLUIObj* pSender)
{
	std::list<GLUIObj*>::iterator pos;
	for(pos=m_obs.begin(); pos!=m_obs.end(); ++pos)
	{
		(*pos)->EventNotified_MouseButtonUp(pSender);
	}
}

void GLUIObj::EventNotified_MouseMoved(GLUIObj* pSender)
{
	std::list<GLUIObj*>::iterator pos;
	for(pos=m_obs.begin(); pos!=m_obs.end(); ++pos)
	{
		(*pos)->EventNotified_MouseMoved(pSender);
	}
}

void GLUIObj::SetSize(double width, double height)
{
	m_bounding_rectangle.right=m_bounding_rectangle.left + width;
	m_bounding_rectangle.bottom=m_bounding_rectangle.top+height;
	ResetUI();
}

void GLUIObj::SetWidth(double width)
{
	m_bounding_rectangle.right=m_bounding_rectangle.left + width;
	ResetUI();
}

void GLUIObj::SetHeight(double height)
{
	m_bounding_rectangle.bottom=m_bounding_rectangle.top+height;
	ResetUI();
}
