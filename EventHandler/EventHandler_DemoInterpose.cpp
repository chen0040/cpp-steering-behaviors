#include "EventHandler_DemoInterpose.h"
#include "../GameWorld/GameWorld.h"
#include "../GLStates/GLState_Interpose.h"

EventHandler_DemoInterpose::EventHandler_DemoInterpose(GLUIObj* pSubject)
: GLActionListener(pSubject)
, m_pWorld(NULL)
{

}

void EventHandler_DemoInterpose::MouseButtonDown(const int &iButton, const int &iX, const int &iY, const int &iXRel, const int &iYRel)
{
	m_pWorld->ChangeState(&glState_Interpose);
}