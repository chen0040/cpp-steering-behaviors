#include "GameEntity.h"
#include "../GameWorld/GameWorld.h"
#include "../GLMath/GLMath.h"
#include <cassert>
#include <cmath>
#include "../GLParticle/GLParticleSystem.h"
#include "../GLParticle/GLParticleSteering_Gravity.h"
#include "../GLParticle/GLDebris.h"
#include "../GLParticle/GLBurstWave.h"

//#define GAME_ENTITY_SHOW_FIELD_SPHERE

#ifdef GAME_ENTITY_SHOW_FIELD_SPHERE
#include <glut.h>
#endif

GameEntity::GameEntity(GameWorld* pWorld)
: m_bounding_radius(1.8)
, m_is_tagged(false)
, m_pWorld(pWorld)
, m_bShowField(false)
, m_typeId(0)
, m_vColor(1.0, 1.0, 1.0)
, m_iZoneId(ZONE_LAND)
, m_bMarkedForDestruction(false)
, m_Height2WidthRatio(1)
, m_bDisplayList(false)
, m_bShowDot(true)
, m_bCameraFocus(false)
{
	if(m_iZoneId==ZONE_LAND)
	{
		m_position.y=m_pWorld->GetEnvHeight(m_position.x, m_position.z);
	}
}

void GameEntity::GenDisplayLists()
{
	m_glFieldList=glGenLists(2);

	this->GenDisplayList_Field(m_glFieldList);

	GLuint display_list_id=m_glFieldList+1;
	this->GenDisplayList_Dot(display_list_id);
}


void GameEntity::GenDisplayList_Dot(GLuint& display_list_id)
{
	const int DT=2;
	if(m_iZoneId==GameEntity::ZONE_LAND)
	{
		glNewList(display_list_id, GL_COMPILE);
		glBegin(GL_QUADS);
		glVertex2i(-DT, -DT);
		glVertex2i(DT, -DT);
		glVertex2i(DT, DT);
		glVertex2i(-DT, DT);
		glEnd();
		glEndList();
	}
	else
	{
		glNewList(display_list_id, GL_COMPILE);
		glBegin(GL_LINES);
		glVertex2i(-DT, 0);
		glVertex2i(DT, 0);
		glVertex2i(0, DT);
		glVertex2i(0, -DT);
		glEnd();
		glEndList();
	}
}

void GameEntity::GenDisplayList_Field(GLuint& display_list_id)
{
	const int SECTOR_COUNT=30;
	const double RING_HEIGHT=0.5;

	double sector=glMath.PI() * 2.0 / SECTOR_COUNT;
	glNewList(display_list_id, GL_COMPILE);
	glBegin(GL_LINE_LOOP);
	for(int i=0; i<SECTOR_COUNT; i++)
	{
		glVertex3d(m_bounding_radius * cos(sector * i), RING_HEIGHT, m_bounding_radius * sin(sector * i));
	}
	glEnd();
	glEndList();
}

void GameEntity::RenderDot()
{
	if(m_bShowDot==false)
	{
		return;
	}

	if(m_bDisplayList==false)
	{
		GenDisplayLists();
		m_bDisplayList=true;
	}
	
	glColor3d(m_vColor.x, m_vColor.y, m_vColor.z);
	glCallList(m_glFieldList+1);
}

GameEntity::~GameEntity()
{
	if(m_bDisplayList)
	{
		glDeleteLists(m_glFieldList, 2);
		m_bDisplayList=false;
	}

	AgentRoleGroup::iterator pos_role;
	for(pos_role=m_roles.begin(); pos_role != m_roles.end(); ++pos_role)
	{
		if(pos_role->second != NULL)
		{
			delete pos_role->second;
		}
	}
	m_roles.clear();
}

void GameEntity::set_position(const GLVector& position)
{
	if(m_iZoneId==ZONE_LAND)
	{
		m_position=position;
		double new_y=m_pWorld->GetEnvHeight(position.x, position.z);
		m_position.y=new_y;
	}
	else
	{
		m_position=position; 
	}
}

void GameEntity::SetColor(double r, double g, double b)
{
	m_vColor.x=r;
	m_vColor.y=g;
	m_vColor.z=b;
}

