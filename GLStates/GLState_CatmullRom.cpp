#include "GLState_CatmullRom.h"
#include "../GameWorld/GameWorld.h"
#include "../GLUI/GLCommandPanel.h"
#include "../GLUI/GLBorderLayout.h"
#include "../GLUI/GLFlowLayout.h"
#include "../GLUI/GLCommand.h"
#include "../GLTexture/GLTextureManager.h"
#include "../EventHandler/EventHandler_ClearCurve.h"
#include "../GameEntity/ConstrainedAgent.h"
#include "../GLMath/GLMath.h"
#include "../GLSteering/GLSteering_Separation.h"
#include "../GLSteering/GLSteering_Alignment.h"
#include "../GLSteering/GLSteering_Cohesion.h"
#include "../GLSteering/GLSteering_Wander.h"
#include "../GLSteering/GLSteering_OffsetPursuit.h"
#include "../GLConsole/GLConsole.h"
#include <iostream>

GLState_CatmullRom::GLState_CatmullRom()
: GLState()
, m_pCommands(NULL)
{
	SetTitleTexture("images\\button_normal_demo_catmull_rom.jpg");
	m_state_name="State_CatmullRom";
}

GLState_CatmullRom::~GLState_CatmullRom()
{
	if(m_pCommands != NULL)
	{
		delete m_pCommands;
		m_pCommands=NULL;
	}
}

GLState_CatmullRom* GLState_CatmullRom::Instance()
{
	static GLState_CatmullRom theInstance;
	return &theInstance;
}

void GLState_CatmullRom::Entered(GameWorld* pWorld)
{
	glConsole << "\nState - Catmull Rom\n";
	glConsole << "click mouse on the plane to generates points for Catmull Rom curve\n";

	SetupUI(pWorld);
	m_curve.Clear();
	pWorld->RemoveAgents();

	ConstrainedAgent* pLeader=CreateConstrainedAgent(pWorld);
	pWorld->AddAgent(pLeader);
	for(int i=0;  i< 5; i++)
	{
		Vehicle* pAgent=CreateFlockAgent(pWorld);

		GLVector offset=CreateOffset(pLeader, pAgent, pWorld);
		pAgent->set_position(pLeader->LocalToWorldCoordinate(offset));

		pWorld->AddAgent(pAgent);
	}
	
}

GLVector GLState_CatmullRom::CreateOffset(Vehicle* pLeader, Vehicle* pFollower, GameWorld* pWorld)
{
	GLVector offset;

	offset=GLVector(glMath.nextDouble(-1, 1), 0, glMath.nextDouble(-1, 1));
	offset=offset.normalize() * (pLeader->GetBoundingSphere().radius + pFollower->GetBoundingSphere().radius);

	return offset;
}

ConstrainedAgent* GLState_CatmullRom::CreateConstrainedAgent(GameWorld* pWorld)
{
	ConstrainedAgent* pAgent=new ConstrainedAgent(pWorld);
	pAgent->SetSpeed(10);
	pAgent->SetPath(&m_curve);
	pAgent->SetModel("WalkMech.md2");

	double side=pWorld->GetFloor().GetLength() / 3;
	pAgent->set_position(GLVector(glMath.nextDouble(-side, side), 0, glMath.nextDouble(-side, side)));

	return pAgent;
}

ConstrainedAgent* GLState_CatmullRom::CreateFlockAgent(GameWorld* pWorld)
{
	ConstrainedAgent* pAgent=new ConstrainedAgent(pWorld);
	pAgent->SetSpeed(10);
	pAgent->SetModel("WalkMech.md2");

	double side=pWorld->GetFloor().GetLength() / 3;
	pAgent->set_position(GLVector(glMath.nextDouble(-side, side), 0, glMath.nextDouble(-side, side)));

	return pAgent;
}

