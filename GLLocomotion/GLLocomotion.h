#ifndef _H_GL_LOCOMOTION_H
#define _H_GL_LOCOMOTION_H

class Vehicle;

class GLLocomotion
{
public:
	GLLocomotion(Vehicle* pAgent);
	virtual ~GLLocomotion();

public:
	virtual void Update(const long& lElapsedTicks)=0;

protected:
	void EnforceNonPenetration();

protected:
	Vehicle* m_pAgent;
};

#endif