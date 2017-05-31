#include "GLSteering_WallAvoidance.h"
#include "../GameEntity/Vehicle.h"
#include "../GLShape/GLPlane.h"
#include "../GLShape/GLLineSegment.h"
#include "../GameWorld/GameWorld.h"
#include <cmath>
#include <cfloat>

GLSteering_WallAvoidance::GLSteering_WallAvoidance(Vehicle* pVehicle)
: GLSteering(pVehicle)
, m_dFeelerLength(5)
{
	m_type="wall avoidance";
}

GLVector GLSteering_WallAvoidance::calculate()
{
	GLVector vForce;
	std::vector<GLLineSegment> feelers;
	CreateFeelers(feelers);

	std::vector<GLPlane> walls=m_pVehicle->GetWorld()->GetWalls();

	int tFeeler=-1;
	int tWall=-1;
	double min_distance=DBL_MAX;
	GLVector tPoint;

	int feeler_count=static_cast<int>(feelers.size());
	int wall_count=static_cast<int>(walls.size());

	for(int f=0; f<feeler_count; f++)
	{
		for(int w=0; w<wall_count; w++)
		{
			double distance;
			GLVector intersection_point;
			if(feelers[f].intersect(walls[w], intersection_point, distance))
			{
				if(distance < min_distance)
				{
					min_distance=distance;
					tPoint=intersection_point;
					tFeeler=f;
					tWall=w;
				}
			}
		}
		
	}

	if(tWall >= 0)
	{
		GLVector vOverShoot=feelers[tFeeler].to - tPoint;
		vForce=walls[tWall].normal * vOverShoot.length();

		/*
		std::cout << "tPoint: " << tPoint.toString().c_str() << std::endl;
		std::cout << "feelers.from: " << feelers[tFeeler].from.toString().c_str() << std::endl;
		std::cout << "feelers.to: " << feelers[tFeeler].to.toString().c_str() << std::endl;
		std::cout << "wall normal: " << walls[tWall].normal.toString().c_str() << std::endl;
		std::cout << "vForce: " << vForce.toString().c_str() << std::endl;
		*/
	}

	return vForce;
}

void GLSteering_WallAvoidance::CreateFeelers(std::vector<GLLineSegment>& feelers)
{
	GLLineSegment feeler1;
	feeler1.from=m_pVehicle->get_position();
	feeler1.to=m_pVehicle->get_position() + m_dFeelerLength * m_pVehicle->GetHeading();
	feelers.push_back(feeler1);

	GLLineSegment feeler2;
	feeler2.from=m_pVehicle->get_position();
	feeler2.to=m_pVehicle->get_position() + m_dFeelerLength * (m_pVehicle->GetHeading() + m_pVehicle->GetSide()).normalize();
	feelers.push_back(feeler2);

	GLLineSegment feeler3;
	feeler3.from=m_pVehicle->get_position();
	feeler3.to=m_pVehicle->get_position() + m_dFeelerLength * (m_pVehicle->GetHeading() - m_pVehicle->GetSide()).normalize();
	feelers.push_back(feeler3);
}