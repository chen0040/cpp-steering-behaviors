#ifndef _H_GL_SYSTEM_H
#define _H_GL_SYSTEM_H

#include <string>

class GLSystem
{
public:
	virtual ~GLSystem();
	static GLSystem* Instance();

private:
	GLSystem();
	GLSystem(const GLSystem& rhs) { }
	GLSystem& operator=(const GLSystem& rhs) { return *this; }

public:
	virtual void execute(const std::string& cmd_exe);
};

#define glSystem (*(GLSystem::Instance()))

#endif