#ifndef _H_EVENT_HANDLER_DEMO_PATH_FOLLOW_H
#define _H_EVENT_HANDLER_DEMO_PATH_FOLLOW_H

#include "../GLUI/GLActionListener.h"

class GameWorld;

class EventHandler_DemoPathFollow : public GLActionListener
{
public:
	EventHandler_DemoPathFollow(GLUIObj* pSubject);
	virtual ~EventHandler_DemoPathFollow();

public:
	virtual void MouseButtonDown(const int& iButton, const int& iX, const int& iY, const int& iXRel, const int& iYRel);

public:
	void SetWorld(GameWorld* pWorld) { m_pWorld=pWorld; }

protected:
	GameWorld* m_pWorld;
};
#endif