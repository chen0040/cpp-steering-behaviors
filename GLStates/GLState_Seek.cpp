#include "GLState_Seek.h"
#include "../GameWorld/GameWorld.h"
#include "../GLSteering/GLSteering_Seek.h"
#include "../GameEntity/Vehicle.h"
#include "../GLConsole/GLConsole.h"
#include <cfloat>

GLState_Seek::GLState_Seek()
: GLState()
{
	SetTitleTexture("images\\button_normal_demo_seek.jpg");
	m_state_name="State_Seek";
}

GLState_Seek::~GLState_Seek()
{

}

void GLState_Seek::Entered(GameWorld* pWorld)
{
	glConsole << "\nState - Seek\n";
	glConsole << "click mouse on the plane to direct the agent to the mouse location\n";

	/*
	if(pWorld->GetPrevState()!=this)
	{
		pWorld->GetCamera().Reset();
	}
	*/
	pWorld->RemoveAgents();
	

	Vehicle* pAgent=new Vehicle(pWorld);
	pAgent->SetModel("WalkMech.md2");
	pAgent->AddSteering("seek", new GLSteering_Seek(pAgent));
	pAgent->SetMass(0.1);
	//pAgent->SetMaxForce(2.0);
	pAgent->SetMaxSpeed(2.0);
	pAgent->GetAnimator().SetMaxFPS(50);
	pWorld->AddAgent(pAgent);

	/*
	pAgent=new GameEntity(pWorld);
	pAgent->SetModel("WalkMech.md2");
	pAgent->set_position(GLVector(4, 0, 0));
	pWorld->AddAgent(pAgent);
	*/
}

void GLState_Seek::Exited(GameWorld* pWorld)
{

}

void GLState_Seek::Update(GameWorld* pWorld, const long& lElapsedTicks)
{
	pWorld->UpdateAgents(lElapsedTicks);
}

GLState_Seek* GLState_Seek::Instance()
{
	static GLState_Seek theInstance;
	return &theInstance;
}

void GLState_Seek::MouseButtonDown(GameWorld* pWorld)
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
			Vehicle* pVehicle=static_cast<Vehicle*>(agents[0]);
			GLSteering_Seek* pSteering=static_cast<GLSteering_Seek*>(pVehicle->GetSteering("seek"));
			pSteering->SetTarget(pt);
			if(!pSteering->On())
			{
				pSteering->TurnOn();
			}
		}
	}
}