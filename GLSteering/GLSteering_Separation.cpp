#include "GLSteering_Separation.h"
#include "../GameEntity/Vehicle.h"
#include "../GameWorld/GameWorld.h"

GLSteering_Separation::GLSteering_Separation(Vehicle* pVehicle)
: GLSteering(pVehicle)
{
	m_type="separation";
}

GLVector GLSteering_Separation::calculate()
{
	GLVector vForce;
	const AgentGroup& agents=m_pVehicle->GetWorld()->GetConstAgents();
	AgentGroup::const_iterator pos_agent;
	for(pos_agent=agents.begin(); pos_agent != agents.end(); ++pos_agent)
	{
		GameEntity* pAgent=*pos_agent;
		if(pAgent != m_pVehicle && pAgent->is_tagged())
		{
			GLVector vAway=m_pVehicle->get_position() - pAgent->get_position();
			double distance=vAway.length();
			if(distance==0) distance=1E-4;
			vForce+=(vAway.normalize() / distance);
		}
	}

	return vForce;
}