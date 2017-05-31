#ifndef _H_GL_STATE_H
#define _H_GL_STATE_H

#include "../GLFloor/GLFloor.h"
#include <iostream>

class GameWorld;

class GLState
{
public:
	GLState();
	virtual ~GLState();

public:
	virtual void Entered(GameWorld* pWorld)=0;
	virtual void Update(GameWorld* pWorld, const long& lElapsedTicks)=0;
	virtual void Exited(GameWorld* pWorld)=0;

public:
	virtual void Render(GameWorld* pWorld);

public:
	virtual void MouseButtonDown(GameWorld* pWorld) { }
	virtual void MouseButtonUp(GameWorld* pWorld) { }
	virtual void MouseMoved(GameWorld* pWorld) { }
	virtual void KeyDown(GameWorld* pWorld) { }
	virtual void KeyUp(GameWorld* pWorld) { }

public:
	void SetTitleTexture(const char* fname);
	std::string GetStateName() const { return m_state_name; }
	void SetStateName(std::string state_name) { m_state_name=state_name; }

protected:
	GLFloor m_title;
	std::string m_state_name;
};
#endif