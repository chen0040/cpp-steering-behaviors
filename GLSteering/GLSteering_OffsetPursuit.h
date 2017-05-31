#ifndef _H_GL_STEERING_OFFSET_PURSUIT_H
#define _H_GL_STEERING_OFFSET_PURSUIT_H

#include "GLSteering.h"

class GLSteering_OffsetPursuit : public GLSteering
{
public:
	GLSteering_OffsetPursuit(Vehicle* pVehicle);
	virtual ~GLSteering_OffsetPursuit() { }

public:
	virtual GLVector calculate();

protected:
	Vehicle* m_pTarget;
public:
	void SetTarget(Vehicle* pTarget) { m_pTarget=pTarget; }

protected:
	GLVector arrive(const GLVector& vTarget, double deceleration);

protected:
	GLVector m_offset;
public:
	void SetOffset(const GLVector offset) { m_offset=offset; }
};

#endif