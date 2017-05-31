#include "EventHandler_CameraReset.h"
#include "../GLCamera/GLCamera.h"
#include <iostream>

EventHandler_CameraReset::EventHandler_CameraReset(GLUIObj* pSubject)
: GLActionListener(pSubject)
, m_pCamera(NULL)
{

}

EventHandler_CameraReset::~EventHandler_CameraReset()
{

}

void EventHandler_CameraReset::MouseButtonDown(const int& iButton, const int& iX, const int& iY, const int& iXRel, const int& iYRel) 
{
	m_pCamera->Reset();
}