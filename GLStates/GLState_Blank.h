#ifndef _H_GL_STATE_BLANK_H
#define _H_GL_STATE_BLANK_H

#include "GLState.h"

class GLState_Blank : public GLState
{
public:
	static GLState_Blank* Instance();
	virtual ~GLState_Blank();

private:
	GLState_Blank();
	GLState_Blank(const GLState_Blank& rhs) { }
	GLState_Blank& operator=(const GLState_Blank& rhs) { return *this; }

public:
	virtual void Entered(GameWorld* pWorld);
	virtual void Exited(GameWorld* pWorld);
	virtual void Update(GameWorld* pWorld, const long& lElapsedTicks);
	virtual void Render(GameWorld* pWorld);

public:
	virtual void MouseButtonDown(GameWorld* pWorld);
};

#define glState_Blank (*(GLState_Blank::Instance()))
#endif