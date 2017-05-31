#include "glxs.h"
#include "../GLUtil/GLExitSystem.h"
#include <iostream>

GLXSys::GLXSys()
: m_bVBOSupported(false)
{

}

GLXSys::~GLXSys()
{

}

GLXSys* GLXSys::Instance()
{
	static GLXSys theInstance;
	return &theInstance;
}

void glxsInit()
{
#ifdef _GLEW_EXTENSION_H
	GLenum err=glewInit();
	if(err!=GLEW_OK)
	{
		GLExitSystem("glxsInit()", "Failed in calling glewInit()");
	}

	std::cout << "Glew Version: " << glewGetString(GLEW_VERSION) << std::endl;

	if(GLEW_VERSION_1_5)
	{
		glXSys.m_bVBOSupported=true;
		std::cout << "This PC has support for GL VERSION_1_5\n";
		std::cout << "VBO rendering enabled\n";
	}
	else
	{
		glXSys.m_bVBOSupported=false;
		std::cout << "This PC has no support for GL VERSION_1_5\n";
		std::cout << "VBO rendering disabled\n";
	}
#endif

	
}