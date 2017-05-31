#include "GLEmitter.h"
#include "../GLMath/GLMath.h"

GLEmitter::GLEmitter()
{

}

GLEmitter::~GLEmitter()
{
	std::vector<GLParticle*>::iterator pos;
	for(pos=m_particles.begin(); pos != m_particles.end(); ++pos)
	{
		delete (*pos);
	}
}

void GLEmitter::Update(const long& lElapsedTicks)
{
	std::vector<GLParticle*>::iterator pos=m_particles.begin();
	while(pos != m_particles.end())
	{
		if((*pos)->IsDead())
		{
			delete (*pos);
			pos=m_particles.erase(pos);
		}
		else
		{
			(*pos)->Update(lElapsedTicks);
			++pos;
		}
	}
}

void GLEmitter::Render()
{
	std::vector<GLParticle*>::const_iterator pos;
	for(pos=m_particles.begin(); pos != m_particles.end(); ++pos)
	{
		(*pos)->Render();
	}
}

void GLEmitter::AddParticle(GLParticle* pParticle)
{
	m_particles.push_back(pParticle);
}

bool GLEmitter::IsDead() const
{
	std::vector<GLParticle*>::const_iterator pos;
	for(pos=m_particles.begin(); pos != m_particles.end(); ++pos)
	{
		if(!(*pos)->IsDead())
		{
			return false;
		}
	}
	return true;
}