#include "GLBorderLayout.h"
#include "GLCommandPanel.h"
#include <iostream>
#include <cassert>

GLBorderLayout::GLBorderLayout()
: GLLayout()
, m_ui(COUNT, NULL)
, EAST_PROP(0.2)
, WEST_PROP(0.2)
, NORTH_PROP(0.2)
, SOUTH_PROP(0.2)
, m_bStretchHorizontally(false)
{

}

GLBorderLayout::~GLBorderLayout()
{
	ClearUIObjs();
}

void GLBorderLayout::Render()
{
	for(size_t i=0; i != m_ui.size(); i++)
	{
		if(m_ui[i] != NULL)
		{
			m_ui[i]->Render();
		}
	}
}

void GLBorderLayout::ResetUI()
{
	GLRect rect=m_pParent->GetInsettedRect();
	double panel_top=rect.top;
	double panel_left=rect.left;
	double panel_width=rect.Width();
	double panel_height=rect.Height();

	if(m_ui[NORTH] != NULL)
	{
		if(m_bStretchHorizontally)
		{
			m_ui[NORTH]->SetBoundingRect(panel_left, panel_left+panel_width, panel_top, panel_top + panel_height * NORTH_PROP);
		}
		else
		{
			m_ui[NORTH]->SetBoundingRect(panel_left + panel_width * WEST_PROP, panel_left+panel_width * (1 - EAST_PROP), panel_top, panel_top + panel_height * NORTH_PROP);
		}
	}
	if(m_ui[SOUTH] != NULL)
	{
		if(m_bStretchHorizontally)
		{
			m_ui[SOUTH]->SetBoundingRect(panel_left, panel_left + panel_width, panel_top + panel_height *(1-SOUTH_PROP), panel_top+panel_height);
		}
		else
		{
			m_ui[SOUTH]->SetBoundingRect(panel_left + panel_width * WEST_PROP, panel_left + panel_width * (1-EAST_PROP), panel_top + panel_height *(1-SOUTH_PROP), panel_top+panel_height);
		}
	}
	if(m_ui[CENTER] != NULL)
	{
		m_ui[CENTER]->SetBoundingRect(panel_left + panel_width * (WEST_PROP), panel_left + panel_width * (1- EAST_PROP), panel_top + panel_height * NORTH_PROP, panel_top + panel_height * ( 1- SOUTH_PROP));
	}
	if(m_ui[WEST] != NULL)
	{
		if(m_bStretchHorizontally)
		{
			m_ui[WEST]->SetBoundingRect(panel_left, panel_left + panel_width * (WEST_PROP), panel_top + panel_height * NORTH_PROP, panel_top + panel_height * (1 - SOUTH_PROP));
		}
		else
		{
			m_ui[WEST]->SetBoundingRect(panel_left, panel_left + panel_width * (WEST_PROP), panel_top, panel_top + panel_height);
		}
	}
	if(m_ui[EAST] != NULL)
	{
		if(m_bStretchHorizontally)
		{
			m_ui[EAST]->SetBoundingRect(panel_left + panel_width * (1-EAST_PROP), panel_left + panel_width, panel_top + panel_height * NORTH_PROP, panel_top + panel_height * (1 - SOUTH_PROP));
		}
		else
		{
			m_ui[EAST]->SetBoundingRect(panel_left + panel_width * (1-EAST_PROP), panel_left + panel_width, panel_top, panel_top + panel_height);
		}
	}
}

void GLBorderLayout::AddUIObj(GLUIObj* pObj, int position)
{
	assert(m_ui[position]==NULL);

	m_ui[position]=pObj;
	pObj->AddObserver(this);

	ResetUI();
}

void GLBorderLayout::RemoveUIObj(int position)
{
	if(m_ui[position] != NULL)
	{
		delete m_ui[position];
		m_ui[position]=NULL;
	}

	ResetUI();
}

void GLBorderLayout::ClearUIObjs()
{
	for(size_t i=0; i!=m_ui.size(); i++)
	{
		if(m_ui[i] != NULL)
		{
			delete m_ui[i];
			m_ui[i]=NULL;
		}
	}
	m_ui.clear();
}

GLUIObj* GLBorderLayout::GetUIObj(int position) const
{
	return m_ui[position];
}

void GLBorderLayout::MouseButtonDown(const int &iButton, const int &iX, const int &iY, const int &iXRel, const int &iYRel)
{
	for(size_t i=0; i != m_ui.size(); i++)
	{
		if(m_ui[i]!=NULL)
		{
			m_ui[i]->MouseButtonDown(iButton, iX, iY, iXRel, iYRel);
		}
	}
}

void GLBorderLayout::MouseButtonUp(const int &iButton, const int &iX, const int &iY, const int &iXRel, const int &iYRel)
{
	for(size_t i=0; i != m_ui.size(); i++)
	{
		if(m_ui[i]!=NULL)
		{
			m_ui[i]->MouseButtonUp(iButton, iX, iY, iXRel, iYRel);
		}
	}
}

void GLBorderLayout::MouseMoved(const int &iButton, const int &iX, const int &iY, const int &iXRel, const int &iYRel)
{
	for(size_t i=0; i != m_ui.size(); i++)
	{
		if(m_ui[i]!=NULL)
		{
			m_ui[i]->MouseMoved(iButton, iX, iY, iXRel, iYRel);
		}
	}
}
