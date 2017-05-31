#include "GLSteering_Flee.h"
#include "../GameEntity/Vehicle.h"
 
GLSteering_Flee::GLSteering_Flee(Vehicle* pVehicle)
: GLSteering(pVehicle)
, m_dPanicDistance(30)
{
	m_type="flee";
}

GLSteering_Flee::~GLSteering_Flee()
{

}

GLVector GLSteering_Flee::calculate()
{
	GLVector vForce;

	GLVector vAway=m_pVehicle->get_position() - m_vTarget;

	if(vAway.lengthSq() > m_dPanicDistance * m_dPanicDistance)
	{
		return vForce;
	}

	GLVector vDesiredVelocity=vAway.Truncate(m_pVehicle->GetMaxSpeed());

	vForce=vDesiredVelocity - m_pVehicle->GetVelocity();

	return vForce ;

}