#include "GLGridLayout.h"
#include "GLCommandPanel.h"
#include <cassert>

GLGridLayout::GLGridLayout(int rows, int cols)
: GLLayout()
{
	m_rows=rows;
	m_cols=cols;
	m_ui.resize(rows*cols, NULL);
}

GLGridLayout::~GLGridLayout()
{
	ClearUIObjs();
}

void GLGridLayout::Render()
{
	for(size_t i=0; i != m_ui.size(); i++)
	{
		if(m_ui[i] != NULL)
		{
			m_ui[i]->Render();
		}
	}
}

void GLGridLayout::ResetUI()
{
	GLRect rect=m_pParent->GetInsettedRect();
	double panel_top=rect.top;
	double panel_left=rect.left;
	double panel_width=rect.Width();
	double panel_height=rect.Height();

	double current_left=panel_left;
	double current_top=panel_top;
	double width=panel_width / m_cols;
	double height=panel_height / m_rows;
	for(int row=0; row < m_rows; row++)
	{
		current_top=row * height;
		for(int col=0; col < m_cols; col++)
		{
			current_left = col * width;
			GLUIObj* pUI=m_ui[row * m_cols + col];
			if(pUI==NULL)
			{
				continue;
			}
			pUI->SetBoundingRect(current_left, current_left+width, current_top, current_top+height);
		}
	}
}

void GLGridLayout::AddUIObj(int row, int col, GLUIObj* pObj)
{
	assert(row*m_cols + col < m_rows * m_cols);
	assert(m_ui[row* m_cols + col] == NULL);

	m_ui[row * m_cols + col]=pObj;
	pObj->AddObserver(this);

	ResetUI();
}

void GLGridLayout::RemoveUIObj(int row, int col)
{
	assert(row*m_cols + col < m_rows * m_cols);
	if(m_ui[row* m_cols + col] != NULL)
	{
		delete m_ui[row * m_cols + col];
		m_ui[row * m_cols + col]=NULL;
	}

	ResetUI();
}

void GLGridLayout::ClearUIObjs()
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

GLUIObj* GLGridLayout::GetUIObj(int row, int col) const
{
	assert(row*m_cols + col < m_rows * m_cols);
	return m_ui[row* m_cols + col];
}

void GLGridLayout::MouseButtonDown(const int &iButton, const int &iX, const int &iY, const int &iXRel, const int &iYRel)
{
	for(size_t i=0; i != m_ui.size(); i++)
	{
		if(m_ui[i] != NULL)
		{
			m_ui[i]->MouseButtonDown(iButton, iX, iY, iXRel, iYRel);
		}
	}
}

void GLGridLayout::MouseButtonUp(const int &iButton, const int &iX, const int &iY, const int &iXRel, const int &iYRel)
{
	for(size_t i=0; i != m_ui.size(); i++)
	{
		if(m_ui[i] != NULL)
		{
			m_ui[i]->MouseButtonUp(iButton, iX, iY, iXRel, iYRel);
		}
	}
}

void GLGridLayout::MouseMoved(const int &iButton, const int &iX, const int &iY, const int &iXRel, const int &iYRel)
{
	for(size_t i=0; i != m_ui.size(); i++)
	{
		if(m_ui[i] != NULL)
		{
			m_ui[i]->MouseMoved(iButton, iX, iY, iXRel, iYRel);
		}
	}
}
