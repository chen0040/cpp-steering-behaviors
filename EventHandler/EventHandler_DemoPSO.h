#ifndef _H_EVENT_HANDLER_PSO_H
#define _H_EVENT_HANDLER_PSO_H

#include "../GLUI/GLActionListener.h"

class GameWorld;

class EventHandler_DemoPSO : public GLActionListener
{
public:
	EventHandler_DemoPSO(GLUIObj* pSubject);
	virtual ~EventHandler_DemoPSO();

	virtual void MouseButtonDown(const int& iButton, const int& iX, const int& iY, const int& iXRel, const int& iYRel);

public:
	void SetWorld(GameWorld* pWorld) { m_pWorld=pWorld; }

protected:
	GameWorld* m_pWorld;
};

#endif