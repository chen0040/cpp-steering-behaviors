#ifndef _H_GL_EMITTER_H
#define _H_GL_EMITTER_H

#include <vector>
#include "GLParticle.h"

class GLEmitter 
{
public:
	GLEmitter();
	virtual ~GLEmitter();

public:
	virtual void Update(const long& lElapsedTicks);
	virtual void Render();

public:
	virtual void AddParticle(GLParticle* pParticle);

public:
	virtual bool IsDead() const;

public:
	GLVector m_vPosition;
	GLVector m_vVelocity;

protected:
	std::vector<GLParticle*> m_particles;
};
#endif