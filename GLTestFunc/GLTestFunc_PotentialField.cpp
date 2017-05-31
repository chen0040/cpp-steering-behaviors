#include "GLTestFunc_PotentialField.h"
#include <cmath>

GLTestFunc_PotentialField::GLTestFunc_PotentialField()
{

}

double GLTestFunc_PotentialField::GetValue(double x1, double x2)
{
	x1*=m_scale;
	x2*=m_scale;
	double d=sqrt(x1*x1 + x2*x2);
	if(d==0) d=1E-10;

	return -1/d;
}

GLTestFunc_PotentialField* GLTestFunc_PotentialField::Instance()
{
	static GLTestFunc_PotentialField theInstance;
	return &theInstance;
}