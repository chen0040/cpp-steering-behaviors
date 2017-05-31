#include "MovingEntity.h"
#include "../GameWorld/GameWorld.h"
#include "../GLMath/GLMath.h"
#include <cmath>
#include <iostream>

MovingEntity::MovingEntity(GameWorld* pWorld)
: GameEntity(pWorld)
, m_dMass(0.01)
, m_dMaxForce(6.0)
, m_dMaxSpeed(150)
, m_vHeading(1, 0, 0)
, m_vSide(0, 0, 1)
, m_iHeadingHistoryCount(20)
, m_bWorldWrap(true)
{

}

MovingEntity::~MovingEntity()
{

}

void MovingEntity::Render()
{
	GameEntity::Render();
}

void MovingEntity::PreRender()
{
	GameEntity::PreRender();

	GLVector vHeading=m_vHeading;
	if(IsSmoothHeadingEnabled())
	{
		vHeading=m_vSmoothHeading;
	}

	GLVector original_axis=GLVector(0.0, 0.0, 1.0);
	GLVector rotation_axis=original_axis.cross(vHeading);
	double Sin=rotation_axis.length();
	double Cos=vHeading.dot(original_axis);

	double rad=atan2(Sin, Cos);
	double angle_in_degree=glMath.toDegree(rad);

	rotation_axis=rotation_axis.normalize();

	glPushMatrix();

	glRotated(angle_in_degree, rotation_axis.x, rotation_axis.y, rotation_axis.z);
}

void MovingEntity::PostRender()
{
	glPopMatrix();
	GameEntity::PostRender();
}

void MovingEntity::Update(const long &lElapsedTicks)
{
	GameEntity::Update(lElapsedTicks);

	if(m_iHeadingHistoryCount > 0)
	{
		UpdateSmoothHeading();
	}

	if(m_bWorldWrap)
	{
		m_pWorld->WorldWrap(this);
	}
}

void MovingEntity::UpdateSmoothHeading()
{
	while(m_vHeadings.size() >= m_iHeadingHistoryCount && (!m_vHeadings.empty()))
	{
		m_vHeadings.pop_front();
	}
	m_vHeadings.push_back(m_vHeading);
	std::list<GLVector>::iterator pos;
	m_vSmoothHeading.reset();
	for(pos=m_vHeadings.begin(); pos != m_vHeadings.end(); ++pos)
	{
		m_vSmoothHeading+=(*pos);
	}
	m_vSmoothHeading /= m_iHeadingHistoryCount;
}