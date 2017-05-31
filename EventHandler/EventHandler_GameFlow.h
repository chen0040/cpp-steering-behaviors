#ifndef _H_EVENT_HANDLER_GAME_FLOW_H
#define _H_EVENT_HANDLER_GAME_FLOW_H

#include "../GLUI/GLActionListener.h"

class GameWorld;

class EventHandler_GameFlow : public GLActionListener
{
public:
	EventHandler_GameFlow(GLUIObj* pSubject);
	virtual ~EventHandler_GameFlow();

	virtual void MouseButtonDown(const int& iButton, const int& iX, const int& iY, const int& iXRel, const int& iYRel);

public:
	void SetWorld(GameWorld* pWorld) { m_pWorld=pWorld; }

protected:
	GameWorld* m_pWorld;
};

#endif