#ifndef _H_GL_STATE_FLEE_H
#define _H_GL_STATE_FLEE_H

#include "GLState.h"

class GLState_Flee : public GLState
{
public:
	virtual ~GLState_Flee();
	static GLState_Flee* Instance();

private:
	GLState_Flee();
	GLState_Flee(const GLState& rhs) { }
	GLState_Flee& operator=(const GLState& rhs) { return *this; }

public:
	virtual void Entered(GameWorld* pWorld);
	virtual void Update(GameWorld* pWorld, const long& lElapsedTicks);
	virtual void Exited(GameWorld* pWorld);

public:
	virtual void MouseButtonDown(GameWorld* pWorld);
};

#define glState_Flee (*(GLState_Flee::Instance()))
#endif