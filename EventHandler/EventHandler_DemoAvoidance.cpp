#include "EventHandler_DemoAvoidance.h"
#include "../GameWorld/GameWorld.h"
#include "../GLStates/GLState_Avoidance.h"

EventHandler_DemoAvoidance::EventHandler_DemoAvoidance(GLUIObj* pSubject)
: GLActionListener(pSubject)
, m_pWorld(NULL)
{

}

void EventHandler_DemoAvoidance::MouseButtonDown(const int& iButton, const int& iX, const int& iY, const int& iXRel, const int& iYRel)
{
	m_pWorld->ChangeState(&glState_Avoidance);
}