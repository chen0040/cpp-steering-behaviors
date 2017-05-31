#include "GLDebris.h"
#include "GLParticleSteering.h"
#include "../GLExtension/glxs.h"
#include <ctime>
#include <glut.h>

GLDebris::GLDebris(GLParticleSteering* pSteering)
: GLParticle(pSteering)
{
	
}

GLDebris::~GLDebris()
{
}

void GLDebris::Render()
{
	glColor3d(m_vColor.x, m_vColor.y, m_vColor.z);

	glPushMatrix();
	glTranslated(m_vPosition.x, m_vPosition.y, m_vPosition.z);

	glRotated(m_vOrientation.x, 1, 0, 0);
	glRotated(m_vOrientation.y, 0, 1, 0);
	glRotated(m_vOrientation.z, 0, 0, 1);

	glScaled(m_vScale.x, m_vScale.y, m_vScale.z);

	glBegin (GL_TRIANGLES);
	glVertex3f (0.0, 0.5, 0.0);
	glVertex3f (-0.25, 0.0, 0.0);
	glVertex3f (0.25, 0.0, 0.0);
	glEnd ();	  

	glPopMatrix();  

	glColor3f(1, 1, 1);
}

void GLDebris::Update(const long& lElapsedTicks)
{
	if(this->IsDead())
	{
		return;
	}

	double elapsed_time=static_cast<double>(lElapsedTicks) / static_cast<double>(CLOCKS_PER_SEC);
	GLVector vAccel=m_pSteering->GetAcceleration(m_vPosition, m_vVelocity);

	m_vVelocity+=vAccel * elapsed_time;

	m_vOrientation+=m_vOrientationVelocity * elapsed_time * 100;
	m_vPosition+=m_vVelocity * elapsed_time;

	m_life -= elapsed_time;
}
