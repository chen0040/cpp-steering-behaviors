#include "GLSkybox.h"
#include "../GLMath/GLMath.h"
#include "../GLTexture/GLTextureManager.h"
#include <glut.h>
#include "../tinyxml/tinyxml.h"
#include "../GLUtil/GLExitSystem.h"
#include <sstream>

GLSkybox::GLSkybox()
: GLObject()
, m_bDisplayList(false)
{
	m_front_texture="Skybox_Tex\\default\\nightsky_FR.JPG";
	m_back_texture="Skybox_Tex\\default\\nightsky_BK.JPG";
	m_left_texture="Skybox_Tex\\default\\nightsky_LF.JPG";
	m_right_texture="Skybox_Tex\\default\\nightsky_RT.JPG";
	m_bottom_texture="Skybox_Tex\\default\\nightsky_DN.JPG";
	m_top_texture="Skybox_Tex\\default\\nightsky_UP.JPG";
}

void GLSkybox::LoadScript(const char* filename)
{
	TiXmlDocument doc;
	if(!doc.LoadFile(filename))
	{
		std::ostringstream oss;
		oss << "Failed to Load " << filename;
		GLExitSystem("GLSkybox::LoadScript(const char* filename)", oss.str().c_str());
	}

	TiXmlElement* xml_root=doc.RootElement();
	for(TiXmlElement* xml_level1=xml_root->FirstChildElement(); xml_level1 != NULL; xml_level1=xml_level1->NextSiblingElement())
	{
		if(strcmp(xml_level1->Value(), "set")==0)
		{
			if(strcmp(xml_level1->Attribute("name"), xml_root->Attribute("use"))==0)
			{
				for(TiXmlElement* xml_level2=xml_level1->FirstChildElement(); xml_level2 != NULL; xml_level2=xml_level2->NextSiblingElement())
				{
					if(strcmp(xml_level2->Value(), "top")==0)
					{
						m_top_texture=xml_level2->Attribute("src");
					}
					else if(strcmp(xml_level2->Value(), "bottom")==0)
					{
						m_bottom_texture=xml_level2->Attribute("src");
					}
					else if(strcmp(xml_level2->Value(), "left")==0)
					{
						m_left_texture=xml_level2->Attribute("src");
					}
					else if(strcmp(xml_level2->Value(), "right")==0)
					{
						m_right_texture=xml_level2->Attribute("src");
					}
					else if(strcmp(xml_level2->Value(), "front")==0)
					{
						m_front_texture=xml_level2->Attribute("src");
					}
					else if(strcmp(xml_level2->Value(), "back")==0)
					{
						m_back_texture=xml_level2->Attribute("src");
					}
				}
			}
		}
	}
}

