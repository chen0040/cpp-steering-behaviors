#ifndef _H_EVENT_HANDLER_DEMO_AVOID_SEEK_H
#define _H_EVENT_HANDLER_DEMO_AVOID_SEEK_H

#include "../GLUI/GLActionListener.h"

class GameWorld;

class EventHandler_DemoAvoidSeek : public GLActionListener
{
public:
	EventHandler_DemoAvoidSeek(GLUIObj* pSubject);
	virtual ~EventHandler_DemoAvoidSeek();

public:
	virtual void MouseButtonDown(const int& iButton, const int& iX, const int& iY, const int& iXRel, const int& iYRel);

public:
	void SetWorld(GameWorld* pWorld);

protected:
	GameWorld* m_pWorld;
};
#endif