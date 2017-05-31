#include "GLBroadcaster.h"
#include "../GameEntity/GameEntity.h"
#include "GLPacket.h"
#include "../GLMath/GLMath.h"
#include <iostream>

GLBroadcaster::GLBroadcaster()
: GLEmitter()
{
	
}

GLBroadcaster::~GLBroadcaster()
{
}

void GLBroadcaster::Render()
{
	std::vector<GLParticle*>::const_iterator pos;
	
	glEnable(GL_COLOR_MATERIAL);
	glColor3f(1, 1, 1);
	glBegin (GL_POINTS);
	for(pos=m_particles.begin(); pos != m_particles.end(); ++pos)
	{
		const GLVector& pp=(*pos)->m_vPosition;
		glVertex3d(pp.x, pp.y, pp.z);
	}
	glEnd();
	glDisable(GL_COLOR_MATERIAL);
}

void GLBroadcaster::Update(const long& lElapsedTicks)
{
	if(IsDead())
	{
		return;
	}

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

void GLBroadcaster::Broadcast(int count, double packet_travelling_distance, double packet_travelling_speed, double initial_radius)
{
	GLVector controller_color(1, 1, 1);
	GLVector vVector;
	double packet_travelling_life_span=packet_travelling_distance / packet_travelling_speed;
	for(int i=0; i<count; ++i)
	{
		GLPacket* particle=new GLPacket();
		particle->m_dInitialSpeed=packet_travelling_speed;
		particle->m_life=packet_travelling_life_span;
		particle->m_vColor=controller_color+GLVector(glMath.nextDouble(0, 0.1), glMath.nextDouble(0, 0.1), glMath.nextDouble(0, 0.1));

		vVector.x=glMath.nextDouble(-1, 1);
		vVector.y=glMath.nextDouble(-1, 1);
		vVector.z=glMath.nextDouble(-1, 1);
		particle->m_vVelocity=vVector.normalize() * packet_travelling_speed;

		vVector.x=glMath.nextDouble(-1, 1);
		vVector.y=glMath.nextDouble(-1, 1);
		vVector.z=glMath.nextDouble(-1, 1);
		particle->m_vPosition=this->m_vPosition + vVector.normalize() * initial_radius * 0.3;

		this->AddParticle(particle);
	}
}
