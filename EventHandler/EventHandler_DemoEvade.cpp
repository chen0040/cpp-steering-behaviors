#include "EventHandler_DemoEvade.h"
#include "../GameWorld/GameWorld.h"
#include "../GLStates/GLState_Evade.h"

EventHandler_DemoEvade::EventHandler_DemoEvade(GLUIObj* pSubject)
: GLActionListener(pSubject)
, m_pWorld(NULL)
{

}

EventHandler_DemoEvade::~EventHandler_DemoEvade()
{

}

void EventHandler_DemoEvade::MouseButtonDown(const int &iButton, const int &iX, const int &iY, const int &iXRel, const int &iYRel)
{
	m_pWorld->ChangeState(&glState_Evade);
}