#ifndef _H_GL_STATE_BSPLINE_H
#define _H_GL_STATE_BSPLINE_H

#include "GLState.h"
#include "../GLShape/GLBSpline.h"

class GLCommandPanel;

class GLState_BSpline : public GLState
{
public:
	virtual ~GLState_BSpline();
	static GLState_BSpline* Instance();

private:
	GLState_BSpline();
	GLState_BSpline(const GLState_BSpline& rhs) { }
	GLState_BSpline& GLState_BSpline::operator=(const GLState_BSpline& rhs) { return *this; }

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
	GLBSpline m_curve;
};

#define glState_BSpline (*(GLState_BSpline::Instance()))
#endif