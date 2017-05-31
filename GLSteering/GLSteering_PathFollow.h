#ifndef _H_GL_STEERING_PATH_FOLLOW_H
#define _H_GL_STEERING_PATH_FOLLOW_H

#include "GLSteering.h"
#include "GLPath.h"

class GLSteering_PathFollow : public GLSteering
{
public:
	GLSteering_PathFollow(Vehicle* pVehicle);
	virtual ~GLSteering_PathFollow();

public:
	virtual GLVector calculate();

protected:
	GLVector seek(const GLVector& vTarget);
	GLVector arrive(const GLVector& vTarget, double deceleration);

protected:
	GLPath* m_pPath;

protected:
	double m_target_deviation;
public:
	void SetTargetDeviation(double dev) { m_target_deviation=dev; }

public:
	void SetPath(GLPath* pPath) { m_pPath=pPath; }

};
#endif