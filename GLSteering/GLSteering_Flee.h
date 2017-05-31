#ifndef _H_GL_STEERING_FLEE_H
#define _H_GL_STEERING_FLEE_H

#include "GLSteering.h"

class GLSteering_Flee : public GLSteering
{
public:
	GLSteering_Flee(Vehicle* pVehicle);
	virtual ~GLSteering_Flee();

public:
	virtual GLVector calculate();

protected:
	double m_dPanicDistance;
public:
	void SetPanicDistance(double distance) { m_dPanicDistance=distance; }

public:
	void SetTarget(const GLVector& vTarget) { m_vTarget=vTarget; }

protected:
	GLVector m_vTarget;
};
#endif