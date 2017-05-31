#include "GLSteering_OffsetPursuit.h"
#include "../GameEntity/Vehicle.h"

GLSteering_OffsetPursuit::GLSteering_OffsetPursuit(Vehicle* pVehicle)
: GLSteering(pVehicle)
, m_pTarget(NULL)
{
	m_type="offset pursuit";
}

GLVector GLSteering_OffsetPursuit::calculate()
{
	GLVector vForce;

	GLVector vWorldOffset=m_pTarget->LocalToWorldCoordinate(m_offset);

	GLVector vTo=vWorldOffset - m_pVehicle->get_position();

	double distance=vTo.length();

	double lookAheadTime=distance / (m_pVehicle->GetMaxSpeed() + m_pTarget->GetSpeed());

	vForce=arrive(m_pVehicle->get_position() + m_pTarget->GetVelocity() * lookAheadTime, 0.9);

	return vForce;
}

GLVector GLSteering_OffsetPursuit::arrive(const GLVector& vTarget, double deceleration)
{
	GLVector vForce(0, 0, 0);

	GLVector vTo=vTarget - m_pVehicle->get_position();

	double distance=vTo.length();

	if(distance > 0)
	{
		double speed=distance / deceleration;

		speed=speed > m_pVehicle->GetMaxSpeed() ? m_pVehicle->GetMaxSpeed() : speed;

		GLVector vDesireVelocity=vTo * speed / distance;

		vForce=vDesireVelocity - m_pVehicle->GetVelocity();
	}

	return vForce;
}