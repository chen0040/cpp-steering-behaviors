#include "GLSteering_Cohesion.h"
#include "../GameEntity/Vehicle.h"
#include "../GameWorld/GameWorld.h"
#include <iostream>

GLSteering_Cohesion::GLSteering_Cohesion(Vehicle* pVehicle)
: GLSteering(pVehicle)
{
	m_type="cohesion";
}

GLVector GLSteering_Cohesion::calculate()
{
	GLVector vForce(0, 0, 0);
	const AgentGroup& agents=m_pVehicle->GetWorld()->GetConstAgents();
	AgentGroup::const_iterator pos_agent;
	GLVector CenterOfMass(0, 0, 0);
	int neighbor_count=0;
	for(pos_agent=agents.begin(); pos_agent !=agents.end(); ++pos_agent)
	{
		GameEntity* pAgent=*pos_agent;
		if(pAgent != m_pVehicle && pAgent->is_tagged() && pAgent->IsSameType(m_pVehicle))
		{
			CenterOfMass+=pAgent->get_position();
			neighbor_count++;
		}
	}

	if(neighbor_count > 0)
	{
		vForce= seek(CenterOfMass / neighbor_count);
	}

	return vForce;
}

GLVector GLSteering_Cohesion::seek(const GLVector &vTarget)
{
	GLVector vTo=vTarget - m_pVehicle->get_position();

	GLVector vDesiredVelocity=vTo.Truncate(m_pVehicle->GetMaxSpeed());

	return vDesiredVelocity - m_pVehicle->GetVelocity();
}