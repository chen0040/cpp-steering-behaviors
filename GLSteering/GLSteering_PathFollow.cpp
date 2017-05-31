#include "GLSteering_PathFollow.h"
#include "../GameEntity/Vehicle.h"

GLSteering_PathFollow::GLSteering_PathFollow(Vehicle* pVehicle)
: GLSteering(pVehicle)
, m_pPath(NULL)
, m_target_deviation(1)
{
	m_type="path follow";
}

GLSteering_PathFollow::~GLSteering_PathFollow()
{

}

GLVector GLSteering_PathFollow::calculate()
{
	GLVector vForce;

	if(m_pPath->IsEnded())
	{
		return arrive(m_pPath->GetCurrent(), 0.9);
	}
	else
	{
		GLVector vTarget=m_pPath->GetCurrent();
		GLVector vTo=vTarget - m_pVehicle->get_position();
		if(vTo.lengthSq() < m_target_deviation)
		{
			m_pPath->Next();
		}
		//std::cout << "current way pont: " << m_pPath->GetCurrent().x << ", " << m_pPath->GetCurrent().y << ", " << m_pPath->GetCurrent().z << std::endl;

		return seek(m_pPath->GetCurrent());
	}
}

GLVector GLSteering_PathFollow::seek(const GLVector& vTarget)
{
	GLVector vTo=vTarget - m_pVehicle->get_position();
	GLVector vDesiredVelocity=vTo.Truncate(m_pVehicle->GetMaxSpeed());

	return vDesiredVelocity - m_pVehicle->GetVelocity();
}

GLVector GLSteering_PathFollow::arrive(const GLVector& vTarget, double deceleration)
{
	GLVector vTo=vTarget - m_pVehicle->get_position();
	double distance=vTo.length();
	if(distance > 0)
	{
		double speed=distance / (deceleration);
		speed=speed > m_pVehicle->GetMaxSpeed() ? m_pVehicle->GetMaxSpeed() : speed;
		GLVector vDesiredVelocity=vTo * speed / distance;
	}
	return GLVector(0, 0, 0);
}