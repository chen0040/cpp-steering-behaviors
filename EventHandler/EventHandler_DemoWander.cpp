#include "EventHandler_DemoWander.h"
#include "../GameWorld/GameWorld.h"
#include "../GLStates/GLState_Wander.h"

EventHandler_DemoWander::EventHandler_DemoWander(GLUIObj* pSubject)
: GLActionListener(pSubject)
{

}

EventHandler_DemoWander::~EventHandler_DemoWander()
{

}

void EventHandler_DemoWander::MouseButtonDown(const int &iButton, const int &iX, const int &iY, const int &iXRel, const int &iYRel)
{
	m_pWorld->ChangeState(&glState_Wander);
}