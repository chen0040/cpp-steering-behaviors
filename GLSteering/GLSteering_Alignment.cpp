#include "GLSteering_Alignment.h"
#include "../GameEntity/Vehicle.h"
#include "../GameWorld/GameWorld.h"
#include <iostream>

GLSteering_Alignment::GLSteering_Alignment(Vehicle* pVehicle)
: GLSteering(pVehicle)
{
	m_type="alignment";
}

GLVector GLSteering_Alignment::calculate()
{
	GLVector vForce(0, 0, 0);
	GLVector vAlignment;
	int alignment_count=0;

	const AgentGroup& agents=m_pVehicle->GetWorld()->GetConstAgents();
	AgentGroup::const_iterator pos_agent;
	for(pos_agent=agents.begin(); pos_agent != agents.end(); ++pos_agent)
	{
		MovingEntity* pAgent=*pos_agent;
		
		if(pAgent!=m_pVehicle && pAgent->is_tagged() && pAgent->IsSameType(m_pVehicle))
		{
			vAlignment+=pAgent->GetHeading();
			alignment_count++;
		}
	}

	if(alignment_count > 0)
	{
		vAlignment /= alignment_count;
		vForce=vAlignment - m_pVehicle->GetHeading();
	}
	
	return vForce;
}