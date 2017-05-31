#include "GLCommand.h"
#include <sdl.h>
#include "../GLExtension/glxs.h"
#include <cassert>
#include <iostream>
#include <glut.h>
#include "../GLFont/GLFontSys.h"

GLCommand::GLCommand(GLTexture* pCommandTexture, GLTexture* pHoverTexture)
: GLUIObj()
, m_pMouseHoverTexture(pHoverTexture)
, m_pCommandTexture(pCommandTexture)
, m_text_offset(0)
{
	m_inset.top=1;
	m_inset.bottom=1;
	m_inset.left=1;
	m_inset.right=1;

	this->SetSize(100, 35);

	m_pButtonTexture=pCommandTexture;
}

GLCommand::~GLCommand()
{
	std::list<GLActionListener*>::iterator pos_listener;
	for(pos_listener=m_listeners.begin(); pos_listener != m_listeners.end(); ++pos_listener)
	{
		delete *pos_listener;
	}
	m_listeners.clear();
}

void GLCommand::Render()
{
	if(!m_bVisible)
	{
		return;
	}

	assert(m_pMouseHoverTexture != NULL);
	assert(m_pCommandTexture != NULL);

	GLRect rect=GetInsettedRect();

	if(m_text.compare("") != 0)
	{
		glFont.println(m_text, rect.left+m_text_offset, 0.5 * (rect.bottom+rect.top));
	}

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

void GLCommand::MouseMoved(const int& iButton, const int& iX, const int& iY, const int& iXRel, const int& iYRel)
{
	if(! this->IsMouseEventCaptured(iX, iY))
	{
		m_pButtonTexture=m_pCommandTexture;
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

	m_pButtonTexture=m_pMouseHoverTexture;
}

void GLCommand::MouseButtonDown(const int& iButton, const int& iX, const int& iY, const int& iXRel, const int& iYRel)
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
		(*pos_listener)->MouseButtonDown(iButton, iX1, iY1, iXRel, iYRel);
	}

	std::list<GLUIObj*>::iterator pos;
	for(pos=m_obs.begin(); pos!=m_obs.end(); ++pos)
	{
		(*pos)->EventNotified_MouseButtonDown(this);
	}
}

void GLCommand::MouseButtonUp(const int& iButton, const int& iX, const int& iY, const int& iXRel, const int& iYRel)
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

bool GLCommand::IsMouseEventCaptured(const int& iX, const int& iY)
{
	GLRect rect=GetInsettedRect();
	return rect.PtInRect(iX, iY);
}
