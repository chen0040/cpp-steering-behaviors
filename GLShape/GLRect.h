#ifndef _H_GL_RECT_H
#define _H_GL_RECT_H

class GLRect
{
public:
	GLRect();
	GLRect(double _left, double _top, double _right, double _bottom);
	virtual ~GLRect();

public:
	GLRect(const GLRect& rhs);
	GLRect& operator=(const GLRect& rhs);

public:
	double Width() const { return right - left; }
	double Height() const { return bottom - top; }

public:
	bool PtInRect(double x, double y);

public:
	double top;
	double bottom;
	double left;
	double right;
};
#endif