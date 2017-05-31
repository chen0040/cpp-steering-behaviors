#ifndef _H_GL_STEERING_ALIGNMENT_H
#define _H_GL_STEERING_ALIGNMENT_H

#include "../GLSteering/GLSteering.h"

class GLSteering_Alignment : public GLSteering
{
public:
	GLSteering_Alignment(Vehicle* pVehicle);
	virtual ~GLSteering_Alignment() { }

public:
	virtual GLVector calculate();
};
#endif