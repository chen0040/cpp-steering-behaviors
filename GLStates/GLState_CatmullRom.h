#ifndef _H_GL_STATE_CATMULL_ROM_H
#define _H_GL_STATE_CATMULL_ROM_H

#include "GLState.h"
#include "../GLShape/GLCatmullRom.h"

class GLCommandPanel;
class ConstrainedAgent;
class Vehicle;

class GLState_CatmullRom : public GLState
{
public:
	virtual ~GLState_CatmullRom();
	static GLState_CatmullRom* Instance();

private:
	GLState_CatmullRom();
	GLState_CatmullRom(const GLState_CatmullRom& rhs) { }
	GLState_CatmullRom& GLState_CatmullRom::operator=(const GLState_CatmullRom& rhs) { return *this; }

public:
	virtual void Entered(GameWorld* pWorld);
	virtual void Exited(GameWorld* pWorld);
	virtual void Update(GameWorld* pWorld, const long& lElapsedTicks);

	virtual void Render(GameWorld* pWorld);

public:
	virtual void MouseButtonDown(GameWorld* pWorld);
	virtual void MouseMoved(GameWorld* pWorld);

protected:
	ConstrainedAgent* CreateConstrainedAgent(GameWorld* pWorld);
	ConstrainedAgent* CreateFlockAgent(GameWorld* pWorld);
	GLVector CreateOffset(Vehicle* pLeader, Vehicle* pFollower, GameWorld* pWorld);

protected:
	GLCommandPanel* m_pCommands;

protected:
	void SetupUI(GameWorld* pWorld);

protected:
	GLCatmullRom m_curve;
};

#define glState_CatmullRom (*(GLState_CatmullRom::Instance()))
#endif