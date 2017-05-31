#include "GLState_Blank.h"
#include "../GameWorld/GameWorld.h"
#include "../GLConsole/GLConsole.h"
#include <cfloat>
#include "../GLFont/GLFontSys.h"
#include "../GLTexture/GLTextureManager.h"
#include "../GameEntity/Obstacle.h"

GLState_Blank::GLState_Blank()
: GLState()
{
	m_state_name="State_Blank";
}

GLState_Blank::~GLState_Blank()
{

}

void GLState_Blank::Entered(GameWorld* pWorld)
{
	pWorld->GetCamera().Reset();
	pWorld->GetCamera().Lock();

	Obstacle* pObs=new Obstacle(pWorld);
	pObs->SetColor(0.5, 0.5, 0.5);
	pObs->SetModel("ufo.3ds");

	pWorld->AddObstacle(pObs);
}

void GLState_Blank::Exited(GameWorld* pWorld)
{
	pWorld->GetCamera().Unlock();

	pWorld->RemoveObstacles();
	pWorld->RemoveAgents();
}

void GLState_Blank::Update(GameWorld* pWorld, const long& lElapsedTicks)
{
	pWorld->UpdateObstacles(lElapsedTicks);
}

void GLState_Blank::Render(GameWorld* pWorld)
{
	GLState::Render(pWorld);
	glFont.println("Start By Clicking on the buttons...", 400, 300);
}

GLState_Blank* GLState_Blank::Instance()
{
	static GLState_Blank theInstance;
	return &theInstance;
}

void GLState_Blank::MouseButtonDown(GameWorld* pWorld)
{
	
}