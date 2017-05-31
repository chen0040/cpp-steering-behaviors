#include "GLRole_Prey.h"
#include "../GameEntity/GameEntity.h"
#include "../GameWorld/GameWorld.h"
#include "../GLParticle/GLBroadcaster.h"
#include "../GLParticle/glParticleSystem.h"
#include <cfloat>

GLRole_Prey::GLRole_Prey(GameEntity* pAgent)
: GLRole(pAgent)
, m_pEvadeSteeringHandler(NULL)
, m_bBroadcast(false)
{

}

GLRole_Prey::~GLRole_Prey()
{

}

void GLRole_Prey::Update(const long& lElapsedTicks)
{
	if(m_pEvadeSteeringHandler == NULL)
	{
		return;
	}

	AgentGroup& agents=m_pAgent->GetWorld()->GetMutableAgents();
	AgentGroup::iterator pos_agent;
	Vehicle* pAgent=NULL;

	double closest_distance=DBL_MAX;
	Vehicle* current_predator=NULL;
	GLVector agent_position=m_pAgent->get_position();

	for(pos_agent=agents.begin(); pos_agent!=agents.end(); ++pos_agent)
	{
		pAgent=(*pos_agent);

		if(m_pAgent == pAgent)
		{
			continue;
		}

		if(!pAgent->InTheSameZone(m_pAgent))
		{
			continue;
		}

		if(m_predator_ids.find(pAgent->GetTypeId())!=m_predator_ids.end())
		{
			double distance=(pAgent->get_position() - agent_position).length();
			if(current_predator == NULL)
			{
				current_predator=pAgent;	
				closest_distance=distance;
			}
			else if(closest_distance > distance)
			{
				current_predator=pAgent;
				closest_distance=distance;
			}
		}
	}

	m_pEvadeSteeringHandler->SetEvadeTarget(current_predator);

	if(current_predator != NULL && m_bBroadcast)
	{
		if(closest_distance < 30)
		{
			GLBroadcaster* transmitter=new GLBroadcaster();
			transmitter->m_vPosition=m_pAgent->get_position();
			transmitter->Broadcast(100, 30, 1000, m_pAgent->get_bounding_radius());
			glParticleSys.AddEmitter(transmitter);
		}
	}
}

bool GLRole_Prey::IsPenetratable(int TypeId) const
{
	std::set<int>::const_iterator pos=m_predator_ids.find(TypeId);
	if(pos != m_predator_ids.end())
	{
		return true;
	}
	else
	{
		return false;
	}
}