#include "GLMath.h"
#include <cstdlib>
#include <cmath>
#include <ctime>

GLMath::GLMath()
{
	srand(static_cast<unsigned int>(time(NULL)));
}

GLMath::~GLMath()
{

}

GLMath* GLMath::Instance()
{
	static GLMath theInstance;
	return &theInstance;
}

double GLMath::PI()
{
	static double PI=3.1415926;
	return PI;
}

double GLMath::Epsilon()
{
	static double eps=10E-5;
	return eps;
}

int GLMath::nextInt(int upper_bound) const
{
	return rand() % upper_bound;
}

double GLMath::nextDouble() const
{
	return static_cast<double>(rand()) / (static_cast<double>(RAND_MAX) + 1.0);
}

double GLMath::nextDouble(double lower_bound, double upper_bound) const
{
	return lower_bound + nextDouble() * (upper_bound - lower_bound);
}