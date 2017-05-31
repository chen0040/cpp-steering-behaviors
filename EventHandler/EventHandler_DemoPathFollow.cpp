#include "EventHandler_DemoPathFollow.h"
#include "../GameWorld/GameWorld.h"
#include "../GLStates/GLState_PathFollow.h"

EventHandler_DemoPathFollow::EventHandler_DemoPathFollow(GLUIObj* pSubject)
: GLActionListener(pSubject)
{

}

EventHandler_DemoPathFollow::~EventHandler_DemoPathFollow()
{
}

void EventHandler_DemoPathFollow::MouseButtonDown(const int &iButton, const int &iX, const int &iY, const int &iXRel, const int &iYRel)
{
	m_pWorld->ChangeState(&glState_PathFollow);
}