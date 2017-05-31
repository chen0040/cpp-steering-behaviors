#ifndef _H_GL_PARTICLE_STEERING_GRAVITY_H
#define _H_GL_PARTICLE_STEERING_GRAVITY_H

#include "GLParticleSteering.h"
#include <vector>

class GLParticleSteering_Gravity : public GLParticleSteering
{
public:
	GLParticleSteering_Gravity();
	GLParticleSteering_Gravity(double g);
	virtual ~GLParticleSteering_Gravity();

public:
	virtual GLVector GetAcceleration(const GLVector& particle_position, const GLVector& particle_velocity);

protected:
	double m_g;
};

#endif