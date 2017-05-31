#ifndef _H_GL_STEERING_AVOIDANCE_H
#define _H_GL_STEERING_AVOIDANCE_H

#include "GLSteering.h"

class GLSteering_Avoidance : public GLSteering
{
public:
	GLSteering_Avoidance(Vehicle* pVehicle);
	virtual ~GLSteering_Avoidance() { }

public:
	virtual GLVector calculate();

protected:
	double m_dMinBoxLength;
	double m_braking_weight;
public:
	void SetMinDetectionBoxLength(double len) { m_dMinBoxLength=len; }
	void SetBrakingWeight(double w) { m_braking_weight=w; }
};
#endif