#include "GLBurstWave.h"
#include "../GameEntity/GameEntity.h"
#include "GLParticle.h"
#include "GLParticleSteering_Viberate.h"
#include "../GLMath/GLMath.h"
#include <iostream>

GLBurstWave::GLBurstWave(double life_span)
: GLEmitter()
, m_life(static_cast<int>(life_span*1000))
, m_broadcast_period(500)
, m_broadcast_counter(500)
, m_broadcast_count_per_wave(300)
, m_packet_travelling_life_span(0)
, m_packet_travelling_speed(0)
, m_core_radius(2)
{
	
}

GLBurstWave::~GLBurstWave()
{
}

void GLBurstWave::Render()
{
	glEnable(GL_COLOR_MATERIAL);
	glColor3f(1, 1, 1);
	std::vector<GLParticle*>::const_iterator pos;
	glBegin(GL_POINTS);
	for(pos=m_particles.begin(); pos != m_particles.end(); ++pos)
	{
		const GLVector& p1=(*pos)->m_vPosition;
		float life=static_cast<float>((*pos)->m_life / m_packet_travelling_life_span);
		if(life > 0.9)
		{
			glColor3f(life , life, (1-life) * life); //(1-life) * (1-life) * life
		}
		else if(life > 0.8)
		{
			glColor3f(life, life*life, 0);
		}
		else if(life >0.7)
		{
			glColor3f(0, life*life*life, 0);
		}
		else
		{
			glColor3f(life , life, (1-life) * life);
		}
		glVertex3d(p1.x, p1.y, p1.z);
	}
	glEnd();
	glColor3f(1, 1, 1);
	glDisable(GL_COLOR_MATERIAL);
}

void GLBurstWave::Update(const long& lElapsedTicks)
{
	if(IsDead())
	{
		return;
	}
	
	m_broadcast_counter+=lElapsedTicks;

	if(m_broadcast_counter >= m_broadcast_period)
	{
		m_broadcast_counter=0;
		Broadcast();
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

	m_life-=lElapsedTicks;
}

void GLBurstWave::SetBroadcastFrequency(double frequency)
{
	m_broadcast_period=static_cast<int>(1000 / frequency);
	m_broadcast_counter=m_broadcast_period;
}

void GLBurstWave::SetBroadcastCountPerWave(int count)
{
	m_broadcast_count_per_wave=count;
}

void GLBurstWave::Broadcast()
{
	GLVector vVector;
	for(int i=0; i<m_broadcast_count_per_wave; ++i)
	{
		GLParticle* particle=NULL;
		if(glMath.nextDouble() < 0.6)
		{
			particle=new GLParticle(new GLParticleSteering_Viberate(7));
			particle->m_life=m_packet_travelling_life_span * 0.77;
			vVector.x=glMath.nextDouble(-1, 1);
			vVector.y=glMath.nextDouble(-1, 1);
			vVector.z=glMath.nextDouble(-1, 1);
			particle->m_vVelocity=vVector.normalize() * m_packet_travelling_speed;

			vVector.x=glMath.nextDouble(-1, 1);
			vVector.y=glMath.nextDouble(0, 1);
			vVector.z=glMath.nextDouble(-1, 1);
			particle->m_vPosition=this->m_vPosition + vVector.normalize() * m_core_radius * 1.5;
		}
		else
		{
			particle=new GLParticle(NULL);
			vVector.x=glMath.nextDouble(-1, 1);
			vVector.y=0;
			vVector.z=glMath.nextDouble(-1, 1);
			particle->m_vVelocity=vVector.normalize() * m_packet_travelling_speed;

			vVector.x=glMath.nextDouble(-1, 1);
			vVector.y=0;
			vVector.z=glMath.nextDouble(-1, 1);
			vVector=vVector.normalize();
			vVector.y=0.5;
			particle->m_vPosition=this->m_vPosition + vVector * m_core_radius * 1.5;
		}

		particle->m_dInitialSpeed=m_packet_travelling_speed;

		this->AddParticle(particle);
	}
}

void GLBurstWave::SetPacketProperty(double packet_travelling_distance, double packet_travelling_speed)
{
	m_packet_travelling_life_span=packet_travelling_distance / packet_travelling_speed;
	m_packet_travelling_speed=packet_travelling_speed;
}