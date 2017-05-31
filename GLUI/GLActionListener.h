#ifndef _H_GL_ACTION_LISTENER_H
#define _H_GL_ACTION_LISTENER_H

class GLUIObj;

class GLActionListener
{
public:
	GLActionListener(GLUIObj* pSubject);
	virtual ~GLActionListener();

public:
	virtual void MouseMoved(const int& iButton, const int& iX, const int& iY, const int& iXRel, const int& iYRel) { }
	virtual void MouseButtonDown(const int& iButton, const int& iX, const int& iY, const int& iXRel, const int& iYRel) { }
	virtual void MouseButtonUp(const int& iButton, const int& iX, const int& iY, const int& iXRel, const int& iYRel) { }
};
#endif