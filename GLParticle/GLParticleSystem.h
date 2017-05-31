#ifndef _H_GL_PARTICLE_SYSTEM_H
#define _H_GL_PARTICLE_SYSTEM_H

#include "GLEmitter.h"
#include <vector>

class GLParticleSystem
{
public:
	virtual ~GLParticleSystem();
	static GLParticleSystem* Instance();

public:
	virtual void Update(const long& lElapsedTicks);
	virtual void Render();

private:
	GLParticleSystem();
	GLParticleSystem(const GLParticleSystem& rhs) { }
	GLParticleSystem& operator=(const GLParticleSystem& rhs) { return *this; }

public:
	void AddEmitter(GLEmitter* pEmitter) { m_emitters.push_back(pEmitter); }

public:
	std::vector<GLEmitter*> m_emitters;
};

#define glParticleSys (*(GLParticleSystem::Instance()))
#endif