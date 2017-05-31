#include "GLRect.h"

GLRect::GLRect()
: top(0)
, bottom(20)
, left(0)
, right(200)
{

}

GLRect::GLRect(double _left, double _top, double _right, double _bottom)
: top(_top)
, bottom(_bottom)
, right(_right)
, left(_left)
{

}

GLRect::~GLRect()
{

}

GLRect::GLRect(const GLRect& rhs)
{
	top=rhs.top;
	bottom=rhs.bottom;
	left=rhs.left;
	right=rhs.right;
}

GLRect& GLRect::operator=(const GLRect& rhs)
{
	top=rhs.top;
	bottom=rhs.bottom;
	left=rhs.left;
	right=rhs.right;

	return *this;
}

bool GLRect::PtInRect(double x, double y)
{
	if(x > left && x < right && y > top && y < bottom)
	{
		return true;
	}
	return false;
}