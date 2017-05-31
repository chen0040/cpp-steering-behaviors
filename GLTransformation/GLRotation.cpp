#include "GLRotation.h"
#include "../GLExtension/glxs.h"
#include <iostream>

void GLRotation::apply()
{
	glRotated(angle_in_degree, x, y, z);
}