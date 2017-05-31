#include "GLSteering_Wander.h"
#include "../GameEntity/Vehicle.h"
#include "../GLMath/GLMath.h"
#include <cmath>

GLSteering_Wander::GLSteering_Wander(Vehicle* pVehicle)
: GLSteering(pVehicle)
, m_dWanderRadius(1.0)
, m_dWanderDistance(1.0)
, m_dWanderJitter(0.1)
{
	m_type="wander";
	double rad=glMath.nextDouble(0, glMath.PI() * 2);
	m_vWanderTarget.x=m_dWanderRadius * cos(rad);
	m_vWanderTarget.z=m_dWanderRadius * sin(rad);
	m_vWanderTarget.y=0;
}

GLSteering_Wander::~GLSteering_Wander()
{

}

GLVector GLSteering_Wander::calculate()
{
	GLVector vForce;

	m_vWanderTarget+=GLVector(m_dWanderJitter * glMath.nextDouble(-1, 1), 0, m_dWanderJitter * glMath.nextDouble(-1, 1));
	m_vWanderTarget = m_vWanderTarget.normalize() * m_dWanderRadius;

	GLVector vTargetLocal=m_vWanderTarget + GLVector(m_dWanderDistance, 0, 0);

	GLVector vTargetWorld=m_pVehicle->LocalToWorldCoordinate(vTargetLocal);

	vForce=vTargetWorld - m_pVehicle->get_position();
	
	return vForce;
}

GLVector GLSteering_Wander::seek(const GLVector& vTarget)
{
	GLVector vTo=vTarget - m_pVehicle->get_position();
	GLVector vDesiredVelocity=vTo.Truncate(m_pVehicle->GetMaxSpeed());

	return vDesiredVelocity - m_pVehicle->GetVelocity();
}