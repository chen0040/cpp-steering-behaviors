#ifndef _H_EVENT_HANDLER_DEMO_BEZIER_H
#define _H_EVENT_HANDLER_DEMO_BEZIER_H

#include "../GLUI/GLActionListener.h"

class GameWorld;

class EventHandler_DemoBezier : public GLActionListener
{
public:
	EventHandler_DemoBezier(GLUIObj* pSubject);
	virtual ~EventHandler_DemoBezier() { }

public:
	virtual void MouseButtonDown(const int& iButton, const int&  iX, const int& iY, const int& iXRel, const int& iYRel);

protected:
	GameWorld* m_pWorld;

public:
	void SetWorld(GameWorld* pWorld) { m_pWorld=pWorld; }
};

#endif