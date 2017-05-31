#ifndef _H_GL_TEST_FUNC_INVERSE_MEXICAN_HAT_H
#define _H_GL_TEST_FUNC_INVERSE_MEXICAN_HAT_H

#include "GLTestFunc.h"

class GLTestFunc_InverseMexicanHat : public GLTestFunc
{
public:
	static GLTestFunc_InverseMexicanHat* Instance();
	virtual ~GLTestFunc_InverseMexicanHat() { }

private:
	GLTestFunc_InverseMexicanHat();
	GLTestFunc_InverseMexicanHat(const GLTestFunc_InverseMexicanHat& rhs) { }
	GLTestFunc_InverseMexicanHat& operator=(const GLTestFunc_InverseMexicanHat& rhs) { return *this; }

public:
	virtual double GetValue(double x1, double x2);
};

#define glTestFunc_InverseMexicanHat (*(GLTestFunc_InverseMexicanHat::Instance()))
#endif