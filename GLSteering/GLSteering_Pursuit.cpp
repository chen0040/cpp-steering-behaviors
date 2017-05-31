#include "GLSteering_Pursuit.h"
#include "../GameEntity/Vehicle.h"
#include <iostream>

GLSteering_Pursuit::GLSteering_Pursuit(Vehicle* pVehicle)
: GLSteering(pVehicle)
, m_pTarget(NULL)
{
	m_type="pursuit";
}

GLSteering_Pursuit::~GLSteering_Pursuit()
{

}

GLVector GLSteering_Pursuit::calculate()
{
	GLVector vForce;

	if(m_pTarget == NULL || m_pTarget->is_marked_for_destruction())
	{
		return vForce;
	}

	GLVector vTo=m_pTarget->get_position() - m_pVehicle->get_position();

	double relativeHeading=m_pVehicle->GetVelocity().dot(m_pTarget->GetVelocity());

	if(vTo.dot(m_pVehicle->GetVelocity()) > 0 && relativeHeading < -0.98)
	{
		return vForce;
	}

	double dist=vTo.length();

	double lookAtTime=dist / (m_pVehicle->GetMaxSpeed() + m_pTarget->GetSpeed());

	vForce=seek(m_pTarget->get_position() + m_pTarget->GetVelocity() * lookAtTime);

	return vForce ;
}

GLVector GLSteering_Pursuit::seek(const GLVector& vTarget)
{
	GLVector vForce;
	GLVector vTo=vTarget - m_pVehicle->get_position();

	GLVector vDesiredVelocity=vTo.Truncate(m_pVehicle->GetMaxSpeed());

	vForce=vDesiredVelocity - m_pVehicle->GetVelocity();

	return vForce;
}