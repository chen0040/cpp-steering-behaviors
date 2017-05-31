#include "GLState_AvoidSeek.h"
#include "../GameWorld/GameWorld.h"
#include "../GLSteering/GLSteering_Seek.h"
#include "../GLSteering/GLSteering_Avoidance.h"
#include "../GameEntity/Vehicle.h"
#include "../GameEntity/Obstacle.h"
#include "../GLMath/GLMath.h"
#include "../GLConsole/GLConsole.h"
#include <cfloat>

GLState_AvoidSeek::GLState_AvoidSeek()
: GLState()
{
	SetTitleTexture("images\\button_normal_demo_avoidseek.jpg");
	m_state_name="State_AvoidSeek";
}

GLState_AvoidSeek::~GLState_AvoidSeek()
{

}

void GLState_AvoidSeek::Entered(GameWorld* pWorld)
{
	glConsole << "\nState - Avoid Seek\n";
	glConsole << "left click mouse on the plane to direct the agent the mouse location\n";
	glConsole << "right click mouse on the plane to generate block\n";

	pWorld->RemoveAgents();
	pWorld->RemoveObstacles();

	Vehicle* pAgent=new Vehicle(pWorld);
	pAgent->SetModel("WalkMech.md2");
	pAgent->SetMass(0.1);
	//pAgent->SetMaxForce(2.0);
	pAgent->SetMaxSpeed(10.0);
	pAgent->GetAnimator().SetMaxFPS(50);

	GLSteering_Avoidance* avoid=new GLSteering_Avoidance(pAgent);
	avoid->SetWeight(10.0);
	// avoid->SetMinDetectionBoxLength(4.0);
	avoid->TurnOn();
	pAgent->AddSteering("avoid", avoid);

	GLSteering_Seek* seek=new GLSteering_Seek(pAgent);
	seek->SetWeight(1.0);
	seek->TurnOn();
	pAgent->AddSteering("seek", seek);

	pWorld->AddAgent(pAgent);
}

Obstacle* GLState_AvoidSeek::CreateObstacle(GameWorld* pWorld)
{
	double side=pWorld->GetFloor().GetLength() / 2.0;

	Obstacle* pObs=new Obstacle(pWorld);
	pObs->set_bound_radius(glMath.nextDouble(2, 4));
	pObs->set_position(GLVector(glMath.nextDouble(-side, side), 0, glMath.nextDouble(-side, side)));

	return pObs;
}

void GLState_AvoidSeek::Exited(GameWorld* pWorld)
{
	pWorld->RemoveObstacles();
	pWorld->RemoveAgents();
}

void GLState_AvoidSeek::Update(GameWorld* pWorld, const long& lElapsedTicks)
{
	pWorld->UpdateAgents(lElapsedTicks);
}

GLState_AvoidSeek* GLState_AvoidSeek::Instance()
{
	static GLState_AvoidSeek theInstance;
	return &theInstance;
}

void GLState_AvoidSeek::MouseButtonDown(GameWorld* pWorld)
{
	GLCamera& game_camera=pWorld->GetCamera();
	GLMouse& game_mouse=pWorld->GetMouse();
	GLFloor& floor=pWorld->GetFloor();
	AgentGroup& agents=pWorld->GetMutableAgents();

	game_camera.PreTransform();
	game_camera.Transform();
	GLRay mouse_ray=game_mouse.GetRay();
	game_camera.PostTransform();

	if(game_mouse.IsLeftButtonDown())
	{
		GLVector pt;
		bool intersected=mouse_ray.intersect(&floor, pt);

		if(intersected)
		{
			Vehicle* pVehicle=static_cast<Vehicle*>(agents[0]);
			GLSteering_Seek* pSteering=static_cast<GLSteering_Seek*>(pVehicle->GetSteering("seek"));
			pSteering->SetTarget(pt);
		}
	}
	if(game_mouse.IsRightButtonDown())
	{
		GLVector intersection_point;
		if(mouse_ray.intersect(&floor, intersection_point))
		{
			Obstacle* pObs=CreateObstacle(pWorld);
			pObs->set_position(intersection_point);
			pWorld->AddObstacle(pObs);
		}
	}
}