bool GameEntity::CollideWith(const GameEntity* rhs, double offset) const
{
	GLSphere sphere1=rhs->GetBoundingSphere();
	GLSphere sphere2=this->GetBoundingSphere();

	double center_distance=(sphere1.center - sphere2.center).length();
	double radius_sum= sphere1.radius + sphere2.radius;

	if(center_distance < radius_sum - offset)
	{
		return true;
	}

	return false;
}

GLSphere GameEntity::GetBoundingSphere() const
{
	GLSphere sphere;
	sphere.center=m_position;
	sphere.radius=m_bounding_radius;
	sphere.center.y+=m_bounding_radius*this->GetHeight2WidthRatio();

	return sphere;
}

GLCube GameEntity::GetBoundingCube() const
{
	GLCube cube;

	cube.minx=m_position.x - m_bounding_radius;
	cube.miny=m_position.y;
	cube.minz=m_position.z - m_bounding_radius;

	cube.maxx=m_position.x + m_bounding_radius;
	cube.maxy=m_position.y + m_bounding_radius * m_Height2WidthRatio;
	cube.maxz=m_position.z + m_bounding_radius;

	return cube;
}

void GameEntity::Render()
{
	if(m_bShowField)
	{
		RenderField();
	}
	if(m_bCameraFocus)
	{
		glPushMatrix();
		glTranslatef(0, static_cast<float>(m_bounding_radius * this->GetHeight2WidthRatio()) * 2.0f, 0);
		glScalef(0.5f, 0.5f, 0.5f);
		RenderField();
		glPopMatrix();
	}
}

void GameEntity::RenderField()
{
	if(m_bDisplayList==false)
	{
		GenDisplayLists();
		m_bDisplayList=true;
	}

#ifdef GAME_ENTITY_SHOW_FIELD_SPHERE
	glEnable(GL_BLEND);
	glColor4f(1, 1, 1, 0.5);

	glutSolidSphere(m_bounding_radius, 20, 20);
	glDisable(GL_BLEND);
#else
	glLineWidth(3.0);
	glCallList(m_glFieldList);
	glLineWidth(1.0);
#endif
}

void GameEntity::Update(const long &lElapsedTicks)
{
	if(m_bCameraFocus && (m_bMarkedForDestruction==false))
	{
		m_pWorld->GetCamera().SetFocus(m_position);
	}
}

bool GameEntity::IntersectWithMouse(double& intersected_distance) const
{
	m_pWorld->GetCamera().PreTransform();
	m_pWorld->GetCamera().Transform();
	GLRay mouse_ray=m_pWorld->GetMouse().GetRay();
	m_pWorld->GetCamera().PostTransform();
	
	bool intersected=mouse_ray.intersect(GetBoundingSphere(), intersected_distance);

	return intersected;
}

bool GameEntity::IntersectWithMouse() const
{
	m_pWorld->GetCamera().PreTransform();
	m_pWorld->GetCamera().Transform();
	GLRay mouse_ray=m_pWorld->GetMouse().GetRay();
	m_pWorld->GetCamera().PostTransform();
	
	double intersected_distance;
	bool intersected=mouse_ray.intersect(GetBoundingSphere(), intersected_distance);

	return intersected;
}

void GameEntity::PreRender()
{
#ifdef GL_OBJECT_STORE_GL_STATES
	GLObject::PreRender();
#endif

	glEnable(GL_COLOR_MATERIAL);
	glColor3d(m_vColor.x, m_vColor.y, m_vColor.z);

	glPushMatrix();
	glTranslated(m_position.x, m_position.y, m_position.z);
}

void GameEntity::PostRender()
{
	glPopMatrix();

	glDisable(GL_COLOR_MATERIAL);

#ifdef GL_OBJECT_STORE_GL_STATES
	GLObject::PostRender();
#endif
}

bool GameEntity::IsPenetratable(GameEntity* pAgent) const
{
	return false;
}

