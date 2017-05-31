#include "GLFlowLayout.h"
#include "GLCommandPanel.h"
#include <iostream>

GLFlowLayout::GLFlowLayout(int alignment)
: GLLayout()
, m_alignment(alignment)
{

}

GLFlowLayout::~GLFlowLayout()
{
	ClearUIObjs();
}

void GLFlowLayout::Render()
{
	for(size_t i=0; i != m_ui.size(); i++)
	{
		m_ui[i]->Render();
	}
}

void GLFlowLayout::ResetUI()
{
	GLRect rect=m_pParent->GetInsettedRect();
	double panel_top=rect.top;
	double panel_left=rect.left;
	double panel_width=rect.Width();
	double panel_height=rect.Height();

	double current_left=panel_left;
	double current_top=panel_top;
	double current_width=0;
	double current_height=0;

	for(size_t i=0; i != m_ui.size(); i++)
	{
		double width=m_ui[i]->GetBoundingRect().Width();
		double height=m_ui[i]->GetBoundingRect().Height();

		m_ui[i]->SetBoundingRect(current_left, current_left+width, current_top, current_top+height);

		if(m_alignment==HORIZONTAL_ALIGNMENT)
		{
			current_left+=width;
			if(current_height < height)
			{
				current_height=height;
			}
			if(current_left+width > panel_width+panel_left)
			{
				current_left=panel_left;
				current_top+=current_height;
				current_height=0;
			}
		}
		else
		{
			current_top+=height;
			if(current_width < width)
			{
				current_width=width; 
			}
			if(current_top + height > panel_height + panel_top)
			{
				current_top=panel_top;
				current_left+=current_width;
				current_width=0;
			}
		}
	}
}

void GLFlowLayout::ClearUIObjs()
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

void GLFlowLayout::AddUIObj(GLUIObj* pObj)
{
	m_ui.push_back(pObj);
	pObj->AddObserver(this);

	ResetUI();
}

void GLFlowLayout::MouseButtonDown(const int &iButton, const int &iX, const int &iY, const int &iXRel, const int &iYRel)
{
	for(size_t i=0; i != m_ui.size(); i++)
	{
		m_ui[i]->MouseButtonDown(iButton, iX, iY, iXRel, iYRel);
	}
}

void GLFlowLayout::MouseButtonUp(const int &iButton, const int &iX, const int &iY, const int &iXRel, const int &iYRel)
{
	for(size_t i=0; i != m_ui.size(); i++)
	{
		m_ui[i]->MouseButtonUp(iButton, iX, iY, iXRel, iYRel);
	}
}

void GLFlowLayout::MouseMoved(const int &iButton, const int &iX, const int &iY, const int &iXRel, const int &iYRel)
{
	for(size_t i=0; i != m_ui.size(); i++)
	{
		m_ui[i]->MouseMoved(iButton, iX, iY, iXRel, iYRel);
	}
}
