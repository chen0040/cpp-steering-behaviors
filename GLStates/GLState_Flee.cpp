#include "GLState_Flee.h"
#include "../GameWorld/GameWorld.h"
#include "../GameEntity/Vehicle.h"
#include "../GLSteering/GLSteering_Flee.h"
#include "../GLConsole/GLConsole.h"

GLState_Flee::GLState_Flee()
: GLState()
{
	SetTitleTexture("images\\button_normal_demo_flee.jpg");
	m_state_name="State_Flee";
}

GLState_Flee::~GLState_Flee()
{

}

void GLState_Flee::Entered(GameWorld* pWorld)
{
	glConsole << "\nState - Flee\n";
	glConsole << "simulate run away behavior\n";

	/*
	if(pWorld->GetPrevState()!=this)
	{
		pWorld->GetCamera().Reset();
	}
	*/

	pWorld->RemoveAgents();

	Vehicle* pAgent=new Vehicle(pWorld);
	pAgent->SetModel("WalkMech.md2");
	
	pAgent->SetMass(0.1);
	//pAgent->SetMaxForce(2.0);
	pAgent->SetMaxSpeed(2.0);

	pAgent->AddSteering("flee", new GLSteering_Flee(pAgent));

	pWorld->AddAgent(pAgent);
}

void GLState_Flee::Update(GameWorld *pWorld, const long &lElapsedTicks)
{
	pWorld->UpdateAgents(lElapsedTicks);
}

void GLState_Flee::Exited(GameWorld* pWorld)
{
	
}

void GLState_Flee::MouseButtonDown(GameWorld *pWorld)
{
	GLCamera& game_camera=pWorld->GetCamera();
	GLMouse& game_mouse=pWorld->GetMouse();
	GLFloor& floor=pWorld->GetFloor();

	if(game_mouse.IsLeftButtonDown())
	{
		game_camera.PreTransform();
		game_camera.Transform();
		GLRay mouse_ray=game_mouse.GetRay();
		game_camera.PostTransform();

		GLVector pt;
		bool intersected=mouse_ray.intersect(&floor, pt);

		AgentGroup& agents=pWorld->GetMutableAgents();
		if(intersected)
		{
			Vehicle* pVehicle=agents[0];
			GLSteering_Flee* pSteering=static_cast<GLSteering_Flee*>(pVehicle->GetSteering("flee"));
			pSteering->SetTarget(pt);
			if(!pSteering->On())
			{
				pSteering->TurnOn();
			}
		}
	}
}

GLState_Flee* GLState_Flee::Instance()
{
	static GLState_Flee theInstance;
	return &theInstance;
}