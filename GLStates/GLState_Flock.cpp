#include "GLState_Flock.h"
#include "../GLMath/GLMath.h"
#include "../GameEntity/Vehicle.h"
#include "../GameWorld/GameWorld.h"
#include "../GLSteering/GLSteering_Separation.h"
#include "../GLSteering/GLSteering_Alignment.h"
#include "../GLSteering/GLSteering_Cohesion.h"
#include "../GLSteering/GLSteering_Wander.h"
#include "../GLSteering/GLSteering_Evade.h"
#include "../GLSteering/GLSteering_Pursuit.h"
//#include "../GLSteering/GLSteering_WallAvoidance.h"
#include "../GLRole/GLRole_Predator.h"
#include "../GLRole/GLRole_Prey.h"
#include "../GLConsole/GLConsole.h"
#include <set>

GLState_Flock::GLState_Flock()
: GLState()
{
	SetTitleTexture("images\\button_normal_demo_flock.jpg");
	m_state_name="State_Flock";
}

void GLState_Flock::Entered(GameWorld* pWorld)
{
	glConsole << "\nState - Flock\n";
	glConsole << "simulate flock behavior : alignment, separation, cohesion\n";
	glConsole << "Two group of prey: white flock and yellow flock\n";
	glConsole << "Two predator: red predator on white flock, green predator on yellow flock\n";
	glConsole << "prey behavior: wander, alignment, separation, cohesion, evate from predator\n";
	glConsole << "predator behavior: wander, pursuit prey\n";

	pWorld->RemoveAgents();
	pWorld->RemoveObstacles();

	Vehicle* predator1=CreatePredator(pWorld);
	predator1->SetColor(1.0, 0.0, 0.0);
	predator1->SetTypeId(PREDATOR1);

	GLRole_Predator* role_predator1=new GLRole_Predator(predator1);
	role_predator1->AddPreyId(FLOCK1);
	predator1->AddRole("predator_role", role_predator1);

	GLSteering_Pursuit* pursuit_predator1=new GLSteering_Pursuit(predator1);
	pursuit_predator1->TurnOn();
	role_predator1->SetPursuitSteeringHandler(pursuit_predator1);
	predator1->AddSteering("pursuit", pursuit_predator1);

	pWorld->AddAgent(predator1);

	int iPreyCount1=pWorld->IsHighEndSystem() ? 9 : 6;
	for(int i=0; i<iPreyCount1; ++i)
	{
		Vehicle* pPrey=CreateAgent(pWorld);
		pPrey->SetTypeId(FLOCK1);

		GLRole_Prey* role_prey=new GLRole_Prey(pPrey);
		role_prey->AddPredatorId(PREDATOR1);
		pPrey->AddRole("prey_role", role_prey);

		GLSteering_Evade* evade=new GLSteering_Evade(pPrey);
		evade->TurnOn();
		role_prey->SetEvadeSteeringHandler(evade);
		pPrey->AddSteering("evade", evade);

		pWorld->AddAgent(pPrey);
	}

	Vehicle* predator2=CreatePredator(pWorld);
	predator2->SetColor(0.0, 1.0, 0.0);
	predator2->SetTypeId(PREDATOR2);

	GLRole_Predator* role_predator2=new GLRole_Predator(predator2);
	role_predator2->AddPreyId(FLOCK2);
	predator2->AddRole("predator_role", role_predator2);

	GLSteering_Pursuit* pursuit_predator2=new GLSteering_Pursuit(predator2);
	role_predator2->SetPursuitSteeringHandler(pursuit_predator2);
	pursuit_predator2->TurnOn();

	predator2->AddSteering("pursuit", pursuit_predator2);

	pWorld->AddAgent(predator2);

	int iPreyCount2=pWorld->IsHighEndSystem() ? 6 : 3;
	for(int i=0; i<iPreyCount2; i++)
	{
		Vehicle* pPrey=CreateAgent(pWorld);
		pPrey->SetTypeId(FLOCK2);
		pPrey->SetColor(1.0, 1.0, 0.0);

		GLRole_Prey* role_prey=new GLRole_Prey(pPrey);
		role_prey->AddPredatorId(PREDATOR2);
		pPrey->AddRole("prey_role", role_prey);

		GLSteering_Evade* evade=new GLSteering_Evade(pPrey);
		role_prey->SetEvadeSteeringHandler(evade);
		evade->TurnOn();
		pPrey->AddSteering("evade", evade);

		pWorld->AddAgent(pPrey);
	}
}

