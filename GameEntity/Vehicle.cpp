#include "Vehicle.h"
#include "../GameWorld/GameWorld.h"
#include "../GLModel/GLModelManager.h"
#include "../GLUtil/GLExitSystem.h"
#include "../GLMath/GLMath.h"
#include <cassert>
#include <set>
#include "../GLRole/GLRole_Unknown.h"
#include "../GLLocomotion/GLLocomotion_Landcraft.h"

Vehicle::Vehicle(GameWorld* pWorld)
: MovingEntity(pWorld)
, m_dVisibleDistance(50)
, m_dHeight(0)
{
	m_pSteering=new GLSteering_Composite(this);
	m_Height2WidthRatio=1.7;
	m_pLocomotion=new GLLocomotion_Landcraft(this);
}

Vehicle::~Vehicle()
{
	if(m_pSteering != NULL)
	{
		delete m_pSteering;
		m_pSteering=NULL;
	}
	if(m_pLocomotion != NULL)
	{
		delete m_pLocomotion;
		m_pLocomotion=NULL;
	}
}

void Vehicle::SetLocomotion(GLLocomotion* pLocomotion)
{
	if(m_pLocomotion != NULL)
	{
		delete m_pLocomotion;
		m_pLocomotion=NULL;
	}
	m_pLocomotion=pLocomotion;
}

bool Vehicle::IsPenetratable(GameEntity* pAgent) const
{
	AgentRoleGroup::const_iterator pos_role;
	for(pos_role=m_roles.begin(); pos_role != m_roles.end(); ++pos_role)
	{
		if((pos_role->second)->IsPenetratable(pAgent->GetTypeId()))
		{
			return true;
		}
	}
	return false;
}

void Vehicle::Update(const long &lElapsedTicks)
{
	MovingEntity::Update(lElapsedTicks);

	assert(m_animator.GetModel() != NULL);

	double elapsed_time=static_cast<double>(lElapsedTicks) / 1000.0;

	GLVector vForce=m_pSteering->calculate();

	GLVector vAccel=vForce / m_dMass;

	m_vVelocity += vAccel * elapsed_time;

	m_vVelocity=m_vVelocity.Truncate(m_dMaxSpeed);

	double speedSq=m_vVelocity.lengthSq();
	double speed=sqrt(speedSq);

	UpdateFPS(speed);

	m_position += m_vVelocity * elapsed_time;

	if(m_pLocomotion != NULL)
	{
		m_pLocomotion->Update(lElapsedTicks);
	}

	m_animator.Update(lElapsedTicks);

	this->UpdateRoleAction(lElapsedTicks);

	if(speedSq > 0.00000001)
	{
		m_vHeading=m_vVelocity / speed;
		m_vSide=GLVector(-m_vHeading.z, m_vHeading.y, m_vHeading.x);
	}
}

void Vehicle::tag_neighbors(double detection_radius)
{
	AgentGroup& agents=m_pWorld->GetMutableAgents();
	AgentGroup::iterator pos_agent;
	for(pos_agent=agents.begin(); pos_agent!=agents.end(); ++pos_agent)
	{
		GameEntity* pAgent=*pos_agent;
		pAgent->untag();
		if(pAgent==this)
		{
			continue;
		}
		double distanceSq=(pAgent->get_position() - m_position).lengthSq();
		double range=detection_radius + pAgent->GetBoundingSphere().radius;
		if(distanceSq < range * range)
		{
			pAgent->tag();
		}
	}
}

void Vehicle::tag_obstacles(double scan_radius)
{
	std::set<GameEntity*> obs;
	m_pWorld->GetNeighborObstacles(this, obs, m_bounding_radius);
	std::set<GameEntity*>::iterator pos_obs;
	for(pos_obs = obs.begin(); pos_obs !=obs.end(); ++pos_obs)
	{
		GameEntity* pObs=*pos_obs;
		pObs->untag();
		if(pObs==this)
		{
			continue;
		}
		double distanceSq=(pObs->get_position() - m_position).lengthSq();
		double range=scan_radius + pObs->GetBoundingSphere().radius;
		if(distanceSq < range * range)
		{
			pObs->tag();
		}
	}
}

