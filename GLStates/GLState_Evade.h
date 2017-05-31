#ifndef _H_STATE_EVADE_H
#define _H_STATE_EVADE_H

#include "GLState.h"

class GLState_Evade : public GLState
{
public:
	virtual ~GLState_Evade();
	static GLState_Evade* Instance();

private:
	GLState_Evade();
	GLState_Evade(const GLState_Evade& rhs) { }
	GLState_Evade& operator=(const GLState_Evade& rhs) { return *this; }

public:
	virtual void Entered(GameWorld* pWorld);
	virtual void Exited(GameWorld* pWorld);
	virtual void Update(GameWorld* pWorld, const long& lElapsedTicks);
};

#define glState_Evade (*(GLState_Evade::Instance()))
#endif