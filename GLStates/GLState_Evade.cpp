#include "GLState_Evade.h"
#include "../GameWorld/GameWorld.h"
#include "../GLSteering/GLSteering_Evade.h"
#include "../GLSteering/GLSteering_Pursuit.h"
#include "../GLSteering/GLSteering_Wander.h"
#include "../GameEntity/Vehicle.h"
#include "../GLConsole/GLConsole.h"

GLState_Evade::GLState_Evade()
: GLState()
{
	SetTitleTexture("images\\button_normal_demo_evade.jpg");
	m_state_name="State_Evade";
}

GLState_Evade::~GLState_Evade()
{

}

void GLState_Evade::Entered(GameWorld *pWorld)
{
	glConsole << "\nState - Evade\n";
	glConsole << "simulate chase - evade predator-prey behavior\n";

	/*
	if(pWorld->GetPrevState()!=this)
	{
		pWorld->GetCamera().Reset();
	}
	*/
	pWorld->RemoveAgents();

	Vehicle* pAgent1=new Vehicle(pWorld);
	pAgent1->SetModel("WalkMech.md2");

	pAgent1->SetMass(1);
	//pAgent1->SetMaxForce(2.0);
	pAgent1->SetMaxSpeed(10.0);
	pAgent1->GetAnimator().SetMaxFPS(50);
	pAgent1->set_position(GLVector(-4, 0, -8));
	pAgent1->EnableSound(false);
	pWorld->AddAgent(pAgent1);

	Vehicle* pAgent2=new Vehicle(pWorld);
	pAgent2->SetModel("WalkMech.md2");

	pAgent2->set_position(GLVector(4, 0, 8));
	pAgent2->SetMass(1);
	//pAgent2->SetMaxForce(2.0);
	pAgent2->SetMaxSpeed(5.0);
	pAgent2->GetAnimator().SetMaxFPS(35);
	pAgent2->EnableSound(false);
	pWorld->AddAgent(pAgent2);

	GLSteering_Evade* pAgentSteering11=new GLSteering_Evade(pAgent1);
	pAgentSteering11->TurnOn();
	pAgentSteering11->SetEvadeTarget(pAgent2);

	GLSteering_Wander* pAgentSteering12=new GLSteering_Wander(pAgent1);
	pAgentSteering12->TurnOn();
	pAgentSteering12->SetWanderDistance(5);
	pAgentSteering12->SetWanderRadius(3);
	pAgentSteering12->SetWanderJitter(1);

	pAgent1->AddSteering("evade", pAgentSteering11);
	pAgent1->AddSteering("wander", pAgentSteering12);

	GLSteering_Pursuit* pAgentSteering21=new GLSteering_Pursuit(pAgent2);
	pAgentSteering21->TurnOn();
	pAgentSteering21->SetTarget(pAgent1);

	GLSteering_Wander* pAgentSteering22=new GLSteering_Wander(pAgent2);
	pAgentSteering22->SetWanderDistance(5);
	pAgentSteering22->SetWanderRadius(3);
	pAgentSteering22->SetWanderJitter(1);
	pAgentSteering22->TurnOn();
	
	pAgent2->AddSteering("pursuit", pAgentSteering21);
	pAgent2->AddSteering("wander", pAgentSteering22);
	pAgent2->SetColor(1.0, 0.0, 0.0);
}

void GLState_Evade::Exited(GameWorld *pWorld)
{
}

void GLState_Evade::Update(GameWorld *pWorld, const long &lElapsedTicks)
{ 
	pWorld->UpdateAgents(lElapsedTicks);
}

GLState_Evade* GLState_Evade::Instance()
{
	static GLState_Evade theInstance;
	return &theInstance;
}

