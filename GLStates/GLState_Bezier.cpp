#include "GLState_Bezier.h"
#include "../GameWorld/GameWorld.h"
#include "../GLUI/GLCommandPanel.h"
#include "../GLUI/GLBorderLayout.h"
#include "../GLUI/GLFlowLayout.h"
#include "../GLUI/GLCommand.h"
#include "../GLTexture/GLTextureManager.h"
#include "../EventHandler/EventHandler_ClearCurve.h"
#include "../GameEntity/ConstrainedAgent.h"
#include "../GLConsole/GLConsole.h"
#include <iostream>

GLState_Bezier::GLState_Bezier()
: GLState()
, m_pCommands(NULL)
{
	SetTitleTexture("images\\button_normal_demo_bezier.jpg");
	m_state_name="State_Bezier";
}

GLState_Bezier::~GLState_Bezier()
{
	if(m_pCommands != NULL)
	{
		delete m_pCommands;
		m_pCommands=NULL;
	}
}

GLState_Bezier* GLState_Bezier::Instance()
{
	static GLState_Bezier theInstance;
	return &theInstance;
}

void GLState_Bezier::Entered(GameWorld* pWorld)
{
	glConsole << "\nState - Bezier\n";
	glConsole << "click mouse on the plane to generates points for Bezier curve\n";

	SetupUI(pWorld);
	m_curve.Clear();
	pWorld->RemoveAgents();

	ConstrainedAgent* pAgent=new ConstrainedAgent(pWorld);
	pAgent->SetSpeed(10);
	pAgent->SetPath(&m_curve);
	pAgent->SetModel("WalkMech.md2");
	pWorld->AddAgent(pAgent);
}

void GLState_Bezier::SetupUI(GameWorld* pWorld)
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

void GLState_Bezier::Exited(GameWorld *pWorld)
{
	pWorld->RemoveAgents();
}

void GLState_Bezier::Update(GameWorld *pWorld, const long &lElapsedTicks)
{
	pWorld->UpdateAgents(lElapsedTicks);
}

void GLState_Bezier::Render(GameWorld* pWorld)
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

void GLState_Bezier::MouseButtonDown(GameWorld* pWorld)
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

void GLState_Bezier::MouseMoved(GameWorld* pWorld)
{
	GLMouse& game_mouse=pWorld->GetMouse();
	int iButton=game_mouse.GetMouseButton();
	int iX=game_mouse.GetMouseX();
	int iY=game_mouse.GetMouseY();
	int iXRel=game_mouse.GetMouseXRel();
	int iYRel=game_mouse.GetMouseYRel();

	m_pCommands->MouseMoved(iButton, iX, iY, iXRel, iYRel);
}