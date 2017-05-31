#ifndef _H_EVENT_HANDLER_DEMO_PURSUIT_H
#define _H_EVENT_HANDLER_DEMO_PURSUIT_H

#include "../GLUI/GLActionListener.h"

class GameWorld;

class EventHandler_DemoPursuit : public GLActionListener
{
public:
	EventHandler_DemoPursuit(GLUIObj* pSubject);
	virtual ~EventHandler_DemoPursuit();

public:
	void MouseButtonDown(const int& iButton, const int& iX, const int& iY, const int& iXRel, const int& iYRel);

protected:
	GameWorld* m_pWorld;

public:
	void SetWorld(GameWorld* pWorld) { m_pWorld=pWorld; }
};
#endif