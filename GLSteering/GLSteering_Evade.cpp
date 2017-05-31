#include "GLSteering_Evade.h"
#include "../GameEntity/Vehicle.h"

GLSteering_Evade::GLSteering_Evade(Vehicle* pVehicle)
: GLSteering(pVehicle)
, m_dPanicDistance(30)
, m_pEvadeTarget(NULL)
{
	m_type="evade";
}

GLSteering_Evade::~GLSteering_Evade()
{

}

GLVector GLSteering_Evade::calculate()
{
	GLVector vForce;

	if(m_pEvadeTarget !=NULL)
	{
		GLVector vAway=m_pVehicle->get_position() - m_pEvadeTarget->get_position();

		double dist=vAway.length();

		double lookAheadTime=dist / (m_pVehicle->GetMaxSpeed() + m_pEvadeTarget->GetSpeed());

		vForce=flee(m_pEvadeTarget->get_position() + m_pEvadeTarget->GetVelocity() * lookAheadTime);
	}

	return vForce ;
}

GLVector GLSteering_Evade::flee(const GLVector& vTarget)
{
	GLVector vAway=m_pVehicle->get_position() - vTarget;

	GLVector vDesiredVelocity=vAway.Truncate(m_pVehicle->GetMaxSpeed());

	if(vAway.lengthSq() > m_dPanicDistance * m_dPanicDistance)
	{
		return GLVector(0, 0, 0);
	}

	return vDesiredVelocity - m_pVehicle->GetVelocity();
}
