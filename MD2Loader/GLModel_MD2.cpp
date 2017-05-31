#include "GLModel_MD2.h" 
#include "../GLUtil/GLExitSystem.h"
#include <sstream>

GLModel_MD2::GLModel_MD2(const char* fname)
: GLModel(fname)
{
	LoadModel(fname);
}

GLModel_MD2::~GLModel_MD2()
{
	glDeleteTextures(32, m_tex);
}

void GLModel_MD2::Render(int iFrame)
{
	m_loader.Draw(iFrame);
}

bool GLModel_MD2::LoadModel(const char* fname)
{
	m_loader.Load(fname);

	glEnable(GL_TEXTURE_2D);
	glGenTextures(32, m_tex);

	if(strcmp(m_loader.GetTexName(), "") !=0)
	{
		if(!LoadTexture(m_loader.GetTexName(), m_tex[0]))
		{
			std::ostringstream oss;
			oss << "Failed to load texture " << m_loader.GetTexName();
			GLExitSystem("GLModel_MD2::LoadModel(const char* fname)", oss.str().c_str());
			return false;
		}
		m_loader.SetTexture(m_tex[0]);
		m_bTexture2DEnabled=true;
	}
	else if(!m_texture_list.empty())
	{
		std::string texture_name=m_texture_list.front();
		
		if(!LoadTexture(texture_name.c_str(), m_tex[0]))
		{
			std::ostringstream oss;
			oss << "Failed to load texture " << texture_name;
			GLExitSystem("GLModel_MD2::LoadModel(const char* fname)", oss.str().c_str());
			return false;
		}

		m_loader.SetTexture(m_tex[0]);
		m_bTexture2DEnabled=true;
	}

	glDisable(GL_TEXTURE_2D);

	m_iFrameCount=m_loader.GetFrameCount();

	return true;
}

