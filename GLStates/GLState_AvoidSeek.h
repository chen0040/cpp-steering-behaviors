#ifndef _H_GL_STATE_AVOID_SEEK_H
#define _H_GL_STATE_AVOID_SEEK_H

#include "GLState.h"

class Obstacle;

class GLState_AvoidSeek : public GLState
{
public:
	static GLState_AvoidSeek* Instance();
	virtual ~GLState_AvoidSeek();

private:
	GLState_AvoidSeek();
	GLState_AvoidSeek(const GLState_AvoidSeek& rhs) { }
	GLState_AvoidSeek& operator=(const GLState_AvoidSeek& rhs) { return *this; }

public:
	virtual void Entered(GameWorld* pWorld);
	virtual void Exited(GameWorld* pWorld);
	virtual void Update(GameWorld* pWorld, const long& lElapsedTicks);

public:
	virtual void MouseButtonDown(GameWorld* pWorld);

protected:
	Obstacle* CreateObstacle(GameWorld* pWorld);
};

#define glState_AvoidSeek (*(GLState_AvoidSeek::Instance()))
#endif