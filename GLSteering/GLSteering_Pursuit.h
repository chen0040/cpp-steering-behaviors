#ifndef _H_GL_STEERING_PURSUIT_H
#define _H_GL_STEERING_PURSUIT_H

#include "GLSteering.h"

class GLSteering_Pursuit : public GLSteering
{
public:
	GLSteering_Pursuit(Vehicle* pVehicle);
	virtual ~GLSteering_Pursuit();

public:
	virtual GLVector calculate();

protected:
	Vehicle* m_pTarget;

public:
	void SetTarget(Vehicle* pVehicle) { m_pTarget=pVehicle; }
	Vehicle* GetTarget() { return m_pTarget; }

protected:
	GLVector seek(const GLVector& vTarget);

};
#endif