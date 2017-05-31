#ifndef _H_GL_STEERING_COMPOSITE_H
#define _H_GL_STEERING_COMPOSITE_H

#include "GLSteering.h"
#include <list>
#include <string>
#include <map>

class GLSteering_Composite : public GLSteering
{
public:
	GLSteering_Composite(Vehicle* pVehicle);
	virtual ~GLSteering_Composite();

public:
	void AddSteering(const char* sname, GLSteering* pSteering);
	GLSteering* GetSteering(std::string sname);
	void RemoveSteering(const char* sname);

public:
	virtual GLVector calculate();

protected:
	bool AccumulateForce(GLVector & vForce, const GLVector& vSteeringForce);

public:
	std::list<GLSteering*> m_steerings;
	std::map<std::string, GLSteering*> m_steering_map;

public:
	virtual void TurnOn();

};
#endif