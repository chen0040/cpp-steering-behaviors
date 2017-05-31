#ifndef _H_EVENT_HANDLER_GAME_MENU_H
#define _H_EVENT_HANDLER_GAME_MENU_H

#include "../GLUI/GLActionListener.h"

class GameWorld;

class EventHandler_GameMenu : public GLActionListener
{
public:
	EventHandler_GameMenu(GLUIObj* pSubject);
	virtual ~EventHandler_GameMenu() { }

public:
	virtual void MouseButtonDown(const int& iButton, const int&  iX, const int& iY, const int& iXRel, const int& iYRel);

protected:
	GameWorld* m_pWorld;

public:
	void SetWorld(GameWorld* pWorld) { m_pWorld=pWorld; }
};

#endif