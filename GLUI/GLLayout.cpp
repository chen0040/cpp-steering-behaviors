#include "GLLayout.h"
#include "GLCommandPanel.h"

GLLayout::GLLayout()
: m_pParent(NULL)
{
	
}

GLLayout::~GLLayout()
{

}

void GLLayout::SetParent(GLCommandPanel* pParent)
{
	m_pParent=pParent;
	this->AddObserver(pParent);
}