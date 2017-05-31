#ifndef _H_GL_STEERING_SEEK_H
#define _H_GL_STEERING_SEEK_H

#include "GLSteering.h"

class GLSteering_Seek : public GLSteering
{
public:
	GLSteering_Seek(Vehicle* pVehicle);
	virtual ~GLSteering_Seek();

public:
	virtual GLVector calculate();

protected:
	GLVector m_vTarget;

public:
	void SetTarget(const GLVector vTarget) { m_vTarget=vTarget; }
	
};
#endif