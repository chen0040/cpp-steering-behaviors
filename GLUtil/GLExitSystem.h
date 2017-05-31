#ifndef _H_GL_EXIT_SYSTEM_H
#define _H_GL_EXIT_SYSTEM_H

#include <fstream>
#include <iostream>

void GLExitSystem(const char* fname, const char* ename);

class GLLogger
{
public:
	virtual ~GLLogger();
	static GLLogger* Instance();

private:
	GLLogger();
	GLLogger(const GLLogger& rhs) { }
	GLLogger& operator=(const GLLogger& rhs) { return *this; }

public:
	std::ofstream m_cout;
};

#define glLogger (GLLogger::Instance()->m_cout)
#endif