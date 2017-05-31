#ifndef _H_GL_STEERING_WANDER_H
#define _H_GL_STEERING_WANDER_H

#include "GLSteering.h"

class GLSteering_Wander : public GLSteering
{
public:
	GLSteering_Wander(Vehicle* pVehicle);
	virtual ~GLSteering_Wander();

public:
	virtual GLVector calculate();

protected:
	GLVector m_vWanderTarget;
	double m_dWanderRadius;
	double m_dWanderDistance;
	double m_dWanderJitter;

public:
	void SetWanderRadius(double radius) { m_dWanderRadius=radius; }
	void SetWanderDistance(double distance) { m_dWanderDistance=distance; }
	void SetWanderJitter(double jitter) { m_dWanderJitter=jitter; }


protected:
	GLVector seek(const GLVector& vTarget);
};
#endif