#ifndef _H_EVENT_HANDLER_ZOOM_OUT_H
#define _H_EVENT_HANDLER_ZOOM_OUT_H

#include "../GLUI/GLActionListener.h"

class GLSphericalCamera;

class EventHandler_ZoomOut : public GLActionListener
{
public:
	EventHandler_ZoomOut(GLUIObj* pSubject);
	virtual ~EventHandler_ZoomOut();

	virtual void MouseButtonDown(const int& iButton, const int& iX, const int& iY, const int& iXRel, const int& iYRel);

public:
	void SetCamera(GLSphericalCamera* pCamera) { m_pCamera=pCamera; }

protected:
	GLSphericalCamera* m_pCamera;
};

#endif