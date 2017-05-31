#include "GLParticleSteering_Gravity.h"

GLParticleSteering_Gravity::GLParticleSteering_Gravity()
: GLParticleSteering()
, m_g(7)
{

}

GLParticleSteering_Gravity::GLParticleSteering_Gravity(double g)
: GLParticleSteering()
, m_g(g)
{

}

GLParticleSteering_Gravity::~GLParticleSteering_Gravity()
{

}

GLVector GLParticleSteering_Gravity::GetAcceleration(const GLVector& particle_position, const GLVector& particle_velocity)
{
	GLVector v;
	v.y=-m_g;
	return v;
}
