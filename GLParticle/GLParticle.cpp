#include "GLParticle.h"
#include "GLParticleSteering.h"
#include "../GLExtension/glxs.h"
#include <ctime>
#include <glut.h>

GLParticle::GLParticle(GLParticleSteering* pSteering)
: m_dInitialSpeed(4)
, m_life(2)
, m_pSteering(pSteering)
{
	
}

GLParticle::~GLParticle()
{
	if(m_pSteering != NULL)
	{
		delete m_pSteering;
		m_pSteering=NULL;
	}
}

void GLParticle::Update(const long& lElapsedTicks)
{
	if(this->IsDead())
	{
		return;
	}

	double elapsed_time=static_cast<double>(lElapsedTicks) / static_cast<double>(CLOCKS_PER_SEC);
	GLVector vAccel;
	if(m_pSteering != NULL)
	{
		vAccel=m_pSteering->GetAcceleration(m_vPosition, m_vVelocity);
	}

	m_vVelocity+=vAccel * elapsed_time;
	m_vPosition+=m_vVelocity * elapsed_time;

	m_life -= elapsed_time;
}
