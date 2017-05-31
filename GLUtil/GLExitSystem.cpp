#include "GLExitSystem.h"
#include <string>
#include <sstream>
#include <iostream>

#ifndef _GL_EXIT_SYSTEM_WRITE_FILE
#define _GL_EXIT_SYSTEM_WRITE_FILE
#endif

void GLExitSystem(const char* fname, const char* ename)
{
	std::cerr << "An Error has been detected...\n";
	std::cerr << "Source: " << fname << std::endl;
	std::cerr << "Error: " << ename << std::endl;

#ifdef _GL_EXIT_SYSTEM_WRITE_FILE
	glLogger << "An Error has been detected...\n";
	glLogger << "Source: " << fname << std::endl;
	glLogger << "Error: " << ename << std::endl;
#endif

	std::exit(0);
}

GLLogger::GLLogger()
{
	m_cout.open("log.txt");
}

GLLogger::~GLLogger()
{
	m_cout.close();
}

GLLogger* GLLogger::Instance()
{
	static GLLogger theInstance;
	return &theInstance;
}