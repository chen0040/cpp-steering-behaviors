#include "EventHandler_ScalePlane.h"
#include "../GLFloor/GLFloor.h"

EventHandler_ScalePlane::EventHandler_ScalePlane(GLUIObj *pSubject)
: GLActionListener(pSubject)
, m_pPlane(NULL)
, m_scale(1.1)
{
}

void EventHandler_ScalePlane::MouseButtonDown(const int &iButton, const int &iX, const int &iY, const int &iXRel, const int &iYRel)
{
	m_pPlane->CreateHorizontalPlane(m_pPlane->GetLength() * m_scale);
}