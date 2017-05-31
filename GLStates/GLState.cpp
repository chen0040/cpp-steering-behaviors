#include "GLState.h"
#include "../GameWorld/GameWorld.h"
#include "../GLTexture/GLTextureManager.h"

GLState::GLState()
{
	const int LEFT=20;
	const int TOP=20;
	const int WIDTH=120;
	const int HEIGHT=40;
	const int BOTTOM=TOP + HEIGHT;
	const int RIGHT=LEFT + WIDTH;

	m_title.set_p1(GLVector(LEFT, TOP, 0));
	m_title.set_p2(GLVector(RIGHT, TOP, 0));
	m_title.set_p3(GLVector(RIGHT, BOTTOM, 0));
	m_title.set_p4(GLVector(LEFT, BOTTOM, 0));

	m_state_name="State_Unknown";
}

GLState::~GLState()
{

}

void GLState::SetTitleTexture(const char* fname)
{
	m_title.SetTexture(glTextures.GetTexture(fname));
}

void GLState::Render(GameWorld *pWorld)
{
	pWorld->Render3DWorld();

	if(m_title.GetTexture() == NULL)
	{
		return;
	}

	glDisable(GL_LIGHTING);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, pWorld->GetWidth(), pWorld->GetHeight(), 0);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glEnable(GL_BLEND);
	glDisable(GL_CULL_FACE);
	glColor4f(1.0f, 1.0f, 1.0f, 0.5);

	m_title.PreRender();
	m_title.Render();
	m_title.PostRender();

	glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glEnable(GL_LIGHTING);
}
