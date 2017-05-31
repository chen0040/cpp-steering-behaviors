#include "EventHandler_DemoAvoidSeek.h"
#include "../GameWorld/GameWorld.h"
#include "../GLStates/GLState_AvoidSeek.h"

EventHandler_DemoAvoidSeek::EventHandler_DemoAvoidSeek(GLUIObj* pSubject)
: GLActionListener(pSubject)
, m_pWorld(NULL)
{

}

EventHandler_DemoAvoidSeek::~EventHandler_DemoAvoidSeek()
{

}

void EventHandler_DemoAvoidSeek::SetWorld(GameWorld* pWorld)
{
	m_pWorld=pWorld;
}

void EventHandler_DemoAvoidSeek::MouseButtonDown(const int &iButton, const int &iX, const int &iY, const int &iXRel, const int &iYRel)
{
	m_pWorld->ChangeState(&glState_AvoidSeek);
}