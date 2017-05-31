#ifndef _H_GL_STATE_PURSUIT_H
#define _H_GL_STATE_PURSUIT_H

#include "GLState.h"

class GLState_Pursuit : public GLState
{
public:
	virtual ~GLState_Pursuit();
	static GLState_Pursuit* Instance();

private:
	GLState_Pursuit();
	GLState_Pursuit(const GLState_Pursuit& rhs) { }
	GLState_Pursuit& operator=(const GLState_Pursuit& rhs) { return *this; }

public:
	virtual void Entered(GameWorld* pWorld);
	virtual void Exited(GameWorld* pWorld);
	virtual void Update(GameWorld* pWorld, const long& lElapsedTicks);

public:
	void MouseButtonDown(GameWorld* pWorld);
};

#define glState_Pursuit (*(GLState_Pursuit::Instance()))
#endif