void GLState_CatmullRom::SetupUI(GameWorld* pWorld)
{
	if(m_pCommands == NULL)
	{
		GLBorderLayout* pCommandLayout=new GLBorderLayout();
		pCommandLayout->SetSouthProp(0.15);
		pCommandLayout->SetWestProp(0.17);
		m_pCommands=new GLCommandPanel(pCommandLayout);
		m_pCommands->SetBoundingRect(0, pWorld->GetWidth(), 0, pWorld->GetHeight());
		
		GLCommandPanel* pCenterPanel=new GLCommandPanel();
		m_pCommands->AddUIObj(pCenterPanel, GLBorderLayout::CENTER);

		GLCommandPanel* pFlowPanel=new GLCommandPanel(new GLFlowLayout(GLFlowLayout::HORIZONTAL_ALIGNMENT));
		GLBorderLayout* pCenterPanelLayout=new GLBorderLayout();
		pCenterPanelLayout->SetSouthProp(0.1);
		pCenterPanelLayout->StretchHorizontally(true);
		pCenterPanel->SetLayout(pCenterPanelLayout);
		pCenterPanel->AddUIObj(pFlowPanel, GLBorderLayout::SOUTH);

		GLCommand* cmdClear=new GLCommand(glTextures.GetTexture("images\\button_normal_clear.jpg"), glTextures.GetTexture("images\\button_hover_clear.jpg"));
		EventHandler_ClearCurve* pEventHandlerClearCurve=new EventHandler_ClearCurve(cmdClear);
		pEventHandlerClearCurve->SetCurve(&m_curve);
		cmdClear->AddActionListener(pEventHandlerClearCurve);
		pFlowPanel->AddUIObj(cmdClear);
	}
}

void GLState_CatmullRom::Exited(GameWorld *pWorld)
{
	pWorld->RemoveAgents();
}

void GLState_CatmullRom::Update(GameWorld *pWorld, const long &lElapsedTicks)
{
	pWorld->UpdateAgents(lElapsedTicks);
}

void GLState_CatmullRom::Render(GameWorld* pWorld)
{
	GLState::Render(pWorld);

	pWorld->GetCamera().PreTransform();
	pWorld->GetCamera().Transform();

	m_curve.PreRender();
	m_curve.Render();
	m_curve.PostRender();

	pWorld->GetCamera().PostTransform();

	m_pCommands->PreRender();
	m_pCommands->Render();
	m_pCommands->PostRender();
}

void GLState_CatmullRom::MouseButtonDown(GameWorld* pWorld)
{
	GLMouse& game_mouse=pWorld->GetMouse();
	int iButton=game_mouse.GetMouseButton();
	int iX=game_mouse.GetMouseX();
	int iY=game_mouse.GetMouseY();
	int iXRel=game_mouse.GetMouseXRel();
	int iYRel=game_mouse.GetMouseYRel();
	
	m_pCommands->MouseButtonDown(iButton, iX, iY, iXRel, iYRel);

	if(m_pCommands->GetEventReceiver_MouseButtonDown() != NULL)
	{
		return;
	}

	GLSphericalCamera& game_camera=pWorld->GetCamera();
	game_camera.PreTransform();
	game_camera.Transform();
	GLRay mouse_ray=game_mouse.GetRay();
	game_camera.PostTransform();

	GLFloor& floor=pWorld->GetFloor();
	GLVector intersection_point;
	if(mouse_ray.intersect(&floor, intersection_point))
	{
		intersection_point.y=1;
		m_curve.AddPoint(intersection_point);
	}
}

void GLState_CatmullRom::MouseMoved(GameWorld* pWorld)
{
	GLMouse& game_mouse=pWorld->GetMouse();
	int iButton=game_mouse.GetMouseButton();
	int iX=game_mouse.GetMouseX();
	int iY=game_mouse.GetMouseY();
	int iXRel=game_mouse.GetMouseXRel();
	int iYRel=game_mouse.GetMouseYRel();

	m_pCommands->MouseMoved(iButton, iX, iY, iXRel, iYRel);
}