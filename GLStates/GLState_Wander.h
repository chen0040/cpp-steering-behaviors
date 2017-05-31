#ifndef _H_GL_STATE_WANDER_H
#define _H_GL_STATE_WANDER_H

#include "GLState.h"

class GLState_Wander : public GLState
{
public:
	virtual ~GLState_Wander();
	static GLState_Wander* Instance();

private:
	GLState_Wander();
	GLState_Wander(const GLState_Wander& rhs) { }
	GLState_Wander& operator=(const GLState_Wander& rhs) {  return *this; }

public:
	virtual void Entered(GameWorld* pWorld);
	virtual void Exited(GameWorld* pWorld);
	virtual void Update(GameWorld* pWorld, const long& lElapsedTicks);

public:
	virtual void MouseButtonDown(GameWorld* pWorld);

protected:
	int m_iMaxFPS;
	int m_dPlaneLength;
};

#define glState_Wander (*(GLState_Wander::Instance()))
#endif