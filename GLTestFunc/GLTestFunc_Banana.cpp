#include "GLTestFunc_Banana.h"

GLTestFunc_Banana::GLTestFunc_Banana()
{

}

GLTestFunc_Banana* GLTestFunc_Banana::Instance()
{
	static GLTestFunc_Banana theInstance;
	return &theInstance;
}

double GLTestFunc_Banana::GetValue(double x1, double x2)
{
	x1 *= m_scale;
	x2 *= m_scale;
	return 100 * (x2 - x1 * x1) * (x2 - x1 * x1) +(1 - x1) * (1 - x1);
}