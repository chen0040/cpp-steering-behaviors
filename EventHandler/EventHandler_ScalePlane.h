#ifndef _H_EVENT_HANDLER_ENLARGE_PLANE_H
#define _H_EVENT_HANDLER_ENLARGE_PLANE_H

#include "../GLUI/GLActionListener.h"

class GLFloor;

class EventHandler_ScalePlane : public GLActionListener
{
public:
	EventHandler_ScalePlane(GLUIObj* pSubject);
	virtual ~EventHandler_ScalePlane() { }

public:
	virtual void MouseButtonDown(const int& iButton, const int& iX, const int& iY, const int& iXRel, const int& iYRel);

protected:
	GLFloor* m_pPlane;
public:
	void SetPlane(GLFloor* pPlane) { m_pPlane=pPlane; }

public:
	void SetScaling(double scale) { m_scale=scale; }
protected:
	double m_scale;
};
#endif