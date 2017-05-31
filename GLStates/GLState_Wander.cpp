#include "GLState_Wander.h"
#include "../GameWorld/GameWorld.h"
#include "../GameEntity/Vehicle.h"
#include "../GLSteering/GLSteering_Wander.h"
#include "../GLConsole/GLConsole.h"

GLState_Wander::GLState_Wander()
: GLState()
{
	SetTitleTexture("images\\button_normal_demo_wander.jpg");
	m_state_name="State_Wander";
}

GLState_Wander::~GLState_Wander()
{

}

GLState_Wander* GLState_Wander::Instance()
{
	static GLState_Wander theInstance;
	return &theInstance;
}

void GLState_Wander::Entered(GameWorld* pWorld)
{
	glConsole << "\nState - Bezier\n";
	glConsole << "agent auto-generate random locomotion to enable self-navigation behavior\n";

	/*
	if(pWorld->GetPrevState()!=this)
	{
		pWorld->GetCamera().Reset();
	}
	*/
	pWorld->RemoveAgents();

	Vehicle* pAgent=new Vehicle(pWorld);

	pAgent->SetModel("WalkMech.md2");

	GLSteering_Wander* pSteering = new GLSteering_Wander(pAgent);
	pAgent->AddSteering("wander", pSteering);
	pSteering->TurnOn();
	pSteering->SetWanderDistance(2);
	pSteering->SetWanderJitter(1);
	pSteering->SetWanderRadius(3);
	pAgent->GetAnimator().SetMaxFPS(50);

	pAgent->SetMass(1);
	//pAgent->SetMaxForce(2.0);
	pAgent->SetMaxSpeed(10.0);

	pWorld->AddAgent(pAgent);

}

void GLState_Wander::Exited(GameWorld* pWorld)
{	
	
}

void GLState_Wander::Update(GameWorld* pWorld, const long& lElapsedTicks)
{
	pWorld->UpdateAgents(lElapsedTicks);
}

void GLState_Wander::MouseButtonDown(GameWorld *pWorld)
{

}

