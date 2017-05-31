#ifndef _H_GL_MATH_H
#define _H_GL_MATH_H

class GLMath
{
public:
	virtual ~GLMath();
	static GLMath* Instance();

public:
	static double PI();
	static double Epsilon();
	double nextDouble() const;
	double nextDouble(double lower_bound, double upper_bound) const;
	int nextInt(int upper_bound) const;

public:
	double toDegree(double rad) { return rad * 180.0 / 3.1415926; }


private:
	GLMath();
	GLMath(const GLMath& rhs) { }
	GLMath& operator=(const GLMath& rhs) { return *this; }
};

#define glMath (*(GLMath::Instance()))
#endif