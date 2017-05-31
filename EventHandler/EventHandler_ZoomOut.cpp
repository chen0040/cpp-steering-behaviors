#include "EventHandler_ZoomOut.h"
#include "../GLCamera/GLSphericalCamera.h"
#include <iostream>

EventHandler_ZoomOut::EventHandler_ZoomOut(GLUIObj* pSubject)
: GLActionListener(pSubject)
, m_pCamera(NULL)
{

}

EventHandler_ZoomOut::~EventHandler_ZoomOut()
{

}

void EventHandler_ZoomOut::MouseButtonDown(const int& iButton, const int& iX, const int& iY, const int& iXRel, const int& iYRel) 
{
	m_pCamera->Reset();
	m_pCamera->IncrementRadius(30);
	
}