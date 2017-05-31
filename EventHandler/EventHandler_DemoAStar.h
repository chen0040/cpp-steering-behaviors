#ifndef _H_EVENT_HANDLER_A_STAR_H
#define _H_EVENT_HANDLER_A_STAR_H

#include "../GLUI/GLActionListener.h"

class GameWorld;

class EventHandler_DemoAStar : public GLActionListener
{
public:
	EventHandler_DemoAStar(GLUIObj* pSubject);
	virtual ~EventHandler_DemoAStar();

	virtual void MouseButtonDown(const int& iButton, const int& iX, const int& iY, const int& iXRel, const int& iYRel);

public:
	void SetWorld(GameWorld* pWorld) { m_pWorld=pWorld; }

protected:
	GameWorld* m_pWorld;
};

#endif