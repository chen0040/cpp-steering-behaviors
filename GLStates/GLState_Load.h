#ifndef _H_GL_STATE_LOAD_H
#define _H_GL_STATE_LOAD_H

#include "GLState.h"

class GLState_Load : public GLState
{
public:
	static GLState_Load* Instance();
	virtual ~GLState_Load();

private:
	GLState_Load();
	GLState_Load(const GLState_Load& rhs) { }
	GLState_Load& operator=(const GLState_Load& rhs) { return *this; }

public:
	virtual void Entered(GameWorld* pWorld);
	virtual void Exited(GameWorld* pWorld);
	virtual void Update(GameWorld* pWorld, const long& lElapsedTicks);
	virtual void Render(GameWorld* pWorld);

public:
	virtual void MouseButtonDown(GameWorld* pWorld);

protected:
	bool m_bSetup;
};

#define glState_Load (*(GLState_Load::Instance()))
#endif