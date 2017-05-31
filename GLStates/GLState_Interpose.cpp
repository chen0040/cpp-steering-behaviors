#include "GLState_Interpose.h"
#include "../GameWorld/GameWorld.h"
#include "../GameEntity/Vehicle.h"
#include "../GLSteering/GLSteering_Wander.h"
#include "../GLSteering/GLSteering_Interpose.h"
#include "../GLConsole/GLConsole.h"
#include <cfloat>

GLState_Interpose::GLState_Interpose()
: GLState()
{
	SetTitleTexture("images\\button_normal_demo_interpose.jpg");
	m_state_name="State_Interpose";
}

void GLState_Interpose::MouseButtonDown(GameWorld* pWorld)
{
	if(pWorld->GetMouse().IsRightButtonDown())
	{
		pWorld->MonitorSelectedAgent();
	}
}

void GLState_Interpose::Entered(GameWorld *pWorld)
{
	glConsole << "\nState - Interpose\n";
	glConsole << "body guard: yellow agent behaviors as body guard and try to intersect on the path between assasin agent and target agent\n";

	pWorld->RemoveAgents();
	pWorld->RemoveObstacles();
	
	Vehicle* pAgent1=CreateWanderAgent(pWorld);
	Vehicle* pAgent2=CreateWanderAgent(pWorld);
	Vehicle* pAgent3=CreateInterposeAgent(pWorld, pAgent1, pAgent2);

	pAgent1->set_position(GLVector(0, 0, 4));
	pAgent2->set_position(GLVector(0, 0, -4));

	pAgent3->set_position(GLVector(-4, 0, 0));

	pWorld->AddAgent(pAgent1);
	pWorld->AddAgent(pAgent2);
	pWorld->AddAgent(pAgent3);
}

Vehicle* GLState_Interpose::CreateWanderAgent(GameWorld* pWorld)
{
	Vehicle* pAgent=new Vehicle(pWorld);
	pAgent->SetModel("WalkMech.md2");
	pAgent->SetMass(1);
	//pAgent->SetMaxForce(2.0);
	pAgent->SetMaxSpeed(10.0);
	pAgent->EnableSound(false);

	GLSteering_Wander* pSteering=new GLSteering_Wander(pAgent);
	pSteering->SetWanderRadius(3);
	pSteering->SetWanderDistance(5);
	pSteering->SetWanderJitter(1);
	pSteering->TurnOn();

	pAgent->AddSteering("wander", pSteering);

	return pAgent;
}

Vehicle* GLState_Interpose::CreateInterposeAgent(GameWorld* pWorld, Vehicle* pAgent1, Vehicle* pAgent2)
{
	Vehicle* pAgent=new Vehicle(pWorld);
	pAgent->SetModel("WalkMech.md2");
	pAgent->SetMass(1);
	//pAgent->SetMaxForce(2.0);
	pAgent->SetMaxSpeed(10.0);
	pAgent->EnableSound(false);

	GLSteering_Interpose* pSteering=new GLSteering_Interpose(pAgent);
	pSteering->SetTarget1(pAgent1);
	pSteering->SetTarget2(pAgent2);

	pSteering->TurnOn();

	pAgent->AddSteering("interpose", pSteering);

	pAgent->SetColor(1.0, 1.0, 0.0);

	return pAgent;
}

void GLState_Interpose::Exited(GameWorld *pWorld)
{
	pWorld->RemoveAgents();
	pWorld->RemoveObstacles();
}

void GLState_Interpose::Update(GameWorld *pWorld, const long &lElapsedTicks)
{
	pWorld->UpdateAgents(lElapsedTicks);
}

GLState_Interpose* GLState_Interpose::Instance()
{
	static GLState_Interpose theInstance;
	return &theInstance;
}

/*
void GLState_Interpose::MouseButtonDown(GameWorld* pWorld)
{
	pWorld->GetCamera().PreTransform();
	pWorld->GetCamera().Transform();
	GLRay mouse_ray=pWorld->GetMouse().GetRay();
	pWorld->GetCamera().PostTransform();

	AgentGroup::iterator pos_agent;
	double min_closest_distance=DBL_MAX;
	GameEntity* pSelected=NULL;
	AgentGroup& agents=pWorld->GetAgents();
	for(pos_agent=agents.begin(); pos_agent != agents.end(); ++pos_agent)
	{
		(*pos_agent)->ShowField(false);
		double closest_distance;
		bool intersected=mouse_ray.intersect((*pos_agent)->GetBoundingSphere(), closest_distance);
		if(intersected)
		{
			if(min_closest_distance > closest_distance)
			{
				min_closest_distance=closest_distance;
				pSelected=*pos_agent;
			}
		}
	}

	if(pSelected != NULL)
	{
		pSelected->ShowField(true);
	}
}
*/