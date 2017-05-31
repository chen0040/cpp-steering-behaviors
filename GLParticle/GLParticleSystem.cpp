#include "GLParticleSystem.h"
#include <iostream>

GLParticleSystem::GLParticleSystem()
{

}

void GLParticleSystem::Update(const long& lElapsedTicks)
{
	std::vector<GLEmitter*>::iterator pos=m_emitters.begin();
	while(pos != m_emitters.end())
	{
		if((*pos)->IsDead())
		{
			delete (*pos);
			pos=m_emitters.erase(pos);
		}
		else
		{
			(*pos)->Update(lElapsedTicks);
			++pos;
		}
	}
}

void GLParticleSystem::Render()
{
	std::vector<GLEmitter*>::iterator pos;
	for(pos=m_emitters.begin(); pos != m_emitters.end(); ++pos)
	{
		(*pos)->Render();
	}
}

GLParticleSystem::~GLParticleSystem()
{
	std::vector<GLEmitter*>::iterator pos;
	for(pos=m_emitters.begin(); pos != m_emitters.end(); ++pos)
	{
		delete (*pos);
	}
}

GLParticleSystem* GLParticleSystem::Instance()
{
	static GLParticleSystem theInstance;
	return &theInstance;
}