void Vehicle::UpdateFPS(double speed)
{
	int iMaxFPS=m_animator.GetMaxFPS();
	double ratio=iMaxFPS / m_dMaxSpeed;
	int iFPS=static_cast<int>(speed * ratio);

	m_animator.SetFPS(iFPS);
}

void Vehicle::SetModel(const char* fname)
{
	m_animator.SetModel(glModels.GetModel(fname));
	this->SetHeight2WidthRatio(glModels.GetModel(fname)->GetHeight2WidthRatio());
}

GLSteering* Vehicle::GetSteering(std::string sname)
{
	return m_pSteering->GetSteering(sname);
}

void Vehicle::AddSteering(const char* sname, GLSteering* pSteering)
{
	m_pSteering->AddSteering(sname, pSteering);
}

void Vehicle::RemoveSteering(const char* sname)
{
	m_pSteering->RemoveSteering(sname);
}

GLVector Vehicle::LocalToWorldCoordinate(const GLVector& vLocalTarget) const
{
	// m_vHeading=[cos(t), 0, sin(t)]
	// m_vSide=[-sin(t), 0, cos(t)]
	// vWorldTarget is obtained by rotating vLocalTarget through angle t and translating a (m_position) displacement
	// vWorldTarget'=[vLocalTarget.x * cos(t) - vLocalTarget.z * sin(t), 0, vLocalTarget.z * cos(t) + vLocalTarget.x * sin(t)]
	// vWorldTarget'.x=vLocalTarget.x * m_vHeading.x + vLocalTarget.z * m_vSide.x
	// vWorldTarget'.z=vLocalTarget.x * m_vHeading.z + vLocalTarget.z * m_vSide.z
	// vWorldTarget'.y=vLocalTarget.y
	// vWorldTarget=vWorldTarget' - m_position

	GLVector vWorldTarget;
	vWorldTarget.x=vLocalTarget.x * m_vHeading.x + vLocalTarget.z * m_vSide.x;
	vWorldTarget.z=vLocalTarget.x * m_vHeading.z + vLocalTarget.z * m_vSide.z;
	vWorldTarget.y=vLocalTarget.y;

	vWorldTarget+=m_position;

	return vWorldTarget;
}

GLVector Vehicle::WorldToLocalCoordinate(const GLVector& vWorldTarget) const
{
	// m_vHeading=[cos(t), 0, sin(t)]
	// m_vSide=[-sin(t), 0, cos(t)]
	// vLocalTarget is obtained by translating vWorldTarget by (-m_position) and then location angle -t
	// vLocalTarget'=vWorldTarget - m_position;
	// vLocalTarget=vLocalTarget' rotated by -t
	// vLocalTarget=[vLocalTarget'.x * cos(-t) - vLocalTarget'.z * sin(-t), 0, vLocalTarget'.z * cos(-t) + vLocalTarget'.x * sin(-t)]
	// vLocalTarget=[vLocalTarget'.x * cos(t) + vLocalTarget'.z * sin(t), 0, vLocalTarget'.z * cos(t) - vLocalTarget'.x * sin(t)]
	// vLocalTarget=[vLocalTarget' dot m_vHeading, 0, vLocalTarget' dot m_vSide]

	GLVector vLocalTarget_pi=vWorldTarget -  m_position;
	GLVector vLocalTarget;
	vLocalTarget.x=vLocalTarget_pi.dot(m_vHeading);
	vLocalTarget.z=vLocalTarget_pi.dot(m_vSide);
	vLocalTarget.y=vWorldTarget.y;

	return vLocalTarget;
}

GLVector Vehicle::LocalToWorldVector(const GLVector& vLocalTarget) const
{
	GLVector vWorldTarget;
	vWorldTarget.x=vLocalTarget.x * m_vHeading.x + vLocalTarget.z * m_vSide.x;
	vWorldTarget.z=vLocalTarget.x * m_vHeading.z + vLocalTarget.z * m_vSide.z;
	vWorldTarget.y=vLocalTarget.y;

	return vWorldTarget;
}

void Vehicle::Render()
{
	MovingEntity::Render();

	assert(m_animator.GetModel() != NULL);
	
	m_animator.PreRender();
	m_animator.Render();
	m_animator.PostRender();
}

