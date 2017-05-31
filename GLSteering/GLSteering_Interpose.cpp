#include "GLSteering_Interpose.h"
#include "../GameEntity/Vehicle.h"

GLSteering_Interpose::GLSteering_Interpose(Vehicle* pVehicle)
: GLSteering(pVehicle)
, m_pTarget1(NULL)
, m_pTarget2(NULL)
{
	m_type="interpose";
}

GLVector GLSteering_Interpose::calculate()
{
	GLVector mid_point=(m_pTarget1->get_position() + m_pTarget2->get_position()) / 2;
	double distance=(mid_point - m_pVehicle->get_position()).length();
	double lookAheadTime=distance / m_pVehicle->GetMaxSpeed();
	
	GLVector vTarget1=m_pTarget1->get_position() + m_pTarget1->GetVelocity() * lookAheadTime;
	GLVector vTarget2=m_pTarget2->get_position() + m_pTarget2->GetVelocity() * lookAheadTime;

	mid_point=(vTarget1 + vTarget2) / 2.0;

	return arrive(mid_point, 0.3);

}

GLVector GLSteering_Interpose::arrive(const GLVector& vTarget, double deceleration)
{
	GLVector vTo=vTarget - m_pVehicle->get_position();

	double distance=vTo.length();

	if(distance > 0)
	{
		double speed=distance / deceleration;
		speed= speed > m_pVehicle->GetMaxSpeed() ? m_pVehicle->GetMaxSpeed() : speed;
		GLVector vDesiredVelocity=vTo * speed / distance;

		return vDesiredVelocity - m_pVehicle->GetVelocity();
	}

	return GLVector(0, 0, 0); 
}