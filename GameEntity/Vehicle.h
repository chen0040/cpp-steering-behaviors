#ifndef _H_VEHICLE_H
#define _H_VEHICLE_H

#include "MovingEntity.h"
#include "../GLSteering/GLSteering_Composite.h"
#include "../GLAnimation/GLAnimator.h"
#include "../MD2Loader/GLModel_MD2.h"
#include "../GLLocomotion/GLLocomotion.h"
#include <string>

class Vehicle : public MovingEntity
{
public:
	Vehicle(GameWorld* pWorld);
	virtual ~Vehicle();

public:
	virtual void Update(const long& lElapsedTicks);
	virtual void Render();

public:
	GLSteering* GetSteering(std::string sname);
	void AddSteering(const char* sname, GLSteering* pSteering);
	void RemoveSteering(const char* sname);

public:
	GLVector LocalToWorldCoordinate(const GLVector& vLocalTarget) const;
	GLVector WorldToLocalCoordinate(const GLVector& vWorldTarget) const;
	GLVector LocalToWorldVector(const GLVector& vLocalTarget) const;

public:
	void SetModel(const char* fname);

protected:
	void UpdateFPS(double speed);
	
protected:
	GLAnimator m_animator;
public:
	GLAnimator& GetAnimator() { return m_animator; }

protected:
	GLSteering_Composite* m_pSteering;
public:
	GLSteering_Composite* GetSteering() { return m_pSteering; }

protected:
	GLLocomotion* m_pLocomotion;
public:
	void SetLocomotion(GLLocomotion* pLocomotion);

public:
	virtual bool IsPenetratable(GameEntity* pAgent) const;

protected:
	double m_dVisibleDistance;
public:
	double GetVisibleDistance() const { return m_dVisibleDistance; }
	void SetVisibleDistance(double distance) { m_dVisibleDistance=distance; }

protected:
	double m_dHeight;
public:
	void SetHoverHeight(double height) { m_dHeight=height; }
	double GetHoverHeight() const { return m_dHeight; }

public:
	void tag_neighbors(double r);
	void tag_obstacles(double scan_radius);

public:
	void EnableSound(bool bEnabled) { m_animator.EnableSound(bEnabled); }
};
#endif