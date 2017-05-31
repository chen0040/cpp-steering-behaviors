#include "GLLocomotion_Aircraft.h"
#include "../GameEntity/Vehicle.h"
#include "../GLMath/GLMath.h"

GLLocomotion_Aircraft::GLLocomotion_Aircraft(Vehicle* pAgent)
: GLLocomotion(pAgent)
, m_dy(0)
{

}

GLLocomotion_Aircraft::~GLLocomotion_Aircraft()
{

}

void GLLocomotion_Aircraft::Update(const long& lElapsedTicks)
{
	const double yvar=0.1;
	m_dy+=glMath.nextDouble() * (yvar * 2) - yvar;
	if(m_dy > yvar) m_dy=yvar;
	if(m_dy < -yvar) m_dy=-yvar;

	GLVector position=m_pAgent->get_position();
	position.y=m_pAgent->GetHoverHeight()+m_dy;
	m_pAgent->set_position(position);

	this->EnforceNonPenetration();
}