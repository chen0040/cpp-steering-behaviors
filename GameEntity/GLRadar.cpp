#include "GLRadar.h"
#include "../GameWorld/GameWorld.h"
#include "GameEntity.h"
#include "../GLFont/GLFontSys.h"
#include <glut.h>
#include <sstream>
#include "../GLMath/GLMath.h"
#include <cmath>
#include "../GLExtension/glxs.h"
#include <glut.h>
#include "../GLParticle/GLParticleSystem.h"
#include "../GLParticle/GLBroadcaster.h"

GLRadar::GLRadar(GameWorld* pWorld)
: Obstacle(pWorld)
, m_max_radius(20)
, m_radius(5)
, m_scan_speed(30)
{
	m_Height2WidthRatio=1.9;
}

void GLRadar::SetMaxScanRadius(double r)
{
	m_max_radius=r;
}

void GLRadar::SetScanSpeed(double s)
{
	m_scan_speed=s;
}

GLRadar::~GLRadar()
{
	
}

void GLRadar::Update(const long& lElapsedTicks)
{
	m_radius+=m_scan_speed * lElapsedTicks / 1000;

	if(m_radius >= m_max_radius)
	{
		m_radius=5;
	}

	GLBroadcaster* transmitter=new GLBroadcaster();
	transmitter->m_vPosition=this->m_position+m_transmission_point;
	transmitter->Broadcast(300, 1, 50, m_radius);

	glParticleSys.AddEmitter(transmitter);

}

void GLRadar::SetTransmisionPoint(const GLVector& pt)
{
	m_transmission_point=pt;
}

