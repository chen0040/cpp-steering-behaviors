#ifndef _H_GL_STEERING_SEPARATION_H
#define _H_GL_STEERING_SEPARATION_H

#include "GLSteering.h"

class GLSteering_Separation : public GLSteering
{
public:
	GLSteering_Separation(Vehicle* pVehicle);
	virtual ~GLSteering_Separation() { }

public:
	virtual GLVector calculate();
};
#endif