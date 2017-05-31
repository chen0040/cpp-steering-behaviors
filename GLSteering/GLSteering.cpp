#include "GLSteering.h"
#include "../GameEntity/Vehicle.h"

GLSteering::GLSteering(Vehicle* pVehicle)
: m_pVehicle(pVehicle)
, m_dWeight(1.0)
, m_bOn(false)
{
	
}

GLSteering::~GLSteering()
{

}

bool GLSteering::is_a(const char* name)
{
	if(m_type.compare(name)==0)
	{
		return true;
	}
	return false;
}