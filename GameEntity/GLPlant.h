#ifndef _H_GL_PLANT_H
#define _H_GL_PLANT_H

#include "Obstacle.h"

class GLPlant : public Obstacle
{
public:
	GLPlant(GameWorld* pWorld);
	virtual ~GLPlant() { }

public:
	virtual void Update(const long& lElapsedTicks);
};
#endif