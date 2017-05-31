#ifndef _H_EVENT_HANDLER_DEMO_WANDER_H
#define _H_EVENT_HANDLER_DEMO_WANDER_H

#include "../GLUI/GLActionListener.h"

class GameWorld;

class EventHandler_DemoWander : public GLActionListener
{
public:
	EventHandler_DemoWander(GLUIObj* pSubject);
	virtual ~EventHandler_DemoWander();

public:
	virtual void MouseButtonDown(const int& iButton, const int& iX, const int& iY, const int& iXRel, const int& iYRel);

protected:
	GameWorld* m_pWorld;

public:
	void SetWorld(GameWorld* pWorld) { m_pWorld=pWorld; }
};
#endif