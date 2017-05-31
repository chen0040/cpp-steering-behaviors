#include "GLLocomotion_Landcraft.h"
#include "../GameEntity/Vehicle.h"
#include "../GLMath/GLMath.h"
#include "../GameWorld/GameWorld.h"

GLLocomotion_Landcraft::GLLocomotion_Landcraft(Vehicle* pAgent)
: GLLocomotion(pAgent)
{

}

GLLocomotion_Landcraft::~GLLocomotion_Landcraft()
{

}

void GLLocomotion_Landcraft::Update(const long& lElapsedTicks)
{
	GLVector vPosition=m_pAgent->get_position();
	GLVector vVelocity=m_pAgent->GetVelocity();

	double old_y=vPosition.y;
	double new_y=m_pAgent->GetWorld()->GetEnvHeight(vPosition.x, vPosition.z);
	vPosition.y=new_y;

	//vVelocity.y=new_y - old_y;
	vVelocity.y=0;

	m_pAgent->SetVelocity(vVelocity);
	m_pAgent->set_position(vPosition);

	this->EnforceNonPenetration();
	this->EnforceNonPenetrationOnLandObstacles();
}

void GLLocomotion_Landcraft::EnforceNonPenetrationOnLandObstacles()
{
	std::set<GameEntity*> obs;
	m_pAgent->GetWorld()->GetNeighborObstacles(m_pAgent, obs, m_pAgent->get_bounding_radius());

	if(obs.empty())
	{
		return;
	}

	std::set<GameEntity*>::iterator pos_obs;
	for(pos_obs=obs.begin(); pos_obs != obs.end(); ++pos_obs)
	{
		(*pos_obs)->EnforceNonPenetration(m_pAgent);
	}
}