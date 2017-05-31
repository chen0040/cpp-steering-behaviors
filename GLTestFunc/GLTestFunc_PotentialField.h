#ifndef _H_GL_TEST_FUNC_POTENTIAL_FIELD_H
#define _H_GL_TEST_FUNC_POTENTIAL_FIELD_H

#include "GLTestFunc.h"

class GLTestFunc_PotentialField : public GLTestFunc
{
public:
	static GLTestFunc_PotentialField* Instance();
	virtual ~GLTestFunc_PotentialField() { }

private:
	GLTestFunc_PotentialField();
	GLTestFunc_PotentialField(const GLTestFunc_PotentialField& rhs) { }
	GLTestFunc_PotentialField& operator=(const GLTestFunc_PotentialField& rhs) { return *this; }

public:
	virtual double GetValue(double x1, double x2);
};

#define glTestFunc_PotentialField (*(GLTestFunc_PotentialField::Instance()))
#endif