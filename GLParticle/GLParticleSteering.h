#ifndef _H_GL_PARTICLE_STEERING_H
#define _H_GL_PARTICLE_STEERING_H

#include "../GLVector/GLVector.h"

class GLParticleSteering
{
public:
	GLParticleSteering();
	virtual ~GLParticleSteering() { }

public:
	virtual GLVector GetAcceleration(const GLVector& particle_position, const GLVector& particle_velocity)=0;
};
#endif