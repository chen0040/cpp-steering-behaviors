#ifndef _H_GL_STEERING_EVADE_H
#define _H_GL_STEERING_EVADE_H

#include "GLSteering.h"

class GLSteering_Evade : public GLSteering
{
public:
	GLSteering_Evade(Vehicle* pVehicle);
	virtual ~GLSteering_Evade();

public:
	virtual GLVector calculate();

protected:
	GLVector flee(const GLVector& vTarget);

protected:
	double m_dPanicDistance;
public:
	void SetPanicDistance(double distance) { m_dPanicDistance=distance; }

protected:
	Vehicle* m_pEvadeTarget;
public:
	void SetEvadeTarget(Vehicle* pVehicle) { m_pEvadeTarget=pVehicle; }
};
#endif