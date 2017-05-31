#include "GLState_Load.h"
#include "../GameWorld/GameWorld.h"
#include "../GLConsole/GLConsole.h"
#include <cfloat>
#include "../GLFont/GLFontSys.h"
#include "../GLTexture/GLTextureManager.h"

GLState_Load::GLState_Load()
: GLState()
, m_bSetup(false)
{
	m_state_name="State_Load";
}

GLState_Load::~GLState_Load()
{

}

void GLState_Load::Entered(GameWorld* pWorld)
{
	glConsole << "\nState - Load\n";
}

void GLState_Load::Exited(GameWorld* pWorld)
{

}

void GLState_Load::Update(GameWorld* pWorld, const long& lElapsedTicks)
{
	if(!m_bSetup)
	{
		int LEFT=0;
		int TOP=0;
		int BOTTOM=TOP + pWorld->GetHeight();
		int RIGHT=LEFT + pWorld->GetWidth();;

		m_title.set_p1(GLVector(LEFT, TOP, 0));
		m_title.set_p2(GLVector(RIGHT, TOP, 0));
		m_title.set_p3(GLVector(RIGHT, BOTTOM, 0));
		m_title.set_p4(GLVector(LEFT, BOTTOM, 0));

		m_title.SetTexture(glTextures.GetTexture("plane.jpg"));
		m_bSetup=true;
	}
}


void GLState_Load::Render(GameWorld* pWorld)
{
	glDisable(GL_LIGHTING);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, pWorld->GetWidth(), pWorld->GetHeight(), 0);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glDisable(GL_CULL_FACE);

	m_title.PreRender();
	m_title.Render();
	m_title.PostRender();

	glEnable(GL_CULL_FACE);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glEnable(GL_LIGHTING);
}

GLState_Load* GLState_Load::Instance()
{
	static GLState_Load theInstance;
	return &theInstance;
}

void GLState_Load::MouseButtonDown(GameWorld* pWorld)
{
	
}