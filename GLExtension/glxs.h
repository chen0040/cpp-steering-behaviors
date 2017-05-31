#ifndef _H_GL_XS_H
#define _H_GL_XS_H

#define _GLEW_EXTENSION_H

#ifdef _GLEW_EXTENSION_H
#define NO_SDL_GLEXT
#include "glew.h"
#include <gl/gl.h>
#else
#include <sdl_opengl.h>
#endif

void glxsInit();

class GLXSys
{
public:
	virtual ~GLXSys();
	static GLXSys* Instance();

private:
	GLXSys();
	GLXSys(const GLXSys& rhs) { }
	GLXSys& operator=(const GLXSys& rhs) { return *this; }

public:
	bool IsVBOSupport() const { return m_bVBOSupported;}
protected:
	bool m_bVBOSupported;

	friend void glxsInit();
};

#define glXSys (*(GLXSys::Instance()))

#endif