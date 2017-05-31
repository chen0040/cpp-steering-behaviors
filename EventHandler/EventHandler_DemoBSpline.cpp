#include "EventHandler_DemoBSpline.h"
#include "../GameWorld/GameWorld.h"
#include "../GLStates/GLState_BSpline.h"

EventHandler_DemoBSpline::EventHandler_DemoBSpline(GLUIObj* pSubject)
: GLActionListener(pSubject)
{

}

void EventHandler_DemoBSpline::MouseButtonDown(const int& iButton, const int&  iX, const int& iY, const int& iXRel, const int& iYRel)
{
	m_pWorld->ChangeState(&glState_BSpline);
}