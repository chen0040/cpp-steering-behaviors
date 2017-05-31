#ifndef _H_GL_DEBRIS_H
#define _H_GL_DEBRIS_H

#include "GLParticle.h"

class GLDebris : public GLParticle
{
public:
	GLDebris(GLParticleSteering* pSteering);
	virtual ~GLDebris();

public:
	virtual void Render();
	virtual void Update(const long& lElapsedTicks);

public:
	GLVector m_vOrientation;
	GLVector m_vOrientationVelocity;
	GLVector m_vScale;
};
#endif