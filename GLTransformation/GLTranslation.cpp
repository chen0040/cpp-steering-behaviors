#include "GLTranslation.h"
#include "../GLExtension/glxs.h"
#include <iostream>

void GLTranslation::apply()
{
	glTranslated(x, y, z);
}