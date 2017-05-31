#ifndef _H_EVENT_HANDLER_DEMO_FLEE_H
#define _H_EVENT_HANDLER_DEMO_FLEE_H

#include "../GLUI/GLActionListener.h"

class GameWorld;

class EventHandler_DemoFlee : public GLActionListener
{
public:
	EventHandler_DemoFlee(GLUIObj* pSubject);
	virtual ~EventHandler_DemoFlee();

public:
	virtual void MouseButtonDown(const int& iButton, const int& iX, const int& iY, const int& iXRel, const int& iYRel);

public:
	void SetWorld(GameWorld* pWorld);

protected:
	GameWorld* m_pWorld;
};
#endif