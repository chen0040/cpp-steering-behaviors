#ifndef _H_GL_STATE_INTERPOSE_H
#define _H_GL_STATE_INTERPOSE_H

#include "GLState.h"

class Vehicle;

class GLState_Interpose : public GLState
{
public:
	virtual ~GLState_Interpose() { }
	static GLState_Interpose* Instance();

private:
	GLState_Interpose();
	GLState_Interpose(const GLState_Interpose& rhs) { }
	GLState_Interpose& operator=(const GLState_Interpose& rhs) { return *this; }

protected:
	Vehicle* CreateWanderAgent(GameWorld* pWorld);
	Vehicle* CreateInterposeAgent(GameWorld* pWorld, Vehicle* pAgent1, Vehicle* pAgent2);

public:
	virtual void Entered(GameWorld* pWorld);
	virtual void Exited(GameWorld* pWorld);
	virtual void Update(GameWorld* pWorld, const long& lElapsedTicks);
	virtual void MouseButtonDown(GameWorld* pWorld);
};

#define glState_Interpose (*(GLState_Interpose::Instance()))
#endif