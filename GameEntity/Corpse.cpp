#include "Corpse.h"
#include <ctime>
#include <iostream>

Corpse::Corpse(GameWorld* pWorld, double scene_duration)
: Obstacle(pWorld)
, m_scene_duration(scene_duration)
, m_scene_life(0)
{
	this->ShowDot(true);
}

Corpse::~Corpse()
{

}

void Corpse::GenDisplayList_Dot(GLuint& display_list_id)
{
	const int DT=2;
	glNewList(display_list_id, GL_COMPILE);
	glBegin(GL_LINES);
	glVertex2i(-DT, DT);
	glVertex2i(DT, -DT);
	glVertex2i(DT, DT);
	glVertex2i(-DT, -DT);
	glEnd();
	glEndList();
}

void Corpse::Update(const long& lElapsedTicks)
{
	if(m_scene_duration < 0)
	{
		return;
	}

	if(m_scene_life > m_scene_duration)
	{
		this->mark_for_destruction();
		return;
	}

	double elapsed_time=static_cast<double>(lElapsedTicks) / CLOCKS_PER_SEC;

	m_scene_life+=elapsed_time;
}