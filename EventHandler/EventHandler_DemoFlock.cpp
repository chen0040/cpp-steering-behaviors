#include "EventHandler_DemoFlock.h"
#include "../GameWorld/GameWorld.h"
#include "../GLStates/GLState_Flock.h"

EventHandler_DemoFlock::EventHandler_DemoFlock(GLUIObj* pSubject)
: GLActionListener(pSubject)
, m_pWorld(NULL)
{

}

void EventHandler_DemoFlock::MouseButtonDown(const int& iButton, const int& iX, const int& iY, const int& iXRel, const int & iYRel)
{
	m_pWorld->ChangeState(&glState_Flock);
}