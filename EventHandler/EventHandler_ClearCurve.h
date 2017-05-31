#ifndef _H_EVENT_HANDLER_CLEAR_CURVE_H
#define _H_EVENT_HANDLER_CLEAR_CURVE_H

#include "../GLUI/GLActionListener.h"

class GLCurve;

class EventHandler_ClearCurve : public GLActionListener
{
public:
	EventHandler_ClearCurve(GLUIObj* pSubject);
	virtual ~EventHandler_ClearCurve() { }

public:
	virtual void MouseButtonDown(const int& iButton, const int& iX, const int& iY, const int& iXRel, const int& iYRel);

public:
	void SetCurve(GLCurve* pCurve) { m_pCurve=pCurve; }

protected:
	GLCurve* m_pCurve;
};
#endif