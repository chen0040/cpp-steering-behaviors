#include "EventHandler_GameFlow.h"
#include "../GameWorld/GameWorld.h"
#include <iostream>

EventHandler_GameFlow::EventHandler_GameFlow(GLUIObj* pSubject)
: GLActionListener(pSubject)
, m_pWorld(NULL)
{

}

EventHandler_GameFlow::~EventHandler_GameFlow()
{

}

void EventHandler_GameFlow::MouseButtonDown(const int& iButton, const int& iX, const int& iY, const int& iXRel, const int& iYRel) 
{
	if(m_pWorld->IsPaused())
	{
		m_pWorld->UnPause();
	}
	else
	{
		m_pWorld->Pause();
	}
}