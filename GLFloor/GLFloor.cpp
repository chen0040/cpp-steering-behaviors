#include "GLFloor.h"
#include "../GameWorld/GameWorld.h"
#include "../GLTexture/GLTextureManager.h"
#include <iostream>

GLFloor::GLFloor()
: GLObject()
, m_pTexture(NULL)
, m_pWorld(NULL)
, m_bDisplayList(false)
{
	
}

GLFloor::~GLFloor()
{
	if(m_bDisplayList)
	{
		glDeleteLists(m_display_lists, 1);
		m_bDisplayList=false;
	}
}

void GLFloor::Init(const char* texture_name, GameWorld* pWorld)
{
	m_pWorld=pWorld;
	CreateHorizontalPlane(DEFAULT_LENGTH);
	this->SetTexture(glTextures.GetTexture(texture_name));
}

double GLFloor::GetLength() const
{
	GLVector disp=p1 - p2;
	return disp.length();
}

void GLFloor::GenDisplayLists()
{
	m_display_lists=glGenLists(1);
	glNewList(m_display_lists, GL_COMPILE);

	glBegin(GL_QUADS);
	glNormal3d(m_normal.x, m_normal.y, m_normal.z);
	glTexCoord2i(0, 1);
	glVertex3d(p4.x, p4.y, p4.z);
	glTexCoord2i(1, 1);
	glVertex3d(p3.x, p3.y, p3.z);
	glTexCoord2i(1, 0);
	glVertex3d(p2.x, p2.y, p2.z);
	glTexCoord2i(0, 0);
	glVertex3d(p1.x, p1.y, p1.z);
	glEnd();

	glEndList();
}

void GLFloor::CreateHorizontalPlane(double length)
{
	int half_length=static_cast<int>(length / 2);
	p1=GLVector(-half_length, 0, half_length);
	p2=GLVector(half_length, 0, half_length);
	p3=GLVector(half_length, 0, -half_length);
	p4=GLVector(-half_length, 0, -half_length);
	calculate_normal();

	m_obsQuadTree.MakeQuadTree(5, -half_length, -half_length, half_length, half_length);

	if(m_bDisplayList)
	{
		glDeleteLists(m_display_lists, 1);
		m_bDisplayList=false;
	}
}

void GLFloor::calculate_normal()
{
	GLVector v1=p2 - p1;
	GLVector v2=p3 - p1;

	GLVector n=v1.cross(v2);
	m_normal=n.normalize();

	m_distance_to_origin=p1.dot(m_normal);
}

void GLFloor::Render()
{
	if(m_reflected_objects.empty())
	{
		RenderNormal();
	}
	else
	{
		RenderReflection();
	}
}

void GLFloor::RenderNormal()
{
	if(m_pTexture!=NULL) 
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_pTexture->GetTextureId());
	}

	if(m_bDisplayList==false)
	{
		GenDisplayLists();
		m_bDisplayList=true;
	}
	glCallList(m_display_lists);

	if(m_pTexture!=NULL) glDisable(GL_TEXTURE_2D);
}

void GLFloor::RenderReflection()
{
	glEnable(GL_STENCIL_TEST);
	glColorMask(0, 0, 0, 0);
	glDisable(GL_DEPTH_TEST);

	glStencilFunc(GL_ALWAYS, 1, 1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	
	RenderNormal();

	glColorMask(1, 1, 1, 1);
	glEnable(GL_DEPTH_TEST);

	glStencilFunc(GL_EQUAL, 1, 1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	
	glCullFace(GL_FRONT);
	glPushMatrix();
	GLVector scaling(1.0, 1.0, 1.0);
	scaling -= m_normal * 2.0;
	glScaled(scaling.x, scaling.y, scaling.z);
	std::vector<GLObject*>::const_iterator pos_agent;
	for(pos_agent=m_reflected_objects.begin(); pos_agent!=m_reflected_objects.end(); ++pos_agent)
	{
		(*pos_agent)->PreRender();
		(*pos_agent)->Render();
		(*pos_agent)->PostRender();
	}
	glPopMatrix();
	glCullFace(GL_BACK);

	glDisable(GL_STENCIL_TEST);

	glEnable(GL_BLEND);
	glEnable(GL_COLOR_MATERIAL);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(1.0f, 1.0f, 1.0f, 0.7f);

	RenderNormal();

	glDisable(GL_BLEND);
	glDisable(GL_COLOR_MATERIAL);
	
}

void GLFloor::PreRender()
{
#ifdef GL_OBJECT_STORE_GL_STATES
	GLObject::PreRender();
#endif

	glPushMatrix();
	//should not call glColor() function here or otherwise RenderReflection() will not work
}

void GLFloor::PostRender()
{
	glPopMatrix();
#ifdef GL_OBJECT_STORE_GL_STATES
	GLObject::PostRender();
#endif
}