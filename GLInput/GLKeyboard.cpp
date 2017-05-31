#include "GLKeyboard.h"
#include <sdl.h>
#include "../GLExtension/glxs.h"

GLKeyboard::GLKeyboard()
{

}

GLKeyboard::~GLKeyboard()
{

}

void GLKeyboard::KeyUp(const int& iKeyEnum)
{
	m_keydown[iKeyEnum]=false;
	/*
	switch(iKeyEnum)
	{
	case SDLK_LEFT:
		m_keydown[KEYDOWN_LEFT]=false;
		break;
	case SDLK_RIGHT:
		m_keydown[KEYDOWN_RIGHT]=false;
		break;
	case SDLK_UP:
		m_keydown[KEYDOWN_UP]=false;
		break;
	case SDLK_DOWN:
		m_keydown[KEYDOWN_DOWN]=false;
		break;
	case SDLK_a:
		m_keydown[KEYDOWN_A]=false;
		break;
	case SDLK_s:
		m_keydown[KEYDOWN_S]=false;
		break;
	}
	*/
}

void GLKeyboard::KeyDown(const int& iKeyEnum)
{
	m_keydown[iKeyEnum]=true;
	/*
	switch(iKeyEnum)
	{
	case SDLK_LEFT:
		m_keydown[KEYDOWN_LEFT]=true;
		break;
	case SDLK_RIGHT:
		m_keydown[KEYDOWN_RIGHT]=true;
		break;
	case SDLK_UP:
		m_keydown[KEYDOWN_UP]=true;
		break;
	case SDLK_DOWN:
		m_keydown[KEYDOWN_DOWN]=true;
		break;
	case SDLK_a:
		m_keydown[KEYDOWN_A]=true;
		break;
	case SDLK_s:
		m_keydown[KEYDOWN_S]=true;
		break;
	}
	*/
}

bool GLKeyboard::IsKeyDown(const int& iKeyEnum)
{
	std::map<int, bool>::iterator pos_key=m_keydown.find(iKeyEnum);
	if(pos_key != m_keydown.end())
	{
		return pos_key->second;
	}

	return false;
}
