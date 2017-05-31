#ifndef _H_GL_STATE_FLOCK_H
#define _H_GL_STATE_FLOCK_H

#include "GLState.h"

class Vehicle;

class GLState_Flock : public GLState
{
public:
	virtual ~GLState_Flock() { }
	static GLState_Flock* Instance();

private:
	GLState_Flock();
	GLState_Flock(const GLState_Flock& rhs) { }
	GLState_Flock& operator=(const GLState_Flock& rhs) { return *this; }

public:
	virtual void Entered(GameWorld* pWorld);
	virtual void Exited(GameWorld* pWorld);
	virtual void Update(GameWorld* pWorld, const long& lElapsedTicks);
	virtual void MouseButtonDown(GameWorld* pWorld);

protected:
	Vehicle* CreateAgent(GameWorld* pWorld);
	Vehicle* CreatePredator(GameWorld* pWorld);

public:
	static const int PREDATOR1=1;
	static const int PREDATOR2=2;
	static const int FLOCK1=3;
	static const int FLOCK2=4;
};
#define glState_Flock (*(GLState_Flock::Instance()))
#endif