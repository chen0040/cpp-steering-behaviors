#include "GLRole_Predator.h"
#include "../GameEntity/GameEntity.h"
#include "../GameWorld/GameWorld.h"
#include "../GameEntity/Corpse.h"
#include <cfloat>

GLRole_Predator::GLRole_Predator(GameEntity* pAgent)
: GLRole(pAgent)
, m_corpse_scene_duration(-1)
, m_pPursuitSteeringHandler(NULL)
, m_bIsKiller(true)
{

}

GLRole_Predator::~GLRole_Predator()
{

}

void GLRole_Predator::SetCorpseSceneDuration(double duration)
{
	m_corpse_scene_duration=duration;
}

bool GLRole_Predator::IsPenetratable(int TypeId) const
{
	std::set<int>::const_iterator pos=m_prey_ids.find(TypeId);
	if(pos != m_prey_ids.end())
	{
		return true;
	}
	return false;
}

void GLRole_Predator::Update(const long& lElapsedTicks)
{
	AgentGroup& agents=m_pAgent->GetWorld()->GetMutableAgents();
	AgentGroup::iterator pos_agent;
	Vehicle* pAgent=NULL;

	double closest_distance=DBL_MAX;
	Vehicle* current_prey=NULL;
	GLVector predator_position=m_pAgent->get_position();

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

		if(m_prey_ids.find(pAgent->GetTypeId())!=m_prey_ids.end())
		{
			if(m_bIsKiller && m_pAgent->CollideWith(pAgent))
			{
				std::cout << "predator of type [" << m_pAgent->GetTypeId() << "] kill prey of type [" << pAgent->GetTypeId() << "]" <<  std::endl;
				pAgent->mark_for_destruction();
				pAgent->Explode(70, 3, 8);

				if(pAgent->GetZoneId()==GameEntity::ZONE_LAND)
				{
					Corpse* obs=new Corpse(m_pAgent->GetWorld(), m_corpse_scene_duration);
					obs->set_bound_radius(1.5);
					obs->SetModel("skull.3ds");
					obs->SetColor(pAgent->GetColor());
					obs->set_position(pAgent->get_position());
					
					m_pAgent->GetWorld()->AddObstacle(obs);
				}
			}
			else
			{
				if(m_pPursuitSteeringHandler != NULL)
				{
					double distance=(pAgent->get_position() - predator_position).length();
					if(current_prey == NULL)
					{
						current_prey=pAgent;	
						closest_distance=distance;
					}
					else if(closest_distance > distance)
					{
						current_prey=pAgent;
						closest_distance=distance;
					}
				}
			}
		}
	}

	if(m_pPursuitSteeringHandler != NULL)
	{
		m_pPursuitSteeringHandler->SetTarget(current_prey);
	}
}
