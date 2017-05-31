#ifndef _H_GL_LOCOMOTION_AIR_CRAFT_H
#define _H_GL_LOCOMOTION_AIR_CRAFT_H

#include "GLLocomotion.h"

class GLLocomotion_Aircraft : public GLLocomotion
{
public:
	GLLocomotion_Aircraft(Vehicle* pAgent);
	virtual ~GLLocomotion_Aircraft();

public:
	virtual void Update(const long& lElapsedTicks);

protected:
	double m_dy;
};

#endif