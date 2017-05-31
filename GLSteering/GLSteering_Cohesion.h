#ifndef _H_GL_STEERING_COHESION_H
#define _H_GL_STEERING_COHESION_H

#include "GLSteering.h"

class GLSteering_Cohesion : public GLSteering
{
public:
	GLSteering_Cohesion(Vehicle* pVehicle);
	virtual ~GLSteering_Cohesion() { }

public:
	virtual GLVector calculate();

protected:
	GLVector seek(const GLVector& vTarget);
};
#endif