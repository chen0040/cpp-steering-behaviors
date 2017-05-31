#ifndef _H_EVENT_HANDLER_DEMO_FLOCK_H
#define _H_EVENT_HANDLER_DEMO_FLOCK_H

#include "../GLUI/GLActionListener.h"

class GameWorld;

class EventHandler_DemoFlock : public GLActionListener
{
public:
	EventHandler_DemoFlock(GLUIObj* pSubject);
	virtual ~EventHandler_DemoFlock() { }

protected:
	GameWorld* m_pWorld;
public:
	void SetWorld(GameWorld* pWorld) { m_pWorld=pWorld; }

public:
	virtual void MouseButtonDown(const int& iButton, const int& iX, const int& iY, const int& iXRel, const int & iYRel);
};
#endif