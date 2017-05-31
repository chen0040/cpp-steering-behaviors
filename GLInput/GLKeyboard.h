#ifndef _H_GL_INPUT_H
#define _H_GL_INPUT_H

#include <map>

class GLKeyboard
{
public:
	GLKeyboard();
	virtual ~GLKeyboard();

public:
	virtual void KeyUp(const int& iKeyEnum);
	virtual void KeyDown(const int& iKeyEnum);

public:
	bool IsKeyDown(const int& iKeyEnum);

protected:
	std::map<int, bool> m_keydown;
};
#endif