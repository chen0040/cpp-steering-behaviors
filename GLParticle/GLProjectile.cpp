#include "GLProjectile.h"
#include "GLBroadcaster.h"
#include "GLParticleSystem.h"

GLProjectile::GLProjectile(double life)
: GLEmitter()
, m_broadcast_period(100)
, m_broadcast_counter(0)
, m_radius(4)
, m_life_span(life)
{

}

GLProjectile::~GLProjectile()
{

}

void GLProjectile::set_broadcast_period(int period)
{
	m_broadcast_period=period;
	m_broadcast_counter=period;
}

void GLProjectile::Update(const long& lElapsedTicks)
{
	if(IsDead())
	{
		return;
	}

	double elapsed_time=static_cast<double>(lElapsedTicks) / 1000.0;
	m_broadcast_counter+=lElapsedTicks;

	if(m_broadcast_counter >= m_broadcast_period)
	{
		m_broadcast_counter=0;
		GLBroadcaster* transmitter=new GLBroadcaster();
		transmitter->m_vPosition=this->m_vPosition;
		transmitter->Broadcast(300, 1, 50, m_radius);

		glParticleSys.AddEmitter(transmitter);
	}

	m_vPosition+=m_vVelocity * elapsed_time;

	m_life_span -= elapsed_time;
}

bool GLProjectile::IsDead() const
{
	return m_life_span > 0;
}