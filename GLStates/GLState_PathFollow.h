#ifndef _H_GL_STATE_PATH_FOLLOW_H
#define _H_GL_STATE_PATH_FOLLOW_H

#include "GLState.h"

class GLState_PathFollow : public GLState
{
public:
	virtual ~GLState_PathFollow();
	static GLState_PathFollow* Instance();

private:
	GLState_PathFollow();
	GLState_PathFollow(const GLState_PathFollow& rhs) { }
	GLState_PathFollow& operator=(const GLState_PathFollow& rhs) { return *this; }

public:
	virtual void Entered(GameWorld* pWorld);
	virtual void Exited(GameWorld* pWorld);
	virtual void Update(GameWorld* pWorld, const long& lElapsedTicks);

};

#define glState_PathFollow (*(GLState_PathFollow::Instance()))
#endif