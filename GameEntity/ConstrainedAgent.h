#ifndef _H_CONSTRAINED_AGENT_H
#define _H_CONSTRAINED_AGENT_H

#include "Vehicle.h"
#include "../GLShape/GLCurve.h"

class ConstrainedAgent : public Vehicle
{
public:
	ConstrainedAgent(GameWorld* pWorld);
	virtual ~ConstrainedAgent();

public:
	virtual void Update(const long& lElapsedTicks);

protected:
	void FollowClosestConstrainedAgent(const long& lElapsedTicks);

protected:
	GLCurve* m_path;
public:
	void SetPath(GLCurve* pCurve) { m_path=pCurve; }

public:
	bool IsConstrained() const { return m_path != NULL; }

protected:
	int m_iCurrent;
	double m_dCurrentDistance;
public:
	void reset();

public:
	void SetSpeed(double speed);
	virtual double GetSpeed() const;

protected:
	GLVector m_vTranslation;
public:
	GLVector GetTranslation() const { return m_vTranslation; }
};
#endif