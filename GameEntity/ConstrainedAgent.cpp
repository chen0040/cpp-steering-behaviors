#include "ConstrainedAgent.h"
#include "../GameWorld/GameWorld.h"
#include "../GLModel/GLModelManager.h"
#include "../GLUtil/GLExitSystem.h"
#include "../GLMath/GLMath.h"
#include <cassert>
#include <cfloat>

ConstrainedAgent::ConstrainedAgent(GameWorld* pWorld)
: Vehicle(pWorld)
, m_iCurrent(0)
, m_path(NULL)
, m_dCurrentDistance(0)
{
	EnableWorldWrap(false);	
}

ConstrainedAgent::~ConstrainedAgent()
{
	
}

void ConstrainedAgent::FollowClosestConstrainedAgent(const long& lElapsedTicks)
{
	ConstrainedAgent* pClosestConstrainedAgent=NULL;
	double min_distance=DBL_MAX;

	const AgentGroup& agents=m_pWorld->GetConstAgents();
	AgentGroup::const_iterator pos_agent;
	for(pos_agent=agents.begin(); pos_agent != agents.end(); ++pos_agent)
	{
		ConstrainedAgent* pConstrainedAgent=static_cast<ConstrainedAgent*>(*pos_agent);
		if(pConstrainedAgent != NULL && pConstrainedAgent != this && pConstrainedAgent->IsConstrained())
		{
			double distance=(pConstrainedAgent->get_position() - m_position).length();
			if(distance < min_distance)
			{
				min_distance=distance;
				pClosestConstrainedAgent=pConstrainedAgent;
			}
		}
	}

	if(pClosestConstrainedAgent != NULL)
	{
		m_vTranslation=pClosestConstrainedAgent->GetTranslation();
		m_vHeading=pClosestConstrainedAgent->GetHeading();
		m_vSide=pClosestConstrainedAgent->GetSide();
		m_vVelocity=pClosestConstrainedAgent->GetVelocity();
		SetSpeed(pClosestConstrainedAgent->GetSpeed());

		m_position+=m_vTranslation;

		UpdateFPS(GetSpeed());

		if(m_vTranslation.length() != 0)
		{	
			m_animator.Update(lElapsedTicks);
		}

		this->UpdateRoleAction(lElapsedTicks);
	}
}

void ConstrainedAgent::Update(const long &lElapsedTicks)
{
	MovingEntity::Update(lElapsedTicks);

	if(!IsConstrained())
	{
		FollowClosestConstrainedAgent(lElapsedTicks);
		return;
	}

	if(m_path->IsNull())
	{
		reset();
		return;
	}

	double elapsed_time=(double)lElapsedTicks / 1000.0;

	double ds=elapsed_time * GetSpeed();

	double s2=m_dCurrentDistance+ds;

	int iNext=m_iCurrent;
	while(! m_path->ReachEnd(++iNext))
	{
		double distance=m_path->GetDistance(iNext);
		if(distance > s2)
		{
			iNext;
			break;
		}
	}

	if(!m_path->ReachEnd(iNext))
	{
		m_iCurrent=iNext-1;

		double s1=m_path->GetDistance(m_iCurrent);
		double sg=s2 - s1;

		GLVector p1=m_path->GetNode(m_iCurrent);
		GLVector p2=m_path->GetNode(iNext);
		p1.y=0;
		p2.y=0;
		GLVector vTo=p2 - p1;

		GLVector prev_position=m_position;
		m_position=p1 + vTo.Truncate(sg);

		m_vTranslation=m_position - prev_position;

		m_vHeading=m_vTranslation.normalize();
		m_vSide=GLVector(-m_vHeading.z, m_vHeading.y, m_vHeading.x);

		m_vVelocity=m_vHeading * GetSpeed();

		m_dCurrentDistance=s2;

		UpdateFPS(GetSpeed());

		m_animator.Update(lElapsedTicks);
	}
	else
	{
		reset();
	}

	this->UpdateRoleAction(lElapsedTicks);
}

void ConstrainedAgent::SetSpeed(double speed)
{
	SetMaxSpeed(speed);
}

double ConstrainedAgent::GetSpeed() const
{
	return GetMaxSpeed();
}

void ConstrainedAgent::reset()
{
	m_iCurrent=0; 
	m_dCurrentDistance=0; 
	m_vTranslation.reset();
}