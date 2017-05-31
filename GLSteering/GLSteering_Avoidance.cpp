#include "GLSteering_Avoidance.h"
#include "../GameEntity/Vehicle.h"
#include "../GameWorld/GameWorld.h"
#include <cfloat>
#include <cmath>
#include <iostream>
#include <set>

GLSteering_Avoidance::GLSteering_Avoidance(Vehicle* pVehicle)
: GLSteering(pVehicle)
, m_dMinBoxLength(13)
, m_braking_weight(0.6)
{
	m_type="avoidance";
}

GLVector GLSteering_Avoidance::calculate()
{
	GLVector vForce;
	double dBoxLength=m_dMinBoxLength + m_dMinBoxLength * (m_pVehicle->GetSpeed()) / m_pVehicle->GetMaxSpeed();

	m_pVehicle->tag_obstacles(dBoxLength);

	GameEntity* pClosestObs=NULL;
	GLVector vClosestObsLocalPosition;
	double closest_obs_distance=DBL_MAX;

	std::set<GameEntity*> obs;
	m_pVehicle->GetWorld()->GetNeighborObstacles(m_pVehicle, obs, dBoxLength);
	std::set<GameEntity*>::iterator pos_obs;
	for(pos_obs=obs.begin(); pos_obs != obs.end(); ++pos_obs)
	{
		GameEntity* pObs=*pos_obs;
		if(pObs->is_tagged())
		{
			//std::cout << "vehicle pos: [" << m_pVehicle->get_position().x << ", " << m_pVehicle->get_position().y << ", " << m_pVehicle->get_position().z << "]"\n";
			//std::cout << "vehicle heading: [" << m_pVehicle->GetHeading().x << ", " << m_pVehicle->GetHeading().y << ", " << m_pVehicle->GetHeading().z << "]"\n";
			//std::cout << "before transformation: [" << pObs->get_position().x << ", " << pObs->get_position().y << ", " << pObs->get_position().z << "]"\n";
			GLVector vLocalObsPosition=m_pVehicle->WorldToLocalCoordinate(pObs->get_position());
			//std::cout << "after transformation: [" << vLocalObsPosition.x << ", " << vLocalObsPosition.y << ", " << vLocalObsPosition.z << "]"\n";

			if(vLocalObsPosition.x >= 0)
			{
				double dExpandedRadius=m_pVehicle->GetBoundingSphere().radius + pObs->GetBoundingSphere().radius;
				if(abs(vLocalObsPosition.z) < dExpandedRadius)
				{
					double sqrt_part=sqrt(dExpandedRadius * dExpandedRadius - vLocalObsPosition.z * vLocalObsPosition.z);
					double distx=vLocalObsPosition.x - sqrt_part;
					if(distx <= 0)
					{
						distx=sqrt_part + vLocalObsPosition.x;
					}
					if(distx < closest_obs_distance)
					{
						closest_obs_distance=distx;
						vClosestObsLocalPosition=vLocalObsPosition;
						pClosestObs=pObs;
					}
				}
			}
		}
	}

	if(pClosestObs != NULL)
	{
		double multiplier=1.0 + (dBoxLength - vClosestObsLocalPosition.x) / dBoxLength;
		vForce.z=(pClosestObs->GetBoundingSphere().radius - vClosestObsLocalPosition.z) * multiplier;

		vForce.x=(pClosestObs->GetBoundingSphere().radius - vClosestObsLocalPosition.x) * m_braking_weight;

		vForce=m_pVehicle->LocalToWorldVector(vForce);
	}

	return vForce;
}

