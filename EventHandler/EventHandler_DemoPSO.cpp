#include "EventHandler_DemoPSO.h"
#include "../GameWorld/GameWorld.h"
#include "../GLStates/GLState_PSO.h"
#include <iostream>

EventHandler_DemoPSO::EventHandler_DemoPSO(GLUIObj* pSubject)
: GLActionListener(pSubject)
, m_pWorld(NULL)
{

}

EventHandler_DemoPSO::~EventHandler_DemoPSO()
{

}

void EventHandler_DemoPSO::MouseButtonDown(const int& iButton, const int& iX, const int& iY, const int& iXRel, const int& iYRel) 
{
	m_pWorld->ChangeState(&glState_PSO);
}