#include "GLTestFunc_InverseMexicanHat.h"
#include <cmath>

GLTestFunc_InverseMexicanHat::GLTestFunc_InverseMexicanHat()
{

}

double GLTestFunc_InverseMexicanHat::GetValue(double x1, double x2)
{
	x1*=m_scale;
	x2*=m_scale;
	return - (1 - x1 * x1 / 4 - x2 * x2 / 4) * exp(- x1 * x1 / 8 - x2 * x2 / 8);
}

GLTestFunc_InverseMexicanHat* GLTestFunc_InverseMexicanHat::Instance()
{
	static GLTestFunc_InverseMexicanHat theInstance;
	return &theInstance;
}