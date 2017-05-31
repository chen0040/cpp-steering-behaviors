#ifndef _H_GL_STEERING_INTERPOSE_H
#define _H_GL_STEERING_INTERPOSE_H

#include "GLSteering.h"

class GLSteering_Interpose : public GLSteering
{
public:
	GLSteering_Interpose(Vehicle* pVehicle);
	virtual ~GLSteering_Interpose() { }

public:
	virtual GLVector calculate();

protected:
	GLVector arrive(const GLVector& vTarget, double deceleration);

public:
	void SetTarget1(Vehicle* pVehicle1) { m_pTarget1=pVehicle1; }
	void SetTarget2(Vehicle* pVehicle2) { m_pTarget2=pVehicle2; }

protected:
	Vehicle* m_pTarget1;
	Vehicle* m_pTarget2;
};
#endif