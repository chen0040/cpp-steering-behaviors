#ifndef _H_GL_MOUSE_H
#define _H_GL_MOUSE_H

#include "../GLRay/GLRay.h"

class GLMouse
{
public:
	GLMouse();
	virtual ~GLMouse();

public:
	virtual void MouseMoved(const int& iButton, const int& iX, const int& iY, const int& iXRel, const int& iYRel);
	virtual void MouseButtonDown(const int& iButton, const int& iX, const int& iY, const int& iXRel, const int& iYRel);
	virtual void MouseButtonUp(const int& iButton, const int& iX, const int& iY, const int& iXRel, const int& iYRel);

public:
	int GetMouseX()  const { return m_iX; }
	int GetMouseY() const { return m_iY; }
	int GetMouseXRel() const { return m_iXRel; }
	int GetMouseYRel() const { return m_iYRel; }
	int GetMouseButton() const { return m_iButton; }
	bool IsLeftButtonDown() const { return m_bLeftButtonDown; }
	bool IsRightButtonDown() const { return m_bRightButtonDown; }

public:
	GLVector Coordinate3D(double window_x, double window_y) const;
	GLRay GetRay() const;

protected:
	GLVector Unproject(double window_x, double window_y, double window_z) const;

protected:
	bool m_bLeftButtonDown;
	bool m_bRightButtonDown;
	int m_iX;
	int m_iY;
	int m_iXRel;
	int m_iYRel;
	int m_iButton;
};
#endif