Vehicle* GLState_Flock::CreatePredator(GameWorld* pWorld)
{
	Vehicle* pAgent=new Vehicle(pWorld);
	//pAgent->SetMaxForce(6);
	pAgent->SetMass(1);
	pAgent->SetMaxSpeed(16);
	pAgent->SetModel("WalkMech.md2");
	pAgent->GetAnimator().SetMaxFPS(64);
	pAgent->EnableSound(false);

	/*
	GLSteering_WallAvoidance* wallAvoidance=new GLSteering_WallAvoidance(pAgent);
	wallAvoidance->SetWeight(10.0);
	wallAvoidance->SetFeelerLength(40);
	wallAvoidance->TurnOn();
	pAgent->AddSteering("wall avoidance", wallAvoidance);
	*/

	GLSteering_Wander* wander=new GLSteering_Wander(pAgent);
	wander->SetWeight(1);
	wander->SetWanderDistance(5.0);
	wander->SetWanderRadius(3.0);
	wander->SetWanderJitter(1.0);
	wander->TurnOn();
	pAgent->AddSteering("wander", wander);

	double side=-pWorld->GetFloor().GetLength() / 2;

	pAgent->set_position(GLVector(glMath.nextDouble(-side, side), 0, glMath.nextDouble(-side, side)));

	return pAgent;

}

void GLState_Flock::Exited(GameWorld* pWorld)
{
	pWorld->RemoveAgents();
	pWorld->RemoveObstacles();
}

void GLState_Flock::Update(GameWorld* pWorld, const long& lElapsedTicks)
{
	pWorld->UpdateAgents(lElapsedTicks);
}

void GLState_Flock::MouseButtonDown(GameWorld* pWorld)
{
	if(pWorld->GetMouse().IsRightButtonDown())
	{
		pWorld->MonitorSelectedAgent();
	}
}

Vehicle* GLState_Flock::CreateAgent(GameWorld* pWorld)
{
	Vehicle* pAgent=new Vehicle(pWorld);
	//pAgent->SetMaxForce(10);
	pAgent->SetMass(1);
	pAgent->SetMaxSpeed(10);
	pAgent->SetModel("WalkMech.md2");
	pAgent->GetAnimator().SetMaxFPS(64);
	pAgent->EnableSound(false);

	/*
	GLSteering_WallAvoidance* wallAvoidance=new GLSteering_WallAvoidance(pAgent);
	wallAvoidance->SetWeight(10.0);
	wallAvoidance->SetFeelerLength(40);
	wallAvoidance->TurnOn();
	pAgent->AddSteering("wall avoidance", wallAvoidance);
	*/

	GLSteering_Separation* separation=new GLSteering_Separation(pAgent);
	separation->SetWeight(3.0);
	separation->TurnOn();
	pAgent->AddSteering("separation", separation);
	
	GLSteering_Alignment* alignment=new GLSteering_Alignment(pAgent);
	alignment->SetWeight(1.0);
	alignment->TurnOn();
	pAgent->AddSteering("alignment", alignment);

	GLSteering_Cohesion* cohesion=new GLSteering_Cohesion(pAgent);
	cohesion->SetWeight(1.0);
	cohesion->TurnOn();
	pAgent->AddSteering("cohesion", cohesion);
	
	GLSteering_Wander* wander=new GLSteering_Wander(pAgent);
	wander->SetWeight(1);
	wander->SetWanderDistance(5.0);
	wander->SetWanderRadius(3.0);
	wander->SetWanderJitter(1.0);
	wander->TurnOn();
	pAgent->AddSteering("wander", wander);


	pAgent->SetVisibleDistance(40);

	double side=-pWorld->GetFloor().GetLength() / 2;


	pAgent->set_position(GLVector(glMath.nextDouble(-side, side), 0, glMath.nextDouble(-side, side)));
	
	return pAgent;
}

GLState_Flock* GLState_Flock::Instance()
{
	static GLState_Flock theInstance;
	return &theInstance;
}