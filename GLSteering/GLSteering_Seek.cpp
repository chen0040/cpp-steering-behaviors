#include "GLSteering_Seek.h"
#include "../GameEntity/Vehicle.h"

GLSteering_Seek::GLSteering_Seek(Vehicle* pVehicle)
: GLSteering(pVehicle)
{
	m_type="seek";
}

GLSteering_Seek::~GLSteering_Seek()
{

}

GLVector GLSteering_Seek::calculate()
{
	GLVector vTo=m_vTarget - m_pVehicle->get_position();


	GLVector vDesiredVelocity=vTo.Truncate(m_pVehicle->GetMaxSpeed());


	GLVector vForce=vDesiredVelocity - m_pVehicle->GetVelocity();

	return vForce ;
}
