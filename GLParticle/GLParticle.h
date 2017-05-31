#ifndef _H_GL_PARTICLE_H
#define _H_GL_PARTICLE_H

#include "../GLVector/GLVector.h"

class GLParticleSteering;

class GLParticle
{
public:
	GLParticle(GLParticleSteering* pSteering);
	virtual ~GLParticle();

public:
	virtual void Render() { }
	virtual void Update(const long& lElapsedTicks);

public:
	GLVector m_vPosition;
	GLVector m_vVelocity;
	GLVector m_vColor;
	double m_life;
	double m_dInitialSpeed;

public:
	virtual bool IsDead() const { return m_life < 0; }

public:
	GLParticleSteering* m_pSteering;
};
#endif