#include "EventHandler_DemoSeek.h"
#include "../GameWorld/GameWorld.h"
#include "../GLStates/GLState_Seek.h"

EventHandler_DemoSeek::EventHandler_DemoSeek(GLUIObj* pSubject)
: GLActionListener(pSubject)
, m_pWorld(NULL)
{

}

EventHandler_DemoSeek::~EventHandler_DemoSeek()
{

}

void EventHandler_DemoSeek::SetWorld(GameWorld* pWorld)
{
	m_pWorld=pWorld;
}

void EventHandler_DemoSeek::MouseButtonDown(const int &iButton, const int &iX, const int &iY, const int &iXRel, const int &iYRel)
{
	m_pWorld->ChangeState(&glState_Seek);
}