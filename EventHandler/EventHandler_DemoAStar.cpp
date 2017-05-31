#include "EventHandler_DemoAStar.h"
#include "../GameWorld/GameWorld.h"
#include "../GLStates/GLState_AStar.h"
#include <iostream>

EventHandler_DemoAStar::EventHandler_DemoAStar(GLUIObj* pSubject)
: GLActionListener(pSubject)
, m_pWorld(NULL)
{

}

EventHandler_DemoAStar::~EventHandler_DemoAStar()
{

}

void EventHandler_DemoAStar::MouseButtonDown(const int& iButton, const int& iX, const int& iY, const int& iXRel, const int& iYRel) 
{
	m_pWorld->ChangeState(&glState_AStar);
}