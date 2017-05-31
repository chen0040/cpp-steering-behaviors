#ifndef _H_GL_FONT_SYS_H
#define _H_GL_FONT_SYS_H
#include <string>
#include "../GLVector/GLVector.h"
#include "../GLExtension/glxs.h"
#include <glut.h>

class GameWorld;

class GLFontSys
{
public:
	static GLFontSys* Instance();
	virtual ~GLFontSys();

private:
	GLFontSys();
	GLFontSys(const GLFontSys& rhs) { }
	GLFontSys& operator=(const GLFontSys& rhs) { return *this; }

public:
	void Init(GameWorld* pWorld);

public:
	void println(std::string text, double x=0, double y=0, double spacing=0.1, void* font=GLUT_BITMAP_HELVETICA_10);
	void print3D(std::string text, void* font=GLUT_STROKE_MONO_ROMAN);

protected:
	void PrePrint2D();
	void PostPrint2D();

public:
	GameWorld* m_pWorld;
};

#define glFont (*(GLFontSys::Instance()))
#endif