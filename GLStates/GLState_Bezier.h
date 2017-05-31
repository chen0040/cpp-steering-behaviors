#ifndef _H_GL_STATE_BEZIER_H
#define _H_GL_STATE_BEZIER_H

#include "GLState.h"
#include "../GLShape/GLBezier.h"

class GLCommandPanel;

class GLState_Bezier : public GLState
{
public:
	virtual ~GLState_Bezier();
	static GLState_Bezier* Instance();

private:
	GLState_Bezier();
	GLState_Bezier(const GLState_Bezier& rhs) { }
	GLState_Bezier& GLState_Bezier::operator=(const GLState_Bezier& rhs) { return *this; }

public:
	virtual void Entered(GameWorld* pWorld);
	virtual void Exited(GameWorld* pWorld);
	virtual void Update(GameWorld* pWorld, const long& lElapsedTicks);

	virtual void Render(GameWorld* pWorld);

public:
	virtual void MouseButtonDown(GameWorld* pWorld);
	virtual void MouseMoved(GameWorld* pWorld);

protected:
	GLCommandPanel* m_pCommands;

protected:
	void SetupUI(GameWorld* pWorld);

protected:
	GLBezier m_curve;
};

#define glState_Bezier (*(GLState_Bezier::Instance()))
#endif