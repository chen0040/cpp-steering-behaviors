#ifndef _H_GL_CAMERA_H
#define _H_GL_CAMERA_H

#include "../GLVector/GLVector.h"

class GLCamera
{
public:
	GLCamera();
	virtual ~GLCamera();

public:
	virtual void PreTransform()=0;
	virtual void Transform()=0;
	virtual void PostTransform()=0;

protected:
	virtual void calculate_vectors()=0;

public:
	GLVector GetEye() const { return m_eye; }
	GLVector GetUp() const { return m_up_direction; }
	GLVector GetCenter() const { return m_focus; }
	GLVector GetRight() const { return m_right_direction; }
	GLVector GetLook() const { return m_look_direction; }

public:
	void SetCenter(const GLVector& focus) { m_focus = focus; }
	virtual void Scroll(const GLVector& focus) { }
	virtual void SetFocus(const GLVector& focus) { }

public:
	virtual void Reset() { }

public:
	void Lock() { m_bLocked=true; }
	void Unlock() { m_bLocked=false; }
	bool IsLocked() const { return m_bLocked; }

protected:
	GLVector m_focus;
	GLVector m_up_direction;
	GLVector m_eye;
	GLVector m_right_direction;
	GLVector m_look_direction;
	bool m_bLocked;

};
#endif