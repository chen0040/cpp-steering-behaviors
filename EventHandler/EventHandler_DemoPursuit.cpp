#include "EventHandler_DemoPursuit.h"
#include "../GameWorld/GameWorld.h"
#include "../GLStates/GLState_Pursuit.h"

EventHandler_DemoPursuit::EventHandler_DemoPursuit(GLUIObj* pSubject)
: GLActionListener(pSubject)
, m_pWorld(NULL)
{

}

EventHandler_DemoPursuit::~EventHandler_DemoPursuit()
{

}

void EventHandler_DemoPursuit::MouseButtonDown(const int &iButton, const int &iX, const int &iY, const int &iXRel, const int &iYRel)
{
	m_pWorld->ChangeState(&glState_Pursuit);
}