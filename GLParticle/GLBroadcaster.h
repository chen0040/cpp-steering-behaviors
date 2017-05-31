#ifndef _H_GL_BROADCASTER_H
#define _H_GL_BROADCASTER_H

#include "GLEmitter.h"

class GameEntity;

class GLBroadcaster : public GLEmitter
{
public:
	GLBroadcaster();
	virtual ~GLBroadcaster();

public:
	virtual void Render();
	virtual void Update(const long& lElapsedTicks);

public:
	void Broadcast(int count, double packet_travelling_distance, double packet_travelling_speed, double initial_radius);
};
#endif