#include "GLLocomotion.h"
#include "../GameEntity/Vehicle.h"
#include "../GameWorld/GameWorld.h"

GLLocomotion::GLLocomotion(Vehicle* pAgent)
: m_pAgent(pAgent)
{

}

GLLocomotion::~GLLocomotion()
{

}

void GLLocomotion::EnforceNonPenetration()
{
	AgentGroup& agents=m_pAgent->GetWorld()->GetMutableAgents();
	AgentGroup::iterator pos_agent;
	GameEntity* pAgent=NULL;
	for(pos_agent=agents.begin(); pos_agent!=agents.end(); ++pos_agent)
	{
		pAgent=(*pos_agent);
		if(m_pAgent == pAgent)
		{
			continue;
		}

		if(!m_pAgent->InTheSameZone(pAgent))
		{
			continue;
		}

		m_pAgent->EnforceNonPenetration(pAgent);
	}
}