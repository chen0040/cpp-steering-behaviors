#include "GLState_PathFollow.h"
#include "../GameWorld/GameWorld.h"
#include "../GameEntity/Vehicle.h"
#include "../GLSteering/GLSteering_PathFollow.h"
#include "../GLSteering/GLSteering_Wander.h"
#include "../GLConsole/GLConsole.h"

GLState_PathFollow::GLState_PathFollow()
: GLState()
{
	SetTitleTexture("images\\button_normal_demo_path_follow.jpg");
	m_state_name="State_PathFollow";
}

void GLState_PathFollow::Entered(GameWorld* pWorld)
{
	glConsole << "\nState - Path Follow\n";
	glConsole << "agent follow path defined by way points\n";

	/*
	if(pWorld->GetPrevState()!=this)
	{
		pWorld->GetCamera().Reset();
	}
	*/
	pWorld->RemoveAgents();

	double length=pWorld->GetFloor().GetLength();
	double side=length / 5;
	pWorld->GetPath().RandomCreate(10, GLVector(-side, 0, -side), GLVector(side, 0, side));
	pWorld->GetPath().EnableLooping(true);

	Vehicle* pAgent=new Vehicle(pWorld);
	pAgent->SetModel("WalkMech.md2");
	pAgent->SetMass(1);
	//pAgent->SetMaxForce(2.0);
	pAgent->SetMaxSpeed(20);
	pAgent->GetAnimator().SetMaxFPS(100);

	GLSteering_PathFollow* pSteering11=new GLSteering_PathFollow(pAgent);
	pSteering11->TurnOn();
	pSteering11->SetPath(&(pWorld->GetPath()));
	pAgent->AddSteering("path follow", pSteering11);

	GLSteering_Wander* pSteering12=new GLSteering_Wander(pAgent);
	pSteering12->SetWanderRadius(3);
	pSteering12->SetWanderDistance(5);
	pSteering12->SetWanderJitter(1);
	pAgent->AddSteering("wander", pSteering12);

	pWorld->AddAgent(pAgent);
}

void GLState_PathFollow::Exited(GameWorld* pWorld)
{
	pWorld->GetPath().DeletePath();
}

void GLState_PathFollow::Update(GameWorld* pWorld, const long& lElapsedTicks)
{
	pWorld->UpdateAgents(lElapsedTicks);
}

GLState_PathFollow* GLState_PathFollow::Instance()
{
	static GLState_PathFollow theInstance;
	return &theInstance;
}

GLState_PathFollow::~GLState_PathFollow()
{
}