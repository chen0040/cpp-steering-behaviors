#ifndef _H_GL_CONSOLE_H
#define _H_GL_CONSOLE_H

#define GL_CONSOLE_2COUT

class GLConsole
{
public:
	virtual ~GLConsole();
	static GLConsole* Instance();

private:
	GLConsole();
	GLConsole(const GLConsole& rhs) { }
	GLConsole& operator=(const GLConsole& rhs) { return *this; }

public:
	GLConsole& operator << (const char* val);

public:
	void EnableConsole(bool bActivated);

private:
	bool m_bConsole;
};

#define glConsole (*(GLConsole::Instance()))
#endif