#include "GLCube.h"
#include <cfloat>

GLCube::GLCube()
: minx(DBL_MAX)
, miny(DBL_MAX)
, minz(DBL_MAX)
, maxx(-DBL_MAX)
, maxy(-DBL_MAX)
, maxz(-DBL_MAX)
{
}

void GLCube::Expand(const GLVector& pt)
{
	if(minx > pt.x)
	{
		minx=pt.x;
	}
	if(maxx < pt.x)
	{
		maxx=pt.x;
	}
	if(miny > pt.y)
	{
		miny=pt.y;
	}
	if(maxy < pt.y)
	{
		maxy=pt.y;
	}
	if(minz > pt.z)
	{
		minz=pt.z;
	}
	if(maxz < pt.z)
	{
		maxz=pt.z;
	}
}

void GLCube::Expand(double amount)
{
	maxx+=amount;
	minx-=amount;

	maxy+=amount;
	miny-=amount;

	maxz+=amount;
	minz-=amount;
}