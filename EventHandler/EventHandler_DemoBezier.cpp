#include "EventHandler_DemoBezier.h"
#include "../GameWorld/GameWorld.h"
#include "../GLStates/GLState_Bezier.h"

EventHandler_DemoBezier::EventHandler_DemoBezier(GLUIObj* pSubject)
: GLActionListener(pSubject)
{

}

void EventHandler_DemoBezier::MouseButtonDown(const int& iButton, const int&  iX, const int& iY, const int& iXRel, const int& iYRel)
{
	m_pWorld->ChangeState(&glState_Bezier);
}