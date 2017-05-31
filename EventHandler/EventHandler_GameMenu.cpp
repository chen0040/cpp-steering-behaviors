#include "EventHandler_GameMenu.h"
#include "../GameWorld/GameWorld.h"
#include "../GLStates/GLState_GameMenu.h"

EventHandler_GameMenu::EventHandler_GameMenu(GLUIObj* pSubject)
: GLActionListener(pSubject)
{

}

void EventHandler_GameMenu::MouseButtonDown(const int& iButton, const int&  iX, const int& iY, const int& iXRel, const int& iYRel)
{
	m_pWorld->ChangeState(&glState_GameMenu);
}