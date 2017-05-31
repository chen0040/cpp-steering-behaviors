#ifndef _H_GL_TEST_FUNC_H
#define _H_GL_TEST_FUNC_H

class GLTestFunc
{
public:
	GLTestFunc();
	virtual ~GLTestFunc();

protected:
	double m_scale;
public:
	void SetScale(double scale) { m_scale=scale; }

public:
	virtual double GetValue(double x1, double x2)=0;
};
#endif