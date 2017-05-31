#ifndef _H_GL_LOCOMOTION_LAND_CRAFT_H
#define _H_GL_LOCOMOTION_LAND_CRAFT_H

#include "GLLocomotion.h"

class GLLocomotion_Landcraft : public GLLocomotion
{
public:
	GLLocomotion_Landcraft(Vehicle* pAgent);
	virtual ~GLLocomotion_Landcraft();

public:
	virtual void Update(const long& lElapsedTicks);

protected:
	void EnforceNonPenetrationOnLandObstacles();
};

#endif