void GLSkybox::GenDisplayLists()
{
	const int SIDE_LENGTH=10;

	m_display_lists=glGenLists(5);

	glNewList(m_display_lists, GL_COMPILE);
	glBegin(GL_QUADS);
	glTexCoord2i(0, 1);
	glVertex3i(-SIDE_LENGTH, -SIDE_LENGTH, -SIDE_LENGTH);
	glTexCoord2i(0, 0);
	glVertex3i(-SIDE_LENGTH, SIDE_LENGTH, -SIDE_LENGTH);
	glTexCoord2i(1, 0);
	glVertex3i(SIDE_LENGTH, SIDE_LENGTH, -SIDE_LENGTH);
	glTexCoord2i(1, 1);
	glVertex3i(SIDE_LENGTH, -SIDE_LENGTH, -SIDE_LENGTH);
	glEnd();
	glEndList();

	glNewList(m_display_lists+1, GL_COMPILE);
	glBegin(GL_QUADS);
	glTexCoord2i(0.0, 0.0);
	glVertex3i(SIDE_LENGTH, SIDE_LENGTH, SIDE_LENGTH);
	glTexCoord2i(1.0, 0.0);
	glVertex3i(-SIDE_LENGTH, SIDE_LENGTH, SIDE_LENGTH);
	glTexCoord2i(1.0, 1.0);
	glVertex3i(-SIDE_LENGTH, -SIDE_LENGTH, SIDE_LENGTH);
	glTexCoord2i(0.0, 1.0);
	glVertex3i(SIDE_LENGTH, -SIDE_LENGTH, SIDE_LENGTH);
	glEnd();
	glEndList();

	glNewList(m_display_lists+2, GL_COMPILE);
	glBegin(GL_QUADS);
	glTexCoord2i(0.0, 0.0);
	glVertex3i(-SIDE_LENGTH, SIDE_LENGTH, SIDE_LENGTH);
	glTexCoord2i(1.0, 0.0);
	glVertex3i(-SIDE_LENGTH, SIDE_LENGTH, -SIDE_LENGTH);
	glTexCoord2i(1.0, 1.0);
	glVertex3i(-SIDE_LENGTH, -SIDE_LENGTH, -SIDE_LENGTH);
	glTexCoord2i(0.0, 1.0);
	glVertex3i(-SIDE_LENGTH, -SIDE_LENGTH, SIDE_LENGTH);
	glEnd();
	glEndList();

	glNewList(m_display_lists+3, GL_COMPILE);
	glBegin(GL_QUADS);
	glTexCoord2i(1.0, 1.0);
	glVertex3i(SIDE_LENGTH, -SIDE_LENGTH, SIDE_LENGTH);
	glTexCoord2i(0.0, 1.0);
	glVertex3i(SIDE_LENGTH, -SIDE_LENGTH, -SIDE_LENGTH);
	glTexCoord2i(0.0, 0.0);
	glVertex3i(SIDE_LENGTH, SIDE_LENGTH, -SIDE_LENGTH);
	glTexCoord2i(1.0, 0.0);
	glVertex3i(SIDE_LENGTH, SIDE_LENGTH, SIDE_LENGTH);
	glEnd();
	glEndList();

	glNewList(m_display_lists+4, GL_COMPILE);
	glBegin(GL_QUADS);
	glTexCoord2i(1.0, 0.0);
	glVertex3i(SIDE_LENGTH, SIDE_LENGTH, SIDE_LENGTH);
	glTexCoord2i(0.0, 0.0);
	glVertex3i(SIDE_LENGTH, SIDE_LENGTH, -SIDE_LENGTH);
	glTexCoord2i(0.0, 1.0);
	glVertex3i(-SIDE_LENGTH, SIDE_LENGTH, -SIDE_LENGTH);
	glTexCoord2i(1.0, 1.0);
	glVertex3i(-SIDE_LENGTH, SIDE_LENGTH, SIDE_LENGTH);
	glEnd();
	glEndList();
}

GLSkybox::~GLSkybox()
{
	if(m_bDisplayList)
	{
		glDeleteLists(m_display_lists, 5);
		m_bDisplayList=false;
	}
}

void GLSkybox::Render()
{
	if(m_bDisplayList==false)
	{
		GenDisplayLists();
		m_bDisplayList=true;
	}

	glEnable(GL_TEXTURE_2D);

	GLTexture* pTexture=glTextures.GetTexture(m_back_texture.c_str());
	glBindTexture(GL_TEXTURE_2D, pTexture->GetTextureId());
	glCallList(m_display_lists);

	pTexture=glTextures.GetTexture(m_front_texture.c_str());
	glBindTexture(GL_TEXTURE_2D, pTexture->GetTextureId());
	glCallList(m_display_lists+1);

	pTexture=glTextures.GetTexture(m_left_texture.c_str());
	glBindTexture(GL_TEXTURE_2D, pTexture->GetTextureId());
	glCallList(m_display_lists+2);

	pTexture=glTextures.GetTexture(m_right_texture.c_str());
	glBindTexture(GL_TEXTURE_2D, pTexture->GetTextureId());
	glCallList(m_display_lists+3);

	pTexture=glTextures.GetTexture(m_top_texture.c_str());
	glBindTexture(GL_TEXTURE_2D, pTexture->GetTextureId());
	glCallList(m_display_lists+4);
	
	glDisable(GL_TEXTURE_2D);
}

void GLSkybox::PreRender()
{
#ifdef GL_OBJECT_STORE_GL_STATES
	GLObject::PreRender();
#endif

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_FOG);

	// important: remember to disable to GL_TEXTURE_2D otherwise skybox (WITHOUT TEXTURE VERSION) won't work
	// glDisable(GL_TEXTURE_2D); 

	glDisable(GL_COLOR_MATERIAL);
	//glColor4f(1, 1, 1, 1);
	glDepthMask(GL_FALSE);

	GLfloat mat[16];
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glGetFloatv(GL_MODELVIEW_MATRIX, mat);
	mat[12]=mat[13]=mat[14]=0.0f;
	glLoadMatrixf(mat);
}

void GLSkybox::PostRender()
{
	glPopMatrix();

	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);

#ifdef GL_OBJECT_STORE_GL_STATES
	GLObject::PostRender();
#endif
}

void GLSkybox::Update(const long& lElapsedTicks)
{
	
}