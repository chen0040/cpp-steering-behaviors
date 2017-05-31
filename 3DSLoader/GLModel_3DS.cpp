#include "GLModel_3DS.h" 
#include "../GLUtil/GLExitSystem.h"
#include <sstream>
#include <io.h>
#include <iostream>

GLModel_3DS::GLModel_3DS(const char* fname)
: GLModel(fname)
, m_loader(NULL)
{
	LoadModel(fname);
}


GLModel_3DS::~GLModel_3DS()
{
	if(m_loader != NULL)
	{
		delete m_loader;
	}

	if(!m_texture_list.empty())
	{
		glDeleteTextures(32, m_tex);
	}
}

void GLModel_3DS::Render(int iFrame)
{
	glFrontFace(GL_CCW);
	if(!m_texture_list.empty())
	{			
		if(m_texture_list.size()==1)
		{
			glBindTexture(GL_TEXTURE_2D, m_tex[0]);

			//Xianshun says:
			// GL_Blend is required if the textures contains alpha component
			if(m_bAlphaBlend) 
			{
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			}
			m_loader->Draw();
			if(m_bAlphaBlend) glDisable(GL_BLEND);

			glBindTexture(GL_TEXTURE_2D, 0);
		}
		else
		{
			/*
			glActiveTextureARB(GL_TEXTURE0_ARB);
			glBindTexture(GL_TEXTURE_2D, m_tex[0]);
			glEnable(GL_TEXTURE_2D);
			glActiveTextureARB(GL_TEXTURE1_ARB);
			glBindTexture(GL_TEXTURE_2D, m_tex[1]);
			glEnable(GL_TEXTURE_2D);
			*/

			glBindTexture(GL_TEXTURE_2D, m_tex[0]);

			if(m_bAlphaBlend)
			{
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			}
			m_loader->Draw();
			if(m_bAlphaBlend) glDisable(GL_BLEND);

			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}
	else
	{
		m_loader->Draw();
	}

	glFrontFace(GL_CW);
}

bool GLModel_3DS::LoadModel(const char* fname)
{
	if(m_loader != NULL)
	{
		delete m_loader;
	}

	if(_access(fname, 0)==-1)
	{
		std::ostringstream oss;
		oss << "model file " << fname << " does not exist";
		GLExitSystem("GLModel_3DS::LoadModel(const char* fname)", oss.str().c_str());
	}
	//std::cout << "start loading: " << fname << std::endl;
	m_loader=new CModel3DS(fname);
	//std::cout << "end loading: " << fname << std::endl;

	if(!m_texture_list.empty())
	{
		glEnable(GL_TEXTURE_2D);
		glGenTextures(32, m_tex);
		for(size_t k=0; k<m_texture_list.size(); k++)
		{
			std::string texture_name=m_texture_list[k];
			if(!LoadTexture(texture_name.c_str(), m_tex[k]))
			{
				std::ostringstream oss;
				oss << "Failed to load texture " << texture_name;
				GLExitSystem("GLModel_3DS::LoadModel(const char* fname)", oss.str().c_str());
				return false;
			}
		}
		m_bTexture2DEnabled=true;
	}

	return true;
}

