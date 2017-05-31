#ifndef _H_GL_STEERING_WALL_AVOIDANCE_H
#define _H_GL_STEERING_WALL_AVOIDANCE_H

#include "GLSteering.h"
#include <vector>

class GLLineSegment;

class GLSteering_WallAvoidance : public GLSteering
{
public:
	GLSteering_WallAvoidance(Vehicle* pVehicle);
	virtual ~GLSteering_WallAvoidance() { }

public:
	virtual GLVector calculate();

protected:
	double m_dFeelerLength;
	void CreateFeelers(std::vector<GLLineSegment>& feelers);
public:
	void SetFeelerLength(double length) { m_dFeelerLength=length; }
};
#endif