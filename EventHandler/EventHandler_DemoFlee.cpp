#include "EventHandler_DemoFlee.h"
#include "../GameWorld/GameWorld.h"
#include "../GLStates/GLState_Flee.h"

EventHandler_DemoFlee::EventHandler_DemoFlee(GLUIObj* pSubject)
: GLActionListener(pSubject)
, m_pWorld(NULL)
{

}

EventHandler_DemoFlee::~EventHandler_DemoFlee()
{

}

void EventHandler_DemoFlee::SetWorld(GameWorld* pWorld)
{
	m_pWorld=pWorld;
}

void EventHandler_DemoFlee::MouseButtonDown(const int &iButton, const int &iX, const int &iY, const int &iXRel, const int &iYRel)
{
	m_pWorld->ChangeState(&glState_Flee);
}