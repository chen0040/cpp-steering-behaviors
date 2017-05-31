#include "GLState_GameMenu.h"
#include "../GameWorld/GameWorld.h"
#include "../GLConsole/GLConsole.h"
#include <cfloat>
#include "../GLTexture/GLTextureManager.h"
#include "../GLUI/GLCommandPanel.h"
#include "../GLUI/GLBorderLayout.h"
#include "../GLUI/GLFlowLayout.h"
#include "../GLUI/GLCommand.h"

GLState_GameMenu::GLState_GameMenu()
: GLState()
, m_pGUI(NULL)
{
	m_state_name="State_GameMenu";
}

GLState_GameMenu::~GLState_GameMenu()
{
	if(m_pGUI != NULL)
	{
		delete m_pGUI;
		m_pGUI=NULL;
	}
}

void GLState_GameMenu::Entered(GameWorld* pWorld)
{
	pWorld->GetEastPanel()->SetVisible(false);
	pWorld->GetWestPanel()->SetVisible(false);
	pWorld->GetSouthPanel()->SetVisible(false);

	pWorld->RemoveAgents();
	pWorld->RemoveObstacles();

	pWorld->GetCamera().Reset();
	pWorld->GetCamera().Lock();

	if(m_pGUI == NULL)
	{
		GLBorderLayout* pCommandLayout=new GLBorderLayout();
		pCommandLayout->SetSouthProp(0.15);
		pCommandLayout->SetWestProp(0.25);
		m_pGUI=new GLCommandPanel(pCommandLayout);
		m_pGUI->SetBoundingRect(0, pWorld->GetWidth(), 0, pWorld->GetHeight());

		GLCommandPanel* pFlowPanel=new GLCommandPanel(new GLFlowLayout(GLFlowLayout::VERTICAL_ALIGNMENT));
		m_pGUI->AddUIObj(pFlowPanel, GLBorderLayout::CENTER);

		
	}
}

void GLState_GameMenu::Render(GameWorld* pWorld)
{
	GLState::Render(pWorld);

	m_pGUI->PreRender();
	m_pGUI->Render();
	m_pGUI->PostRender();
}

void GLState_GameMenu::Exited(GameWorld* pWorld)
{
	pWorld->GetEastPanel()->SetVisible(true);
	pWorld->GetWestPanel()->SetVisible(true);
	pWorld->GetSouthPanel()->SetVisible(true);

	pWorld->GetCamera().Unlock();
}

void GLState_GameMenu::Update(GameWorld* pWorld, const long& lElapsedTicks)
{
	
}

GLState_GameMenu* GLState_GameMenu::Instance()
{
	static GLState_GameMenu theInstance;
	return &theInstance;
}

void GLState_GameMenu::MouseButtonDown(GameWorld* pWorld)
{
	GLMouse& game_mouse=pWorld->GetMouse();
	int iButton=game_mouse.GetMouseButton();
	int iX=game_mouse.GetMouseX();
	int iY=game_mouse.GetMouseY();
	int iXRel=game_mouse.GetMouseXRel();
	int iYRel=game_mouse.GetMouseYRel();
	
	m_pGUI->MouseButtonDown(iButton, iX, iY, iXRel, iYRel);

	if(m_pGUI->GetEventReceiver_MouseButtonDown() != NULL)
	{
		return;
	}
}

void GLState_GameMenu::MouseMoved(GameWorld* pWorld)
{
	GLMouse& game_mouse=pWorld->GetMouse();
	int iButton=game_mouse.GetMouseButton();
	int iX=game_mouse.GetMouseX();
	int iY=game_mouse.GetMouseY();
	int iXRel=game_mouse.GetMouseXRel();
	int iYRel=game_mouse.GetMouseYRel();

	m_pGUI->MouseMoved(iButton, iX, iY, iXRel, iYRel);
}