#ifndef _H_GL_STATE_SEEK_H
#define _H_GL_STATE_SEEK_H

#include "GLState.h"

class GLState_Seek : public GLState
{
public:
	static GLState_Seek* Instance();
	virtual ~GLState_Seek();

private:
	GLState_Seek();
	GLState_Seek(const GLState_Seek& rhs) { }
	GLState_Seek& operator=(const GLState_Seek& rhs) { return *this; }

public:
	virtual void Entered(GameWorld* pWorld);
	virtual void Exited(GameWorld* pWorld);
	virtual void Update(GameWorld* pWorld, const long& lElapsedTicks);

public:
	virtual void MouseButtonDown(GameWorld* pWorld);
};

#define glState_Seek (*(GLState_Seek::Instance()))
#endif