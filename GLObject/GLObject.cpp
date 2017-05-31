#include "GLObject.h"

GLObject::GLObject()
#ifdef GL_OBJECT_STORE_GL_STATES
: m_GL_LIGHTING(GL_FALSE)
, m_GL_DEPTH_TEST(GL_FALSE)
, m_GL_FOG(GL_FALSE)
, m_GL_TEXTURE_2D(GL_FALSE)
, m_GL_COLOR_MATERIAL(GL_FALSE)
#endif
{
}

GLObject::~GLObject()
{

}

#ifdef GL_OBJECT_STORE_GL_STATES
void GLObject::PreRender()
{
	glGetBooleanv(GL_LIGHTING, &m_GL_LIGHTING);
	glGetBooleanv(GL_DEPTH_TEST, &m_GL_DEPTH_TEST);
	glGetBooleanv(GL_FOG, &m_GL_FOG);

	glGetBooleanv(GL_COLOR_MATERIAL, &m_GL_COLOR_MATERIAL);
	glGetBooleanv(GL_TEXTURE_2D, &m_GL_TEXTURE_2D);
}
#endif

#ifdef GL_OBJECT_STORE_GL_STATES
void GLObject::PostRender()
{
	if(m_GL_LIGHTING==GL_TRUE)
	{
		glEnable(GL_LIGHTING);
	}
	else
	{
		glDisable(GL_LIGHTING);
	}
	if(m_GL_DEPTH_TEST==GL_TRUE)
	{
		glEnable(GL_DEPTH_TEST);
	}
	else
	{
		glDisable(GL_DEPTH_TEST);
	}
	if(m_GL_FOG==GL_TRUE)
	{
		glEnable(GL_FOG);
	}
	else
	{
		glDisable(GL_FOG);
	}
	if(m_GL_TEXTURE_2D==GL_TRUE)
	{
		glEnable(GL_TEXTURE_2D);
	}
	else
	{
		glDisable(GL_TEXTURE_2D);
	}
	if(m_GL_COLOR_MATERIAL==GL_TRUE)
	{
		glEnable(GL_COLOR_MATERIAL);
	}
	else
	{
		glDisable(GL_COLOR_MATERIAL);
	}
}
#endif