void GameEntity::EnforceNonPenetration(GameEntity* pAgent)
{
	assert(pAgent != this);

	if(this->IsPenetratable(pAgent))
	{
		return;
	}

	GLVector vTo=pAgent->get_position() - this->get_position();
	double distance=vTo.length();
	double overlapping_distance=pAgent->GetBoundingSphere().radius + m_bounding_radius - distance;
	if(overlapping_distance > 0)
	{
		if(distance==0)
		{
			distance=glMath.Epsilon();
		}
		pAgent->set_position(pAgent->get_position()+vTo * overlapping_distance / distance);
	}
}

void GameEntity::UpdateRoleAction(const long& lElapsedTicks)
{
	AgentRoleGroup::iterator pos_role;
	for(pos_role=m_roles.begin(); pos_role != m_roles.end(); ++pos_role)
	{
		if(pos_role->second != NULL)
		{
			pos_role->second->Update(lElapsedTicks);
		}
	}
}

void GameEntity::RemoveRole(std::string role_name)
{
	AgentRoleGroup::iterator pos_role=m_roles.find(role_name);
	if(pos_role!=m_roles.end())
	{
		if(pos_role->second != NULL)
		{
			delete pos_role->second;
			m_roles.erase(pos_role);
		}
	}
}

void GameEntity::AddRole(std::string role_name, GLRole* pRole)
{
	AgentRoleGroup::iterator pos_role=m_roles.find(role_name);
	if(pos_role != m_roles.end())
	{
		delete pos_role->second;
		pos_role->second=pRole;
	}
	else
	{
		m_roles[role_name]=pRole;
	}
}

void GameEntity::GenerateBurstWave(int packet_count_per_wave, double packet_travelling_distance, double packet_travelling_speed, double broadcast_frequency, double life_span)
{
	GLBurstWave* wave_generator=new GLBurstWave(life_span);
	wave_generator->m_vPosition=m_position;
	wave_generator->SetBroadcastCountPerWave(packet_count_per_wave);
	wave_generator->SetBroadcastFrequency(broadcast_frequency);
	wave_generator->SetCoreRadius(m_bounding_radius);
	wave_generator->SetPacketProperty(packet_travelling_distance, packet_travelling_speed);
	glParticleSys.AddEmitter(wave_generator);
}

void GameEntity::Explode(int bill_board_count, double particle_life, double particle_initial_speed)
{
	GLVector position=this->get_position();

	GLEmitter* pEmitter=new GLEmitter();
	pEmitter->m_vPosition=position+GLVector(0, m_bounding_radius* 1.7, 0);

	for(int i=0; i<bill_board_count; ++i)
	{
		GLDebris* particle=new GLDebris(new GLParticleSteering_Gravity());
		particle->m_dInitialSpeed=particle_initial_speed;
		particle->m_life=particle_life;
		particle->m_vColor=m_vColor+GLVector(glMath.nextDouble(0, 0.1), glMath.nextDouble(0, 0.1), glMath.nextDouble(0, 0.1));

		GLVector vVector(glMath.nextDouble(-1, 1), glMath.nextDouble(-1, 1), glMath.nextDouble(-1, 1));
		particle->m_vOrientationVelocity=vVector;

		particle->m_vScale.x=glMath.nextDouble(-2, 2);
		particle->m_vScale.y=glMath.nextDouble(-2, 2);
		particle->m_vScale.z=glMath.nextDouble(-2, 2);

		vVector.x=glMath.nextDouble(-1, 1);
		vVector.y=glMath.nextDouble(-1, 1);
		vVector.z=glMath.nextDouble(-1, 1);
		particle->m_vVelocity=vVector.normalize() * particle_initial_speed;

		vVector.x=glMath.nextDouble(-1, 1);
		vVector.y=glMath.nextDouble(-1, 1);
		vVector.z=glMath.nextDouble(-1, 1);
		particle->m_vPosition=position + vVector.normalize() * m_bounding_radius * 0.3;

		pEmitter->AddParticle(particle);
	}

	glParticleSys.AddEmitter(pEmitter);

	if(m_pWorld->IsHighEndSystem())
	{
		int packet_count_per_wave=1000;
		double packet_travelling_distance=m_bounding_radius * 3;
		double packet_travelling_speed=5;
		double broadcast_frequency=30;
		double life_span=0.3;
		GenerateBurstWave(packet_count_per_wave, packet_travelling_distance, packet_travelling_speed, broadcast_frequency, life_span);
	}
}