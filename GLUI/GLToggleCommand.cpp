#include "GLToggleCommand.h"
#include <sdl.h>
#include "../GLExtension/glxs.h"
#include <cassert>
#include <iostream>
#include <glut.h>

GLToggleCommand::GLToggleCommand(GLTexture* pCommandTextureOn, GLTexture* pHoverTextureOn, GLTexture* pCommandTextureOff, GLTexture* pHoverTextureOff)
: GLUIObj()
, m_pMouseHoverTextureOn(pHoverTextureOn)
, m_pCommandTextureOn(pCommandTextureOn)
, m_pMouseHoverTextureOff(pHoverTextureOff)
, m_pCommandTextureOff(pCommandTextureOff)
, m_bOn(false)
{
	m_inset.top=1;
	m_inset.bottom=1;
	m_inset.left=1;
	m_inset.right=1;

	this->SetSize(100, 35);

	if(m_bOn)
	{
		m_pButtonTexture=pCommandTextureOn;
	}
	else
	{
		m_pButtonTexture=pCommandTextureOff;
	}
}

GLToggleCommand::~GLToggleCommand()
{
	std::list<GLActionListener*>::iterator pos_listener;
	for(pos_listener=m_listeners.begin(); pos_listener != m_listeners.end(); ++pos_listener)
	{
		delete *pos_listener;
	}
	m_listeners.clear();
}

void GLToggleCommand::Render()
{
	if(!m_bVisible)
	{
		return;
	}

	assert(m_pMouseHoverTextureOn != NULL);
	assert(m_pCommandTextureOn != NULL);
	assert(m_pMouseHoverTextureOff != NULL);
	assert(m_pCommandTextureOff != NULL);

	GLRect rect=GetInsettedRect();

	glDisable(GL_LIGHTING);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(1.0f, 1.0f, 1.0f, 0.5f);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_pButtonTexture->GetTextureId());

	glBegin(GL_QUADS);
	glTexCoord2i(0, 0);
	glVertex2d(rect.left, rect.top);
	glTexCoord2i(1, 0);
	glVertex2d(rect.right, rect.top);
	glTexCoord2i(1, 1);
	glVertex2d(rect.right, rect.bottom);
	glTexCoord2i(0, 1);
	glVertex2d(rect.left, rect.bottom);
	glEnd();

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glEnable(GL_LIGHTING);
}

void GLToggleCommand::MouseMoved(const int& iButton, const int& iX, const int& iY, const int& iXRel, const int& iYRel)
{
	if(! this->IsMouseEventCaptured(iX, iY))
	{
		if(m_bOn)
		{
			m_pButtonTexture=m_pCommandTextureOn;
		}
		else
		{
			m_pButtonTexture=m_pCommandTextureOff;
		}
		return;
	}

	GLRect rect=GetInsettedRect();
	int iX1=iX - rect.left;
	int iY1=iY - rect.top;

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

	if(m_bOn)
	{
		m_pButtonTexture=m_pMouseHoverTextureOn;
	}
	else
	{
		m_pButtonTexture=m_pMouseHoverTextureOff;
	}
}

void GLToggleCommand::MouseButtonDown(const int& iButton, const int& iX, const int& iY, const int& iXRel, const int& iYRel)
{
	if(! this->IsMouseEventCaptured(iX, iY))
	{
		return;
	}

	GLRect rect=GetInsettedRect();
	int iX1=iX - rect.left;
	int iY1=iY - rect.top;

	m_bOn=!m_bOn;

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

void GLToggleCommand::MouseButtonUp(const int& iButton, const int& iX, const int& iY, const int& iXRel, const int& iYRel)
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

bool GLToggleCommand::IsMouseEventCaptured(const int& iX, const int& iY)
{
	GLRect rect=GetInsettedRect();
	return rect.PtInRect(iX, iY);
}
