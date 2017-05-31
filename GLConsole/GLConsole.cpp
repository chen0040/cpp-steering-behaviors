#include "GLConsole.h"
#include <iostream>

GLConsole::GLConsole()
: m_bConsole(false)
{

}

GLConsole::~GLConsole()
{
}

GLConsole* GLConsole::Instance()
{
	static GLConsole theInstance;
	return &theInstance;
}

GLConsole& GLConsole::operator <<(const char *val)
{
#ifdef GL_CONSOLE_2COUT
	if(m_bConsole)
	{
		std::cout << val;
	}
#endif
	return *this;
}

void GLConsole::EnableConsole(bool bActivated)
{
	m_bConsole=bActivated;
}