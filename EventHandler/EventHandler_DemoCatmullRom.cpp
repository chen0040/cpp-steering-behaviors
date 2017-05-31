#include "EventHandler_DemoCatmullRom.h"
#include "../GameWorld/GameWorld.h"
#include "../GLStates/GLState_CatmullRom.h"

EventHandler_DemoCatmullRom::EventHandler_DemoCatmullRom(GLUIObj* pSubject)
: GLActionListener(pSubject)
{

}

void EventHandler_DemoCatmullRom::MouseButtonDown(const int& iButton, const int&  iX, const int& iY, const int& iXRel, const int& iYRel)
{
	m_pWorld->ChangeState(&glState_CatmullRom);
}