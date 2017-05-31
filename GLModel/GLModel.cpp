#include "GLModel.h" 
#include "../GLUtil/GLExitSystem.h"
#include <sdl_image.h>
#include "../GLMath/GLMath.h"
#include <sstream>
#include "../tinyxml/tinyxml.h"
#include "../GLTransformation/GLScaling.h"
#include "../GLTransformation/GLRotation.h"
#include "../GLTransformation/GLTranslation.h"
#include <iostream>

GLModel::GLModel(const char* fname)
: m_iFrameCount(0)
, m_bSoundEnabled(false)
, m_bTexture2DEnabled(false)
, m_bAlphaBlend(false)
, m_Height2WidthRatio(1)
{
	m_model_name=fname;
	LoadConfiguration(fname);
}

GLModel::~GLModel()
{
	clear_transformations();
}

void GLModel::clear_transformations()
{
	std::list<GLTransformation*>::iterator pos;
	for(pos=m_transformations.begin(); pos!=m_transformations.end(); ++pos)
	{
		delete *pos;
	}
	m_transformations.clear();
}


void GLModel::add_transformation(GLTransformation* pTransformation)
{
	m_transformations.push_back(pTransformation);
}

void GLModel::PlaySound(int iFrame)
{
	if(!m_bSoundEnabled)
	{
		return;
	}

}

void GLModel::PreRender()
{
#ifdef GL_OBJECT_STORE_GL_STATES
	GLObject::PreRender();
#endif
	glEnable(GL_NORMALIZE);

	if(m_bTexture2DEnabled)
	{
		glEnable(GL_TEXTURE_2D);
	}

	glPushMatrix();

	std::list<GLTransformation*>::iterator pos;
	for(pos=m_transformations.begin(); pos!=m_transformations.end(); ++pos)
	{
		(*pos)->apply();
	}
}

void GLModel::PostRender()
{
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);

	glDisable(GL_NORMALIZE);

#ifdef GL_OBJECT_STORE_GL_STATES
	GLObject::PostRender();
#endif
}

bool GLModel::LoadConfiguration(const char* fname)
{
	RemoveTextures();
	m_bSoundEnabled=false;

	std::ostringstream oss;
	oss << fname << ".xml";
	std::string model_configuration_file=oss.str();

	TiXmlDocument doc;

	if(!doc.LoadFile(model_configuration_file.c_str()))
	{
		std::ostringstream oss;
		oss << "Failed to load xml file " << fname << " for the model " << this->m_model_name;
		GLExitSystem("GLModel::LoadConfiguration(const char* fname)", oss.str().c_str());
		return false;
	}

	TiXmlElement* pXmlRoot=doc.RootElement();
	for(TiXmlElement* pXmlElement1=pXmlRoot->FirstChildElement(); pXmlElement1 != NULL; pXmlElement1=pXmlElement1->NextSiblingElement())
	{
		if(strcmp(pXmlElement1->Value(), "transformations")==0)
		{
			for(TiXmlElement* pXmlElement2=pXmlElement1->FirstChildElement(); pXmlElement2 != NULL; pXmlElement2=pXmlElement2->NextSiblingElement())
			{
				if(strcmp(pXmlElement2->Value(), "rotation")==0)
				{
					double angle_in_degree, x, y, z;
					pXmlElement2->QueryDoubleAttribute("angle_in_degree", &angle_in_degree);
					pXmlElement2->QueryDoubleAttribute("x", &x);
					pXmlElement2->QueryDoubleAttribute("y", &y);
					pXmlElement2->QueryDoubleAttribute("z", &z);
					this->add_transformation(new GLRotation(angle_in_degree, x, y, z));
				}
				else if(strcmp(pXmlElement2->Value(), "scale")==0)
				{
					double x, y, z;
					pXmlElement2->QueryDoubleAttribute("x", &x);
					pXmlElement2->QueryDoubleAttribute("y", &y);
					pXmlElement2->QueryDoubleAttribute("z", &z);
					this->add_transformation(new GLScaling(x, y, z));
				}
				else if(strcmp(pXmlElement2->Value(), "translate")==0)
				{
					double x, y, z;
					pXmlElement2->QueryDoubleAttribute("x", &x);
					pXmlElement2->QueryDoubleAttribute("y", &y);
					pXmlElement2->QueryDoubleAttribute("z", &z);
					this->add_transformation(new GLTranslation(x, y, z));
				}
			}
		}
		else if(strcmp(pXmlElement1->Value(), "texture")==0)
		{
			if(strcmp(pXmlElement1->Attribute("src"), "") != 0)
			{
				AddTexture(pXmlElement1->Attribute("src"));
				m_bTexture2DEnabled=true;
			}
			if(strcmp(pXmlElement1->Attribute("enabled"), "true") ==0)
			{
				m_bTexture2DEnabled=true;
			}
			if(strcmp(pXmlElement1->Attribute("alpha_blend"), "true")==0)
			{
				m_bAlphaBlend=true;
			}
		}
		else if(strcmp(pXmlElement1->Value(), "sounds")==0)
		{
			for(TiXmlElement* sound_xml=pXmlElement1->FirstChildElement(); sound_xml != NULL; sound_xml=sound_xml->NextSiblingElement())
			{
				std::string name=sound_xml->Attribute("name");
				std::string src=sound_xml->Attribute("src");
				int freq;
				sound_xml->QueryIntAttribute("frequency", &freq);
				m_sounds[name]=std::make_pair<>(src, freq);
				m_bSoundEnabled=true;
			}
		}
		else if(strcmp(pXmlElement1->Value(), "height_to_width_ratio")==0)
		{
			pXmlElement1->QueryDoubleAttribute("value", &m_Height2WidthRatio);
		}
	}

	return true;
}

void GLModel::EnableSound(bool bEnabled)
{
	if(m_sounds.empty())
	{
		m_bSoundEnabled=false;
	}
	else
	{
		m_bSoundEnabled=bEnabled; 
	}
}

bool GLModel::LoadTexture(const char* texture_name, GLuint texture_id)
{
	glBindTexture(GL_TEXTURE_2D, texture_id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	SDL_Surface* pTexture=IMG_Load(texture_name);

	/*
	SDL_SetColorKey(pTexture, SDL_SRCCOLORKEY, SDL_MapRGB(pTexture->format, 0, 0, 0));
	SDL_Surface* pTemp=SDL_DisplayFormatAlpha(pTexture);
	SDL_FreeSurface(pTexture);
	pTexture=pTemp;
	pTemp=NULL;
	*/

	if(pTexture == NULL)
	{
		GLExitSystem("GLModel::LoadTexture(const char* texture_name, GLuint texture_id)", "failed to load texture");
		return false;
	}

	GLenum format;
	GLint nColor=pTexture->format->BytesPerPixel;
	if(nColor==4)
	{
		if(pTexture->format->Rmask & 0x000000ff)
		{
			format=GL_RGBA;
		}
		else
		{
			format=GL_BGRA;
		}
	}
	else if(nColor==3)
	{
		if(pTexture->format->Rmask & 0x0000ff)
		{
			format=GL_RGB;
		}
		else
		{
			format=GL_BGR;
		}
	}
	else
	{
		GLExitSystem("GLModel::LoadTexture(const char* texture_name, GLuint texture_id)", "unrecognized format");
		return false;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, nColor, pTexture->w, pTexture->h, 0, format, GL_UNSIGNED_BYTE, pTexture->pixels);

	SDL_FreeSurface(pTexture);

	return true;
}

void GLModel::AddTexture(const char* texture_name)
{
	std::string texture=texture_name;
	m_texture_list.push_back(texture_name);
}
