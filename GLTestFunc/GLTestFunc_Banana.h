#ifndef _H_GL_TEST_FUNCTION_BANANA_H
#define _H_GL_TEST_FUNCTION_BANANA_H

#include "GLTestFunc.h"

class GLTestFunc_Banana : public GLTestFunc
{
public:
	static GLTestFunc_Banana* Instance();
	virtual ~GLTestFunc_Banana() { }

public:
	virtual double GetValue(double x1, double x2);

private:
	GLTestFunc_Banana();
	GLTestFunc_Banana(GLTestFunc_Banana& rhs)  { }
	GLTestFunc_Banana& operator=(GLTestFunc_Banana& rhs) { return *this; }

};

#define glTestFunc_Banana (*(GLTestFunc_Banana::Instance()))

#endif