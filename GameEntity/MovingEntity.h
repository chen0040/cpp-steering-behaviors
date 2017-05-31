#ifndef _H_MOVING_ENTITY_H
#define _H_MOVING_ENTITY_H

#include "GameEntity.h"
#include <list>

class MovingEntity : public GameEntity
{
public:
	MovingEntity(GameWorld* pWorld);
	virtual ~MovingEntity();

public:
	virtual void Update(const long& lElapsedTicks);
	virtual void Render();
	virtual void PreRender();
	virtual void PostRender();

protected:
	double m_dMaxForce;
public:
	void SetMaxForce(double force) { m_dMaxForce=force; }
	double GetMaxForce() const { return m_dMaxForce; }

protected:
	double m_dMaxSpeed;
public:
	void SetMaxSpeed(double speed) { m_dMaxSpeed=speed; }
	double GetMaxSpeed() const { return m_dMaxSpeed; }

protected:
	bool m_bWorldWrap;
public:
	void EnableWorldWrap(bool bEnabled) { m_bWorldWrap=bEnabled; }

protected:
	double m_dMass;
public:
	void SetMass(double mass) { m_dMass=mass; }
	double GetMass() const { return m_dMass; }

protected:
	GLVector m_vVelocity;
	GLVector m_vHeading;
	GLVector m_vSide;

public:
	GLVector GetVelocity() const { return m_vVelocity; }
	GLVector GetHeading() const { return m_vHeading; }
	GLVector GetSide() const { return m_vSide; }
	virtual double GetSpeed() const { return m_vVelocity.length(); }
	void SetVelocity(GLVector vVelocity) { m_vVelocity=vVelocity; }

public:
	void EnableSmoothHeading(int iCount) { m_iHeadingHistoryCount=iCount; }
	void DisableSmoothHeading() { m_iHeadingHistoryCount=-1; }
	bool IsSmoothHeadingEnabled() const { return m_iHeadingHistoryCount > 0; }

protected:
	std::list<GLVector> m_vHeadings;
	unsigned int m_iHeadingHistoryCount;
	GLVector m_vSmoothHeading;

protected:
	void UpdateSmoothHeading();
};
#endif