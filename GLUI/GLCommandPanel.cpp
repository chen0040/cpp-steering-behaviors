#include "GLCommandPanel.h"
#include "GLFlowLayout.h"
#include <iostream>
#include <cassert>

GLCommandPanel::GLCommandPanel(GLLayout* pLayout)
: GLUIObj()
, m_pEventReceiver_MouseButtonDown(NULL)
, m_pEventReceiver_MouseButtonUp(NULL)
, m_pEventReceiver_MouseMoved(NULL)
, m_bShowPanel(false)
{
	if(pLayout==NULL)
	{
		m_pLayout=new GLFlowLayout(GLFlowLayout::HORIZONTAL_ALIGNMENT);
		m_pLayout->SetParent(this);
	}
	else
	{
		m_pLayout=pLayout;
		m_pLayout->SetParent(this);
	}
	SetBoundingRect(0, 640, 0, 480);
}

void GLCommandPanel::SetLayout(GLLayout* pLayout)
{
	assert(!pLayout==NULL);

	if(m_pLayout != NULL)
	{
		delete m_pLayout;
		m_pLayout=NULL;
	}

	m_pLayout=pLayout;
	m_pLayout->SetParent(this);
}

GLCommandPanel::~GLCommandPanel()
{
	if(m_pLayout != NULL)
	{
		delete m_pLayout;
		m_pLayout=NULL;
	}
}

void GLCommandPanel::MouseButtonDown(const int& iButton, const int& iX, const int& iY, const int& iXRel, const int& iYRel)
{
	if(!m_bVisible)
	{
		return;
	}

	m_pEventReceiver_MouseButtonDown=NULL;
	m_pLayout->MouseButtonDown(iButton, iX, iY, iXRel, iYRel);
}

void GLCommandPanel::MouseButtonUp(const int& iButton, const int& iX, const int& iY, const int& iXRel, const int& iYRel)
{
	if(!m_bVisible)
	{
		return;
	}

	m_pEventReceiver_MouseButtonUp=NULL;
	m_pLayout->MouseButtonUp(iButton, iX, iY, iXRel, iYRel);
}
	
void GLCommandPanel::MouseMoved(const int& iButton, const int& iX, const int& iY, const int& iXRel, const int& iYRel)
{
	if(!m_bVisible)
	{
		return;
	}

	m_pEventReceiver_MouseMoved=NULL;
	m_pLayout->MouseMoved(iButton, iX, iY, iXRel, iYRel);
}

void GLCommandPanel::EventNotified_MouseButtonDown(GLUIObj* pSender)
{
	GLUIObj::EventNotified_MouseButtonDown(pSender);

	m_pEventReceiver_MouseButtonDown=pSender;
}

void GLCommandPanel::EventNotified_MouseButtonUp(GLUIObj* pSender)
{
	GLUIObj::EventNotified_MouseButtonDown(pSender);

	m_pEventReceiver_MouseButtonUp=pSender;
}
	
void GLCommandPanel::EventNotified_MouseMoved(GLUIObj* pSender)
{
	GLUIObj::EventNotified_MouseMoved(pSender);

	m_pEventReceiver_MouseMoved=pSender;
}

void GLCommandPanel::AddUIObj(GLUIObj* pObj)
{
	m_pLayout->AddUIObj(pObj);
}

void GLCommandPanel::AddUIObj(int row, int col, GLUIObj* pObj)
{
	m_pLayout->AddUIObj(row, col, pObj);
}

void GLCommandPanel::AddUIObj(GLUIObj* pObj, int position)
{
	m_pLayout->AddUIObj(pObj, position);
}

GLUIObj* GLCommandPanel::GetUIObj(int position) const
{
	return m_pLayout->GetUIObj(position);
}

GLUIObj* GLCommandPanel::GetUIObj(int row, int col) const
{
	return m_pLayout->GetUIObj(row, col);
}

void GLCommandPanel::RemoveUIObj(int position)
{
	m_pLayout->RemoveUIObj(position);
}

void GLCommandPanel::RemoveUIObj(int row, int col)
{
	m_pLayout->RemoveUIObj(row, col);
}

void GLCommandPanel::ClearUIObjs()
{
	m_pLayout->ClearUIObjs();
}

void GLCommandPanel::PreRender()
{
	if(!m_bVisible)
	{
		return;
	}

	GLint matViewport[4];
	glGetIntegerv(GL_VIEWPORT, matViewport);

	double window_width=static_cast<double>(matViewport[2]);
	double window_height=static_cast<double>(matViewport[3]);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, window_width, window_height, 0);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
}

void GLCommandPanel::PostRender()
{
	if(!m_bVisible)
	{
		return;
	}

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void GLCommandPanel::Render()
{
	if(!m_bVisible)
	{
		return;
	}

	if(m_bShowPanel)
	{
		RenderPanel();
	}
	m_pLayout->Render();
}

void GLCommandPanel::RenderPanel()
{
	if(!m_bVisible)
	{
		return;
	}

	GLRect rect=GetInsettedRect();

	glBegin(GL_LINE_LOOP);
	glVertex2d(rect.left, rect.top);
	glVertex2d(rect.right, rect.top);
	glVertex2d(rect.right, rect.bottom);
	glVertex2d(rect.left, rect.bottom);
	glEnd();
}