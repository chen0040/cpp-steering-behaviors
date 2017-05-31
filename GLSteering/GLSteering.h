#ifndef _H_GL_STEERING_H
#define _H_GL_STEERING_H

#include "../GLVector/GLVector.h"
#include <string>

class Vehicle;

class GLSteering
{
public:
	GLSteering(Vehicle* pVehicle);
	virtual ~GLSteering();

public:
	virtual GLVector calculate()=0;

protected:
	Vehicle* m_pVehicle;

protected:
	bool m_bOn;

public:
	virtual void TurnOn() { m_bOn=true; }
	virtual void TurnOff() { m_bOn=false; }
	bool On() const { return m_bOn; }

protected:
	double m_dWeight;

public:
	void SetWeight(double dWeight) { m_dWeight=dWeight; }
	double GetWeight() { return m_dWeight; }
	
protected:
	std::string m_type;
public:
	bool is_a(const char* name);
	const char* get_name() const { return m_type.c_str(); }
};
#endif