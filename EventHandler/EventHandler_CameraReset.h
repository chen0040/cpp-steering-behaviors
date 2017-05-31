#ifndef _H_EVENT_HANDLER_CAMERA_RESET_H
#define _H_EVENT_HANDLER_CAMERA_RESET_H

#include "../GLUI/GLActionListener.h"

class GLCamera;

class EventHandler_CameraReset : public GLActionListener
{
public:
	EventHandler_CameraReset(GLUIObj* pSubject);
	virtual ~EventHandler_CameraReset();

	virtual void MouseButtonDown(const int& iButton, const int& iX, const int& iY, const int& iXRel, const int& iYRel);

public:
	void SetCamera(GLCamera* pCamera) { m_pCamera=pCamera; }

protected:
	GLCamera* m_pCamera;
};

#endif