#include "GLTower.h"
#include "../GameWorld/GameWorld.h"
#include "GameEntity.h"
#include "../GLFont/GLFontSys.h"
#include <glut.h>
#include <sstream>
#include "../GLMath/GLMath.h"
#include <cmath>

GLTower::GLTower(GameWorld* pWorld)
: Obstacle(pWorld)
, m_capture_id(-1)
, m_has_captures(0)
, m_bDefaultTowerDL(false)
{
	m_Height2WidthRatio=2.1;
}

GLTower::~GLTower()
{
	if(m_bDefaultTowerDL)
	{
		glDeleteLists(m_default_tower_display_list, 2);
		m_bDefaultTowerDL=false;
	}
}

void GLTower::Update(const long& lElapsedTicks)
{
	this->SetColor(1, 1, 1);
	AgentGroup& agents=m_pWorld->GetMutableAgents();
	for(size_t i=0; i<agents.size(); i++)
	{
		if(agents[i]->GetTypeId()!=m_capture_id)
		{
			continue;
		}
		if(agents[i]->CollideWith(this))
		{
			agents[i]->mark_for_destruction();
			this->SetColor(0, 1, 0);
			m_has_captures++;
		}
	}
}


void GLTower::PreRender()
{
#ifdef GL_OBJECT_STORE_GL_STATES
	GLObject::PreRender();
#endif

	glEnable(GL_COLOR_MATERIAL);

	glPushMatrix();
	glTranslated(m_position.x, m_position.y, m_position.z);
}

void GLTower::PostRender()
{
	glPopMatrix();

	glDisable(GL_COLOR_MATERIAL);

#ifdef GL_OBJECT_STORE_GL_STATES
	GLObject::PostRender();
#endif
}

bool GLTower::IsPenetratable(GameEntity* pAgent) const
{
	if(m_capture_id == pAgent->GetTypeId())
	{
		return true;
	}
	return false;
}

void GLTower::Render()
{
	if(m_pModel == NULL)
	{
		if(m_bDefaultTowerDL==false)
		{
			float side=static_cast<float>(m_bounding_radius); // * sqrt(2.0) / 2.0;
			float tower_height=4.44f;

			m_default_tower_display_list=glGenLists(2);

			glNewList(m_default_tower_display_list, GL_COMPILE);
			glPushMatrix();
			glTranslatef(0.0f, tower_height*side / 2.0f, 0.0f);
			glScalef(1.0f, tower_height, 1.0f);
			glutSolidCube(side);
			glPopMatrix();
			glEndList();

			glNewList(m_default_tower_display_list+1, GL_COMPILE);
			glPushMatrix();
			glTranslatef(0.0f, 0.5f*side, 0.0f);
			glScalef(1.2f, 1.0f, 1.2f);
			glutSolidCube(side);
			glPopMatrix();

			glPushMatrix();
			glTranslatef(0.0f, 0.1f*side, 0.0f);
			glScalef(1.5f, 0.2f, 1.5f);
			glutSolidCube(side);
			glPopMatrix();

			glPushMatrix();
			glTranslatef(0.0f, side*(1.6f), 0.0f);
			glScalef(1.2f, 1.0f, 1.2f);
			glutSolidCube(side);
			glPopMatrix();

			glPushMatrix();
			glTranslatef(0.0f, side*(2.7f), 0.0f);
			glScalef(1.2f, 1.0f, 1.2f);
			glutSolidCube(side);
			glPopMatrix();

			glPushMatrix();
			glTranslatef(0.0f, side*(3.8f), 0.0f);
			glScalef(1.2f, 1.0f, 1.2f);
			glutSolidCube(side);
			glPopMatrix();

			glEndList();
			m_bDefaultTowerDL=true;
		}
		

		glDisable(GL_CULL_FACE);
		
		glColor3f(0.5f, 0.1f, 0.5f);
		glCallList(m_default_tower_display_list);

		glColor3d(m_vColor.x, m_vColor.y, m_vColor.z);
		glCallList(m_default_tower_display_list+1);
		
		glEnable(GL_CULL_FACE);
	}
	else
	{
		glColor3d(m_vColor.x, m_vColor.y, m_vColor.z);

		//std::cout << "PreRender: " << m_pModel->toString() << std::endl;
		m_pModel->PreRender();
		//std::cout << "Render: " << m_pModel->toString() << std::endl;
		m_pModel->Render(0);
		//std::cout << "PostRender: " << m_pModel->toString() << std::endl;
		m_pModel->PostRender();
	}
	
	if(m_has_captures > 0)
	{
		glTranslatef(0, 10, 0);
		glPushMatrix();
		
		glEnable(GL_BLEND);
		glShadeModel(GL_SMOOTH);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glColor4f(0, 1, 0, 0.7f);
		glutSolidSphere(1, 20, 20);

		glDisable(GL_BLEND);
		glShadeModel(GL_FLAT);

		glTranslatef(-1, 1.5f, 0);
		std::ostringstream oss;
		oss << "count: " << m_has_captures;
		glFont.print3D(oss.str());

		glPopMatrix();
	}
}