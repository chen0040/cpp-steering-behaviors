#include "GLFontSys.h"
#include "../GameWorld/GameWorld.h"
#include "../GLUtil/GLExitSystem.h"

GLFontSys::GLFontSys()
: m_pWorld(NULL)
{

}

GLFontSys::~GLFontSys()
{

}

GLFontSys* GLFontSys::Instance()
{
	static GLFontSys theInstance;
	return &theInstance;
}

void GLFontSys::Init(GameWorld* pWorld)
{
	m_pWorld=pWorld;
}

void GLFontSys::println(std::string text, double x, double y, double spacing, void* font)
{
	if(m_pWorld == NULL)
	{
		GLExitSystem("GLFontSys::println(std::string text, double x, double y, double spacing, void* font)", "m_pWorld==NULL");
	}

	PrePrint2D();

	double x1=x;
	double y1=y;
	for(size_t i=0; i<text.length(); i++)
	{
		char c=text[i];
		glRasterPos2d(x1, y1);
		if(c != '\n')
		{
			glutBitmapCharacter(font, (int)c);
			x1+=glutBitmapWidth(font, (int)c)+spacing;
		}
		else
		{
			x1=x;
			y1+=15;
		}
	}

	PostPrint2D();
}

void GLFontSys::print3D(std::string text, void* font)
{
	glPushMatrix();
	glScaled(0.01, 0.01, 0.01);
	for(size_t i=0; i<text.length(); i++)
	{
		glutStrokeCharacter(font, (int)(text[i]));
	}
	glPopMatrix();
}

void GLFontSys::PrePrint2D()
{
	//glEnable(GL_COLOR_MATERIAL);
	glDisable(GL_CULL_FACE);
	glDisable(GL_LIGHTING);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, m_pWorld->GetWidth(), 0, m_pWorld->GetHeight());

	glScalef(1, -1, 1);
	glTranslatef(0, -m_pWorld->GetHeight(), 0);

	glMatrixMode(GL_MODELVIEW);
}

void GLFontSys::PostPrint2D()
{
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_LIGHTING);
	glEnable(GL_CULL_FACE);
	//glDisable(GL_COLOR_MATERIAL);
}

