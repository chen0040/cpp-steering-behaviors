#ifndef _H_GL_CORPSE_H
#define _H_GL_CORPSE_H

#include "Obstacle.h"

class Corpse : public Obstacle
{
public:
	Corpse(GameWorld* pWorld, double scene_duration);
	virtual ~Corpse();

protected:
	double m_scene_duration;
	double m_scene_life;

protected:
	void GenDisplayList_Dot(GLuint& display_list_id);

public:
	virtual void Update(const long & lElapsedTicks);
};
#endif