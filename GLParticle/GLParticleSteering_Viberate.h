#ifndef _H_GL_PARTICLE_STEERING_VIBERATE_H
#define _H_GL_PARTICLE_STEERING_VIBERATE_H

#include "GLParticleSteering.h"
#include <vector>

class GLParticleSteering_Viberate : public GLParticleSteering
{
public:
	GLParticleSteering_Viberate();
	GLParticleSteering_Viberate(double g);
	virtual ~GLParticleSteering_Viberate();

public:
	virtual GLVector GetAcceleration(const GLVector& particle_position, const GLVector& particle_velocity);

protected:
	double m_g;
};

#endif