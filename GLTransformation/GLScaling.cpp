#include "GLScaling.h"
#include "../GLExtension/glxs.h"
#include <iostream>

void GLScaling::apply()
{
	glScaled(x, y, z);
}