#ifndef _H_EVENT_HANDLER_DEMO_INTERPOSE_H
#define _H_EVENT_HANDLER_DEMO_INTERPOSE_H

#include "../GLUI/GLActionListener.h"

class GameWorld;

class EventHandler_DemoInterpose : public GLActionListener
{
public:
	EventHandler_DemoInterpose(GLUIObj* pSubject);
	virtual ~EventHandler_DemoInterpose() { }

public:
	void MouseButtonDown(const int& iButton, const int& iX, const int& iY, const int& iXRel, const int& iYRel);

protected:
	GameWorld* m_pWorld;

public:
	void SetWorld(GameWorld* pWorld) { m_pWorld=pWorld; }
};
#endif
