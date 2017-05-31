#include "EventHandler_ClearCurve.h"
#include "../GLShape/GLCurve.h"

EventHandler_ClearCurve::EventHandler_ClearCurve(GLUIObj* pSubject)
: GLActionListener(pSubject)
, m_pCurve(NULL)
{
}

void EventHandler_ClearCurve::MouseButtonDown(const int &iButton, const int &iX, const int &iY, const int &iXRel, const int &iYRel)
{
	m_pCurve->Clear();
}