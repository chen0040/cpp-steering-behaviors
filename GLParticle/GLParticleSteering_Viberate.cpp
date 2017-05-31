#include "GLParticleSteering_Viberate.h"
#include "../GLMath/GLMath.h"

GLParticleSteering_Viberate::GLParticleSteering_Viberate()
: GLParticleSteering()
, m_g(7)
{

}

GLParticleSteering_Viberate::GLParticleSteering_Viberate(double g)
: GLParticleSteering()
, m_g(g)
{

}

GLParticleSteering_Viberate::~GLParticleSteering_Viberate()
{

}

GLVector GLParticleSteering_Viberate::GetAcceleration(const GLVector& particle_position, const GLVector& particle_velocity)
{
	GLVector v;
	v.y=-m_g * glMath.nextDouble(-1, 1);
	return v;
}
