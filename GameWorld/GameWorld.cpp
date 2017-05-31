#include "GameWorld.h"
#include "../GLMath/GLMath.h"
#include "../GLLights/GLLight_Positional.h"
#include "../GLTexture/GLTextureManager.h"
#include "../GLUI/GLCommand.h"
#include "../GLUI/GLToggleCommand.h"
#include "../GLUI/GLMiniMap.h"
#include "../GLUI/GLBorderLayout.h"
#include "../GLUI/GLFlowLayout.h"
#include "../EventHandler/EventHandler_CameraReset.h"
#include "../EventHandler/EventHandler_ZoomOut.h"
#include "../EventHandler/EventHandler_DemoSeek.h"
#include "../EventHandler/EventHandler_DemoFlee.h"
#include "../EventHandler/EventHandler_DemoWander.h"
#include "../EventHandler/EventHandler_DemoPursuit.h"
#include "../EventHandler/EventHandler_DemoEvade.h"
#include "../EventHandler/EventHandler_ScalePlane.h"
#include "../EventHandler/EventHandler_DemoPathFollow.h"
#include "../EventHandler/EventHandler_DemoInterpose.h"
#include "../EventHandler/EventHandler_DemoFlock.h"
#include "../EventHandler/EventHandler_DemoAvoidance.h"
#include "../EventHandler/EventHandler_DemoAvoidSeek.h"
#include "../EventHandler/EventHandler_DemoBezier.h"
#include "../EventHandler/EventHandler_DemoBSpline.h"
#include "../EventHandler/EventHandler_DemoCatmullRom.h"
#include "../EventHandler/EventHandler_DemoAStar.h"
#include "../EventHandler/EventHandler_DemoPSO.h"
#include "../EventHandler/EventHandler_GameFlow.h"
#include "../EventHandler/EventHandler_GameMenu.h"
#include "../GLStates/GLState_Load.h"
#include "../GLStates/glState_GameMenu.h"
#include <io.h>
#include "../tinyxml/tinyxml.h"
#include "../GLUtil/GLExitSystem.h"
#include "../GLFont/GLFontSys.h"
#include "../GLConsole/GLConsole.h"
#include <sstream>
#include "../GLParticle/GLParticleSystem.h"
#include <algorithm>
#include <cfloat>

GameWorld::GameWorld()
: GameEngine()
, m_scroll_rate(3)
, m_pState(NULL)
, m_pPrevState(NULL)
, m_bShowTerrain(false)
, m_bShowFloor(true)
, m_bShowFPS(true)
, m_bFrustumEnabled(true)
, m_bHighEndSystem(false)
, m_bPaused(false)
, m_pSouthPanel(NULL)
, m_current_gui_page_name("Demo")
, m_bLoaded(false)
{
	
}

GameWorld::~GameWorld()
{
	RemoveAgents();
	RemoveObstacles();

	m_pSouthPanel=NULL;

	GUIPageGroup::iterator pos_panel;
	for(pos_panel=m_gui_page_group.begin(); pos_panel != m_gui_page_group.end(); ++pos_panel)
	{
		delete (pos_panel->second.first);
	}
	m_gui_page_group.clear();
}

bool GameWorld::SetVideoMode(const int& iWidth, const int& iHeight)
{
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);

	// need to call this line if shadow and reflection is to be successful
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 1);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	if(_access("game_engine.xml", 0)==-1)
	{
		SetSurface(SDL_SetVideoMode(iWidth, iHeight, 16, SDL_OPENGL));
	}
	else
	{
		LoadGameEngineSettings();	
	}

	if(GetSurface() == NULL)
	{
		std::cout << "Current Resolution is not supported by this PC" << std::endl;
		std::cout << "Please select a different mode and try again" << std::endl;
		std::cout << "Press any key to exit" << std::endl;
		char temp;
		std::cin >> temp;
		return false;
	}

	return true;
}

double GameWorld::GetEnvHeight(double x, double z) const
{
	if(m_bShowFloor)
	{
		return 0;
	}
	else if(m_bShowTerrain)
	{
		return m_terrain.GetHeight(static_cast<int>(x), static_cast<int>(z));
	}
	else
	{
		return 0;
	}
}

double GameWorld::GetEnvWidth() const
{
	if(m_bShowFloor)
	{
		return m_floor.GetLength();
	}
	else if(m_bShowTerrain)
	{
		return m_terrain.GetWidth();
	}

	return 0;
}

double GameWorld::GetEnvLength() const
{
	if(m_bShowFloor)
	{
		return m_floor.GetLength();
	}
	else if(m_bShowTerrain)
	{
		return m_terrain.GetLength();
	}

	return 0;
}

void GameWorld::LoadGameEngineSettings()
{
	TiXmlDocument doc;
	if(!doc.LoadFile("game_engine.xml"))
	{
		GLExitSystem("GameWorld::SetVideoMode(const int& iWidth, const int& iHeight)", "Failed to load game_engine.xml");
	}

	
	TiXmlElement* root_xml=doc.RootElement();
	std::string mode_name=root_xml->Attribute("mode");
	if(strcmp(root_xml->Attribute("showFPS"), "true")==0)
	{
		m_bShowFPS=true;
	}
	else
	{
		m_bShowFPS=false;
	}
	if(strcmp(root_xml->Attribute("showAuthor"), "true")==0)
	{
		m_bShowAuthor=true;
	}
	else
	{
		m_bShowAuthor=false;
	}
	if(strcmp(root_xml->Attribute("console_print"), "true")==0)
	{
		glConsole.EnableConsole(true);
	}
	else
	{
		glConsole.EnableConsole(false);
	}
	if(strcmp(root_xml->Attribute("high_end_system"), "true")==0)
	{
		m_bHighEndSystem=true;
	}
	else
	{
		m_bHighEndSystem=false;
	}

	bool mode_found=false;
	bool bFullScreen=false;
	int iWidth=800;
	int iHeight=600;
	for(TiXmlElement* mode_xml=root_xml->FirstChildElement(); mode_xml != NULL; mode_xml=mode_xml->NextSiblingElement())
	{
		if(strcmp(mode_xml->Value(), "mode")==0)
		{
			if(mode_name.compare(mode_xml->Attribute("name"))==0)
			{
				if(strcmp(mode_xml->Attribute("fullscreen"), "true")==0)
				{
					bFullScreen=true;
				}
				else
				{
					bFullScreen=false;
				}
				for(TiXmlElement* mode_setting=mode_xml->FirstChildElement(); mode_setting != NULL; mode_setting=mode_setting->NextSiblingElement())
				{
					if(strcmp(mode_setting->Value(), "resolution")==0)
					{
						mode_setting->QueryIntAttribute("width", &iWidth);
						mode_setting->QueryIntAttribute("height", &iHeight);
					}
				}
				
				mode_found=true;
				break;
			}
		}
	}
	if(mode_found)
	{
		if(bFullScreen)
		{
			SetSurface(SDL_SetVideoMode(iWidth, iHeight, 16, SDL_OPENGL | SDL_FULLSCREEN));
		}
		else
		{
			SetSurface(SDL_SetVideoMode(iWidth, iHeight, 16, SDL_OPENGL));
		}
		SetWidth(iWidth);
		SetHeight(iHeight);
	}
	else
	{
		SetSurface(SDL_SetVideoMode(iWidth, iHeight, 16, SDL_OPENGL));
		SetWidth(iWidth);
		SetHeight(iHeight);
	}
}

void GameWorld::RemoveAgents()
{
	AgentGroup::iterator pos_agent;
	for(pos_agent=m_agents.begin(); pos_agent != m_agents.end(); ++pos_agent)
	{
		delete *pos_agent;
	}
	m_agents.clear();
	m_mapped_agents.clear();
}

void GameWorld::AddObstacle(Obstacle* pAgent)
{
	if(pAgent == NULL)
	{
		return;
	}
	ObstacleGroup::iterator pos_obs=std::find(m_obs.begin(), m_obs.end(), pAgent);
	if(pos_obs != m_obs.end())
	{
		return;
	}
	m_obs.push_back(pAgent);
	if(m_bShowFloor)
	{
		m_floor.GetObstacleQuadTree().AddAgent(pAgent);
	}
	if(m_bShowTerrain)
	{
		m_terrain.GetObstacleQuadTree().AddAgent(pAgent);
	}
}

void GameWorld::AddAgent(Vehicle* pAgent)
{
	if(pAgent == NULL)
	{
		return;
	}
	AgentGroup::iterator pos_agent=std::find(m_agents.begin(), m_agents.end(), pAgent);
	if(pos_agent != m_agents.end())
	{
		return;
	}
	m_agents.push_back(pAgent);
}

void GameWorld::UpdateObstacles(const long& lElapsedTicks)
{
	ObstacleGroup::iterator pos_obs;
	for(pos_obs=m_obs.begin(); pos_obs != m_obs.end(); ++pos_obs)
	{
		(*pos_obs)->Update(lElapsedTicks);
	}
}

void GameWorld::UpdateAgents(const long& lElapsedTicks)
{
	AgentGroup::iterator pos_agent;
	for(pos_agent=m_agents.begin(); pos_agent != m_agents.end(); ++pos_agent)
	{
		(*pos_agent)->Update(lElapsedTicks);
		//GLVector agent_position=(*pos_agent)->get_position();
	}
}

void GameWorld::GetNeighborObstacles(const GameEntity* pAgent, std::set<GameEntity*>& obs, double radius)
{
	if(m_bShowFloor)
	{
		m_floor.GetObstacleQuadTree().GetNeighbors(pAgent, obs, radius);
	}
	if(m_bShowTerrain)
	{
		m_terrain.GetObstacleQuadTree().GetNeighbors(pAgent, obs, radius);
	}
}

void GameWorld::RemoveObstacles()
{
	ObstacleGroup::iterator pos_obs;
	for(pos_obs=m_obs.begin(); pos_obs != m_obs.end(); ++pos_obs)
	{
		delete *pos_obs;
	}
	m_obs.clear();
	m_mapped_obstacles.clear();

	if(m_bShowFloor)
	{
		m_floor.ResetQuadTree();
	}
	if(m_bShowTerrain)
	{
		m_terrain.ResetQuadTree();
	}
}

void GameWorld::AdditionalInit()
{
	glxsInit();
	glInit();

	ChangeState(&glState_Load);
}

void GameWorld::glFontInit()
{
	glFont.Init(this);
}

void GameWorld::glTerrainInit()
{
	m_terrain.LoadFromImage("height_map.jpg", this);
}

void GameWorld::glLuaInit()
{
	
}

void GameWorld::CreateConsolePanel_Page(std::string panel_name)
{
	GUIPageGroup::iterator pos_panel=m_gui_page_group.find(panel_name);
	if(pos_panel != m_gui_page_group.end())
	{
		return;
	}

	GLCommandPanel* commands=new GLCommandPanel();
	commands->SetBoundingRect(0, GetWidth(), 0, GetHeight());
	GLBorderLayout* pCommandLayout=new GLBorderLayout();
	pCommandLayout->SetWestProp(0.17); 
	pCommandLayout->SetWestProp(0.17); 
	commands->SetLayout(pCommandLayout);

	GLCommandPanel* pEastPanel=new GLCommandPanel(new GLFlowLayout(GLFlowLayout::VERTICAL_ALIGNMENT));
	pEastPanel->SetInset(GLRect(0, 50, 0, 0));
	commands->AddUIObj(pEastPanel, GLBorderLayout::EAST);

	GLCommandPanel* pWestPanel=new GLCommandPanel(new GLFlowLayout(GLFlowLayout::VERTICAL_ALIGNMENT));
	pWestPanel->SetInset(GLRect(20, 90, 0, 0));
	commands->AddUIObj(pWestPanel, GLBorderLayout::WEST);

	GLMiniMap* mmMap=new GLMiniMap();
	mmMap->SetWidth(120);
	mmMap->SetHeight(120);
	mmMap->SetCamera(&m_camera);
	mmMap->SetWorld(this);
	pWestPanel->AddUIObj(mmMap);

	m_gui_page_group[panel_name].first=commands;
	m_gui_page_group[panel_name].second[GLBorderLayout::EAST]=pEastPanel;
	m_gui_page_group[panel_name].second[GLBorderLayout::WEST]=pWestPanel;

	m_gui_page_names.push_back(panel_name);
}

void GameWorld::CreateConsolePanel_Command()
{
	m_commands.SetBoundingRect(0, GetWidth(), 0, GetHeight());
	GLBorderLayout* pCommandLayout=new GLBorderLayout();
	m_commands.SetLayout(pCommandLayout);
	pCommandLayout->SetSouthProp(0.15); 
	pCommandLayout->SetWestProp(0.17); 

	m_pSouthPanel=new GLCommandPanel();
	m_commands.AddUIObj(m_pSouthPanel, GLBorderLayout::SOUTH);

	GLCommand* cmdResetCamera=new GLCommand(glTextures.GetTexture("images\\button_normal_reset_camera.jpg"), glTextures.GetTexture("images\\button_hover_reset_camera.jpg"));
	EventHandler_CameraReset* pEventHandlerCR=new EventHandler_CameraReset(cmdResetCamera);
	pEventHandlerCR->SetCamera(&m_camera);
	cmdResetCamera->AddActionListener(pEventHandlerCR);
	m_pSouthPanel->AddUIObj(cmdResetCamera);

	GLCommand* cmdEnlargePlane=new GLCommand(glTextures.GetTexture("images\\button_normal_enlarge_plane.jpg"), glTextures.GetTexture("images\\button_hover_enlarge_plane.jpg"));
	EventHandler_ScalePlane* pEventHandlerEnlargePlane=new EventHandler_ScalePlane(cmdEnlargePlane);
	pEventHandlerEnlargePlane->SetScaling(1.1);
	pEventHandlerEnlargePlane->SetPlane(&m_floor);
	cmdEnlargePlane->AddActionListener(pEventHandlerEnlargePlane);
	m_pSouthPanel->AddUIObj(cmdEnlargePlane);

	GLCommand* cmdShrinkPlane=new GLCommand(glTextures.GetTexture("images\\button_normal_shrink_plane.jpg"), glTextures.GetTexture("images\\button_hover_shrink_plane.jpg"));
	EventHandler_ScalePlane* pEventHandlerShrinkPlane=new EventHandler_ScalePlane(cmdShrinkPlane);
	pEventHandlerShrinkPlane->SetScaling(0.9);
	pEventHandlerShrinkPlane->SetPlane(&m_floor);
	cmdShrinkPlane->AddActionListener(pEventHandlerShrinkPlane);
	m_pSouthPanel->AddUIObj(cmdShrinkPlane);

	GLCommand* cmdZoomOut=new GLCommand(glTextures.GetTexture("images\\button_normal_zoom_out.jpg"), glTextures.GetTexture("images\\button_hover_zoom_out.jpg"));
	EventHandler_ZoomOut* pEventHandlerZoomOut=new EventHandler_ZoomOut(cmdZoomOut);
	pEventHandlerZoomOut->SetCamera(&m_camera);
	cmdZoomOut->AddActionListener(pEventHandlerZoomOut);
	m_pSouthPanel->AddUIObj(cmdZoomOut);

	GLToggleCommand* cmdGameFlow=new GLToggleCommand(
		glTextures.GetTexture("images\\button_normal_pause.jpg"), 
		glTextures.GetTexture("images\\button_hover_pause.jpg"),
		glTextures.GetTexture("images\\button_normal_unpause.jpg"), 
		glTextures.GetTexture("images\\button_hover_unpause.jpg"));
	cmdGameFlow->ToggleOn(true);
	EventHandler_GameFlow* pEventHandlerGameFlow=new EventHandler_GameFlow(cmdGameFlow);
	pEventHandlerGameFlow->SetWorld(this);
	cmdGameFlow->AddActionListener(pEventHandlerGameFlow);
	m_pSouthPanel->AddUIObj(cmdGameFlow);


	GLCommand* cmdGameMenu=new GLCommand(glTextures.GetTexture("images\\button_normal_game_menu.jpg"), glTextures.GetTexture("images\\button_hover_game_menu.jpg"));
	EventHandler_GameMenu* pEventHandler_GameMenu=new EventHandler_GameMenu(cmdGameMenu);
	pEventHandler_GameMenu->SetWorld(this);
	cmdGameMenu->AddActionListener(pEventHandler_GameMenu);
	m_pSouthPanel->AddUIObj(cmdGameMenu);
}

void GameWorld::CreateConsolePanel_Demo(std::string panel_name)
{
	GLCommandPanel* commands=new GLCommandPanel();
	commands->SetBoundingRect(0, GetWidth(), 0, GetHeight());
	GLBorderLayout* pCommandLayout=new GLBorderLayout();
	pCommandLayout->SetWestProp(0.17); 
	pCommandLayout->SetWestProp(0.17); 
	commands->SetLayout(pCommandLayout);

	GLCommandPanel* pEastPanel=new GLCommandPanel(new GLFlowLayout(GLFlowLayout::VERTICAL_ALIGNMENT));
	pEastPanel->SetInset(GLRect(0, 50, 0, 0));
	commands->AddUIObj(pEastPanel, GLBorderLayout::EAST);

	GLCommandPanel* pWestPanel=new GLCommandPanel(new GLFlowLayout(GLFlowLayout::VERTICAL_ALIGNMENT));
	pWestPanel->SetInset(GLRect(20, 90, 0, 0));
	commands->AddUIObj(pWestPanel, GLBorderLayout::WEST);

	GLCommand* cmdSeek=new GLCommand(glTextures.GetTexture("images\\button_normal_demo_seek.jpg"), glTextures.GetTexture("images\\button_hover_demo_seek.jpg"));
	cmdSeek->SetWidth(120);
	EventHandler_DemoSeek* pEventHandler_DemoSeek=new EventHandler_DemoSeek(cmdSeek);
	pEventHandler_DemoSeek->SetWorld(this);
	cmdSeek->AddActionListener(pEventHandler_DemoSeek);
	pEastPanel->AddUIObj(cmdSeek);

	GLCommand* cmdFlee=new GLCommand(glTextures.GetTexture("images\\button_normal_demo_flee.jpg"), glTextures.GetTexture("images\\button_hover_demo_flee.jpg"));
	cmdFlee->SetWidth(120);
	EventHandler_DemoFlee* pEventHandler_DemoFlee=new EventHandler_DemoFlee(cmdFlee);
	pEventHandler_DemoFlee->SetWorld(this);
	cmdFlee->AddActionListener(pEventHandler_DemoFlee);
	pEastPanel->AddUIObj(cmdFlee);

	GLCommand* cmdWander=new GLCommand(glTextures.GetTexture("images\\button_normal_demo_wander.jpg"), glTextures.GetTexture("images\\button_hover_demo_wander.jpg"));
	cmdWander->SetWidth(120);
	EventHandler_DemoWander* pEventHandler_DemoWander=new EventHandler_DemoWander(cmdWander);
	pEventHandler_DemoWander->SetWorld(this);
	cmdWander->AddActionListener(pEventHandler_DemoWander);
	pEastPanel->AddUIObj(cmdWander);

	GLCommand* cmdPursuit=new GLCommand(glTextures.GetTexture("images\\button_normal_demo_pursuit.jpg"), glTextures.GetTexture("images\\button_hover_demo_pursuit.jpg"));
	cmdPursuit->SetWidth(120);
	EventHandler_DemoPursuit* pEventHandler_DemoPursuit=new EventHandler_DemoPursuit(cmdPursuit);
	pEventHandler_DemoPursuit->SetWorld(this);
	cmdPursuit->AddActionListener(pEventHandler_DemoPursuit);
	pEastPanel->AddUIObj(cmdPursuit);
	
	GLCommand* cmdEvade=new GLCommand(glTextures.GetTexture("images\\button_normal_demo_evade.jpg"), glTextures.GetTexture("images\\button_hover_demo_evade.jpg"));
	cmdEvade->SetWidth(120);
	EventHandler_DemoEvade* pEventHandler_DemoEvade=new EventHandler_DemoEvade(cmdEvade);
	pEventHandler_DemoEvade->SetWorld(this);
	cmdEvade->AddActionListener(pEventHandler_DemoEvade);
	pEastPanel->AddUIObj(cmdEvade);

	GLCommand* cmdPathFollow=new GLCommand(glTextures.GetTexture("images\\button_normal_demo_path_follow.jpg"), glTextures.GetTexture("images\\button_hover_demo_path_follow.jpg"));
	cmdPathFollow->SetWidth(120);
	EventHandler_DemoPathFollow* pEventHandler_DemoPathFollow=new EventHandler_DemoPathFollow(cmdPathFollow);
	pEventHandler_DemoPathFollow->SetWorld(this);
	cmdPathFollow->AddActionListener(pEventHandler_DemoPathFollow);
	pEastPanel->AddUIObj(cmdPathFollow);

	GLCommand* cmdInterpose=new GLCommand(glTextures.GetTexture("images\\button_normal_demo_interpose.jpg"), glTextures.GetTexture("images\\button_hover_demo_interpose.jpg"));
	cmdInterpose->SetWidth(120);
	EventHandler_DemoInterpose* pEventHandler_DemoInterpose=new EventHandler_DemoInterpose(cmdInterpose);
	pEventHandler_DemoInterpose->SetWorld(this);
	cmdInterpose->AddActionListener(pEventHandler_DemoInterpose);
	pEastPanel->AddUIObj(cmdInterpose);

	GLCommand* cmdFlock=new GLCommand(glTextures.GetTexture("images\\button_normal_demo_flock.jpg"), glTextures.GetTexture("images\\button_hover_demo_flock.jpg"));
	cmdFlock->SetWidth(120);
	EventHandler_DemoFlock* pEventHandler_DemoFlock=new EventHandler_DemoFlock(cmdFlock);
	pEventHandler_DemoFlock->SetWorld(this);
	cmdFlock->AddActionListener(pEventHandler_DemoFlock);
	pEastPanel->AddUIObj(cmdFlock);

	GLCommand* cmdAvoidance=new GLCommand(glTextures.GetTexture("images\\button_normal_demo_avoidance.jpg"), glTextures.GetTexture("images\\button_hover_demo_avoidance.jpg"));
	cmdAvoidance->SetWidth(120);
	EventHandler_DemoAvoidance* pEventHandler_DemoAvoidance=new EventHandler_DemoAvoidance(cmdAvoidance);
	pEventHandler_DemoAvoidance->SetWorld(this);
	cmdAvoidance->AddActionListener(pEventHandler_DemoAvoidance);
	pEastPanel->AddUIObj(cmdAvoidance);

	GLCommand* cmdAvoidSeek=new GLCommand(glTextures.GetTexture("images\\button_normal_demo_avoidseek.jpg"), glTextures.GetTexture("images\\button_hover_demo_avoidseek.jpg"));
	cmdAvoidSeek->SetWidth(120);
	EventHandler_DemoAvoidSeek* pEventHandler_DemoAvoidSeek=new EventHandler_DemoAvoidSeek(cmdAvoidSeek);
	pEventHandler_DemoAvoidSeek->SetWorld(this);
	cmdAvoidSeek->AddActionListener(pEventHandler_DemoAvoidSeek);
	pEastPanel->AddUIObj(cmdAvoidSeek);

	GLCommand* cmdBezier=new GLCommand(glTextures.GetTexture("images\\button_normal_demo_bezier.jpg"), glTextures.GetTexture("images\\button_hover_demo_bezier.jpg"));
	cmdBezier->SetWidth(120);
	EventHandler_DemoBezier* pEventHandler_DemoBezier=new EventHandler_DemoBezier(cmdBezier);
	pEventHandler_DemoBezier->SetWorld(this);
	cmdBezier->AddActionListener(pEventHandler_DemoBezier);
	pWestPanel->AddUIObj(cmdBezier);

	GLCommand* cmdBSpline=new GLCommand(glTextures.GetTexture("images\\button_normal_demo_bspline.jpg"), glTextures.GetTexture("images\\button_hover_demo_bspline.jpg"));
	cmdBSpline->SetWidth(120);
	EventHandler_DemoBSpline* pEventHandler_DemoBSpline=new EventHandler_DemoBSpline(cmdBSpline);
	pEventHandler_DemoBSpline->SetWorld(this);
	cmdBSpline->AddActionListener(pEventHandler_DemoBSpline);
	pWestPanel->AddUIObj(cmdBSpline);

	GLCommand* cmdCatmullRom=new GLCommand(glTextures.GetTexture("images\\button_normal_demo_catmull_rom.jpg"), glTextures.GetTexture("images\\button_hover_demo_catmull_rom.jpg"));
	cmdCatmullRom->SetWidth(120);
	EventHandler_DemoCatmullRom* pEventHandler_DemoCatmullRom=new EventHandler_DemoCatmullRom(cmdCatmullRom);
	pEventHandler_DemoCatmullRom->SetWorld(this);
	cmdCatmullRom->AddActionListener(pEventHandler_DemoCatmullRom);
	pWestPanel->AddUIObj(cmdCatmullRom);

	GLCommand* cmdAStar=new GLCommand(glTextures.GetTexture("images\\button_normal_demo_astar.jpg"), glTextures.GetTexture("images\\button_hover_demo_astar.jpg"));
	cmdAStar->SetWidth(120);
	EventHandler_DemoAStar* pEventHandler_DemoAStar=new EventHandler_DemoAStar(cmdAStar);
	pEventHandler_DemoAStar->SetWorld(this);
	cmdAStar->AddActionListener(pEventHandler_DemoAStar);
	pWestPanel->AddUIObj(cmdAStar);

	GLCommand* cmdPSO=new GLCommand(glTextures.GetTexture("images\\button_normal_demo_pso.jpg"), glTextures.GetTexture("images\\button_hover_demo_pso.jpg"));
	cmdPSO->SetWidth(120);
	EventHandler_DemoPSO* pEventHandler_DemoPSO=new EventHandler_DemoPSO(cmdPSO);
	pEventHandler_DemoPSO->SetWorld(this);
	cmdPSO->AddActionListener(pEventHandler_DemoPSO);
	pWestPanel->AddUIObj(cmdPSO);

	GLMiniMap* mmMap=new GLMiniMap();
	mmMap->SetWidth(120);
	mmMap->SetHeight(120);
	mmMap->SetCamera(&m_camera);
	mmMap->SetWorld(this);
	pWestPanel->AddUIObj(mmMap);

	m_gui_page_group[panel_name].first=commands;
	m_gui_page_group[panel_name].second[GLBorderLayout::EAST]=pEastPanel;
	m_gui_page_group[panel_name].second[GLBorderLayout::WEST]=pWestPanel;

	m_gui_page_names.push_back(panel_name);
}

void GameWorld::glGUIInit()
{
	CreateConsolePanel_Command();
	CreateConsolePanel_Demo("Demo");

	this->GetEastPanel()->SetVisible(false);
	this->GetWestPanel()->SetVisible(false);
	this->GetSouthPanel()->SetVisible(false);
}

void GameWorld::glSkyboxInit()
{
	m_skybox.LoadScript("GLSkybox.xml");
}

void GameWorld::UnPause()
{
	m_bPaused=false;
}

void GameWorld::Pause()
{
	m_bPaused=true;
}

bool GameWorld::IsPaused() const 
{
	return m_bPaused;
}

void GameWorld::glInit()
{
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_FLAT);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);
	glEnable(GL_CULL_FACE);


	int width=this->GetWidth();
	int height=this->GetHeight();
	double aspect=static_cast<double>(width) / static_cast<double>(height);

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, aspect, 1.0, 1000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void GameWorld::glLightInit()
{
	GLLight_Positional* light0=new GLLight_Positional();
	m_light_system.SetLight(GL_LIGHT0, light0);
}

void GameWorld::glCameraInit()
{
	m_camera.SetMinTheta(glMath.PI() / 2 * 0.1);
	m_camera.SetCameraCenterConstraints(GLVector(-50, 0, -50), GLVector(50, 0, 50));
}

void GameWorld::glFloorInit()
{
	m_floor.Init("plane.jpg", this);
}

void GameWorld::Think(const long& lElapsedTicks)
{
	static unsigned short int iLoaded=0;
	if(!m_bLoaded)
	{
		switch(iLoaded)
		{
		case 0:
			glFontInit();
			break;
		case 1:
			glCameraInit();
			break;
		case 2:
			glLightInit();
			break;
		case 3:
			glSkyboxInit();
			break;
		case 4:
			glFloorInit();
			break;
		case 5:
			glTerrainInit();
			break;
		case 6:
			glLuaInit();
			break;
		case 7:
			glGUIInit();
			break;
		case 8:
			ChangeState(&glState_GameMenu);
			m_bLoaded=true;
			break;
		}

		m_pState->Update(this, lElapsedTicks);
		
		++iLoaded;
		return;
	}

	double elapsed_time=static_cast<double>(lElapsedTicks) / 1000.0;
	
	if(!m_camera.IsLocked())
	{
		UpdateCameraThroughKeyboard(elapsed_time);
		UpdateCameraThroughMousePosition(elapsed_time);
	}

	MappedAgentGroup::iterator pos_mapped_agent=m_mapped_agents.begin();
	while(pos_mapped_agent != m_mapped_agents.end())
	{
		if((pos_mapped_agent->second)->is_marked_for_destruction())
		{
			pos_mapped_agent=m_mapped_agents.erase(pos_mapped_agent);
		}
		else
		{
			++pos_mapped_agent;
		}
	}

	MappedObstacleGroup::iterator pos_mapped_obstacle=m_mapped_obstacles.begin();
	while(pos_mapped_obstacle != m_mapped_obstacles.end())
	{
		if((pos_mapped_obstacle->second)->is_marked_for_destruction())
		{
			pos_mapped_obstacle=m_mapped_obstacles.erase(pos_mapped_obstacle);
		}
		else
		{
			++pos_mapped_obstacle;
		}
	}

	AgentGroup::iterator pos_agent=m_agents.begin();
	while(pos_agent != m_agents.end())
	{
		if((*pos_agent)->is_marked_for_destruction())
		{
			delete (*pos_agent);
			(*pos_agent)=NULL;
			pos_agent=m_agents.erase(pos_agent);
		}
		else
		{
			++pos_agent;
		}
	}

	ObstacleGroup::iterator pos_obs=m_obs.begin();
	bool no_obs_deletion=true;
	while(pos_obs != m_obs.end())
	{
		if((*pos_obs)->is_marked_for_destruction())
		{
			delete (*pos_obs);
			(*pos_obs)=NULL;
			pos_obs=m_obs.erase(pos_obs);
			no_obs_deletion=false;
		}
		else
		{
			++pos_obs;
		}
	}

	if(no_obs_deletion==false)
	{
		if(m_bShowFloor)
		{
			m_floor.ResetQuadTree();
			for(pos_obs=m_obs.begin(); pos_obs != m_obs.end(); ++pos_obs)
			{
				m_floor.GetObstacleQuadTree().AddAgent(*pos_obs);
			}
		}
		if(m_bShowTerrain)
		{
			m_terrain.ResetQuadTree();
			for(pos_obs=m_obs.begin(); pos_obs != m_obs.end(); ++pos_obs)
			{
				m_terrain.GetObstacleQuadTree().AddAgent(*pos_obs);
			}
		}
	}

	if(m_bPaused==false)
	{
		glParticleSys.Update(lElapsedTicks);
		m_pState->Update(this, lElapsedTicks);
	}
}

void GameWorld::UpdateCameraThroughKeyboard(const double& elapsed_time)
{
	if(m_keyboard.IsKeyDown(SDLK_LEFT))
	{
		m_camera.IncrementBeta(elapsed_time);
	}
	if(m_keyboard.IsKeyDown(SDLK_RIGHT))
	{
		m_camera.IncrementBeta(-elapsed_time);
	}
	if(m_keyboard.IsKeyDown(SDLK_UP))
	{
		m_camera.IncrementTheta(elapsed_time * 2);
	}
	if(m_keyboard.IsKeyDown(SDLK_DOWN))
	{
		m_camera.IncrementTheta(-elapsed_time * 2);
	}
	if(m_keyboard.IsKeyDown(SDLK_a))
	{
		m_camera.IncrementRadius(elapsed_time * 8);
	}
	if(m_keyboard.IsKeyDown(SDLK_s))
	{
		m_camera.IncrementRadius(-elapsed_time * 8);
	}

}

void GameWorld::ChangeState(GLState* pState)
{
	if(m_pState != NULL)
	{
		m_pState->Exited(this);
	}

	m_pPrevState=m_pState;

	m_pState=pState;

	m_pState->Entered(this);
}

void GameWorld::UpdateCameraThroughMousePosition(const double& elapsed_time)
{
	if(m_commands.GetEventReceiver_MouseMoved()!=NULL)
	{
		return;
	}

	const int MARGIN=20;

	//Xianshun says:
	// Calls such as glGetFloatv, glGetIntegerv, glIsEnabled, glGetError, glGetString require a slow, round trip transaction between the application and renderer. 
	// Especially avoid them in your main rendering code
	// 
	/*
	GLint matViewport[4];
	glGetIntegerv(GL_VIEWPORT, matViewport);

	int window_width=matViewport[2];
	int window_height=matViewport[3];
	*/
	int window_width=GetWidth();
	int window_height=GetHeight();

	int window_x=m_mouse.GetMouseX();
	int window_y=m_mouse.GetMouseY();

	if(window_x < MARGIN)
	{
		GLVector vRight=m_camera.GetRight();
		vRight *= (-1);
		m_camera.Scroll( vRight * elapsed_time * m_scroll_rate);
	}
	else if(window_x > window_width - MARGIN)
	{
		GLVector vRight=m_camera.GetRight();
		m_camera.Scroll( vRight * elapsed_time * m_scroll_rate);
	}
	if(window_y < MARGIN)
	{
		GLVector vLook=m_camera.GetLook();

		m_camera.Scroll(vLook * elapsed_time * m_scroll_rate);
	}
	else if(window_y > window_height- MARGIN)
	{
		GLVector vLook=m_camera.GetLook();
		vLook *= (-1);

		m_camera.Scroll(vLook * elapsed_time * m_scroll_rate);
	}
}

void GameWorld::WorldBound(GameEntity* pAgent)
{
	GLVector pos=pAgent->get_position();
	double x=pos.x;
	double z=pos.z;

	if(m_bShowFloor)
	{
		double full_length=m_floor.GetLength();
		double half_length=full_length / 2.0;
		if(x < -half_length)
		{
			double dx=-half_length - x;
			x+=dx * 2;
		}
		else if(x > half_length)
		{
			double dx=half_length - x;
			x+=dx * 2;
		}
		if(z < -half_length)
		{
			double dz=-half_length - z;
			z+=dz * 2;
		}
		else if(z > half_length)
		{
			double dz=half_length - z;
			z+=dz * 2;
		}
	}
	else if(m_bShowTerrain)
	{
		double full_length=m_terrain.GetLength();
		double full_width=m_terrain.GetWidth();
		double half_length=full_length / 2.0;
		double half_width=full_width / 2.0;

		if(x < -half_width)
		{
			double dx=-half_width - x;
			x+=dx * 2;
		}
		else if(x > half_width)
		{
			double dx=half_width - x;
			x+=dx * 2;
		}
		if(z < -half_length)
		{
			double dz=-half_length - z;
			z+=dz * 2;
		}
		else if(z > half_length)
		{
			double dz=half_length - z;
			z+=dz * 2;
		}
	}

	pos.x=x;
	pos.z=z;
	//pos.y=this->GetEnvHeight(x, z);
	pAgent->set_position(pos);
}

void GameWorld::WorldWrap(GameEntity* pAgent)
{
	GLVector pos=pAgent->get_position();
	double x=pos.x;
	double z=pos.z;

	if(m_bShowFloor)
	{
		double full_length=m_floor.GetLength();
		double half_length=full_length / 2.0;
		if(x < -half_length)
		{
			x+=full_length;
		}
		else if(x > half_length)
		{
			x-=full_length;
		}
		if(z < -half_length)
		{
			z+=full_length;
		}
		else if(z > half_length)
		{
			z-=full_length;
		}
	}
	else if(m_bShowTerrain)
	{
		double full_length=m_terrain.GetLength();
		double full_width=m_terrain.GetWidth();
		double half_length=full_length / 2.0;
		double half_width=full_width / 2.0;

		if(x < -half_width)
		{
			x+=full_width;
		}
		else if(x > half_width)
		{
			x-=full_width;
		}
		if(z < -half_length)
		{
			z+=full_length;
		}
		else if(z > half_length)
		{
			z-=full_length;
		}
	}

	pos.x=x;
	pos.z=z;
	//pos.y=this->GetEnvHeight(x, z);
	pAgent->set_position(pos);
}

std::vector<GLPlane> GameWorld::GetWalls() const
{
	std::vector<GLPlane> walls;
	double side_length=m_floor.GetLength() * 0.4;
	walls.push_back(GLPlane(GLVector(1, 0, 0), -side_length));
	walls.push_back(GLPlane(GLVector(-1, 0, 0), -side_length));
	walls.push_back(GLPlane(GLVector(0, 0, 1), -side_length));
	walls.push_back(GLPlane(GLVector(0, 0, -1), -side_length));

	return walls;
}

void GameWorld::Render3DWorld()
{
	m_camera.PreTransform();
	m_camera.Transform();

	if(m_bFrustumEnabled)
	{
		m_frustum.MakeCameraFrustum();
	}

	m_skybox.PreRender();
	m_skybox.Render();
	m_skybox.PostRender();

	m_light_system.PreRender();
	m_light_system.Render();

	AgentGroup::iterator pos_agent;
	ObstacleGroup::iterator pos_obs;

	if(m_bShowFloor)
	{
		m_floor.clear_reflected_objects();
		m_floor.add_reflected_object(&m_skybox);

		if(!m_path.IsNull())
		{
			m_floor.add_reflected_object(&m_path);
		}

		for(pos_agent=m_agents.begin(); pos_agent != m_agents.end(); ++pos_agent)
		{
			if(m_bFrustumEnabled==false || m_frustum.CubeInFrustum((*pos_agent)->GetBoundingCube()))
			{
				m_floor.add_reflected_object(*pos_agent);
			}
		}
		for(pos_obs=m_obs.begin(); pos_obs != m_obs.end(); ++pos_obs)
		{
			if(m_bFrustumEnabled==false || m_frustum.CubeInFrustum((*pos_obs)->GetBoundingCube()))
			{
				m_floor.add_reflected_object(*pos_obs);
			}
		}

		m_floor.PreRender();
		m_floor.Render();
		m_floor.PostRender();
	}

	if(m_bShowTerrain)
	{
		m_terrain.PreRender();
		m_terrain.Render();
		m_terrain.PostRender();
	}

	if(!m_path.IsNull())
	{
		m_path.PreRender();
		m_path.Render();
		m_path.PostRender();
	}

	for(pos_agent=m_agents.begin(); pos_agent != m_agents.end(); ++pos_agent)
	{
		if(m_bFrustumEnabled==false || m_frustum.CubeInFrustum((*pos_agent)->GetBoundingCube()))
		{
			(*pos_agent)->PreRender();
			(*pos_agent)->Render();
			(*pos_agent)->PostRender();
		}
	}

	for(pos_obs=m_obs.begin(); pos_obs != m_obs.end(); ++pos_obs)
	{
		if(m_bFrustumEnabled==false || m_frustum.CubeInFrustum((*pos_obs)->GetBoundingCube()))
		{
			(*pos_obs)->PreRender();
			(*pos_obs)->Render();
			(*pos_obs)->PostRender();
		}
	}

	glParticleSys.Render();

	m_light_system.PostRender();

	m_camera.PostTransform();

	m_commands.PreRender();
	m_commands.Render();
	m_commands.PostRender();

	m_gui_page_group[m_current_gui_page_name].first->PreRender();
	m_gui_page_group[m_current_gui_page_name].first->Render();
	m_gui_page_group[m_current_gui_page_name].first->PostRender();

	if(m_bShowFPS)
	{
		ShowFPS();
	}
	if(m_bShowAuthor)
	{
		ShowAuthor();
	}
	if(m_bPaused)
	{
		std::ostringstream oss;
		oss << "The System is now paused... ";

		glFont.println(oss.str(), GetWidth() / 2.0, GetHeight() / 2.0 -  20);
	}
}

void GameWorld::Render(SDL_Surface* pScreen)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	m_pState->Render(this);

	SDL_GL_SwapBuffers();
}

void GameWorld::ShowFPS()
{
	std::ostringstream oss;
	oss << "FPS: " << GetFPS();

	glFont.println(oss.str(), 10, GetHeight() -  20);
}

void GameWorld::ShowAuthor()
{
	glFont.println("This work is developed by Chen Xianshun", 10, 10);
}

void GameWorld::KeyDown(const int &iKeyEnum)
{
	m_keyboard.KeyDown(iKeyEnum);

	m_pState->KeyDown(this);
}

void GameWorld::KeyUp(const int &iKeyEnum)
{
	m_keyboard.KeyUp(iKeyEnum);

	m_pState->KeyUp(this);
}

void GameWorld::MouseMoved(const int& iButton, const int& iX, const int& iY, const int& iXRel, const int& iYRel)
{
	m_mouse.MouseMoved(iButton, iX, iY, iXRel, iYRel);
	m_commands.MouseMoved(iButton, iX, iY, iXRel, iYRel);

	if(!m_gui_page_group.empty())
	{
		m_gui_page_group[m_current_gui_page_name].first->MouseMoved(iButton, iX, iY, iXRel, iYRel);
	}

	m_pState->MouseMoved(this);
}

void GameWorld::MouseButtonDown(const int& iButton, const int& iX, const int& iY, const int& iXRel, const int& iYRel)
{
	m_mouse.MouseButtonDown(iButton, iX, iY, iXRel, iYRel);
	m_commands.MouseButtonDown(iButton, iX, iY, iXRel, iYRel);
	m_gui_page_group[m_current_gui_page_name].first->MouseButtonDown(iButton, iX, iY, iXRel, iYRel);

	if(m_commands.GetEventReceiver_MouseButtonDown() != NULL)
	{
		return;
	}

	if(m_gui_page_group[m_current_gui_page_name].first->GetEventReceiver_MouseButtonDown() != NULL)
	{
		return;
	}

	m_pState->MouseButtonDown(this);
}

void GameWorld::MouseButtonUp(const int &iButton, const int &iX, const int &iY, const int &iXRel, const int &iYRel)
{
	m_mouse.MouseButtonUp(iButton, iX, iY, iXRel, iYRel);
	m_commands.MouseButtonUp(iButton, iX, iY, iXRel, iYRel);	
	m_gui_page_group[m_current_gui_page_name].first->MouseButtonUp(iButton, iX, iY, iXRel, iYRel);	

	m_pState->MouseButtonUp(this);
}

void GameWorld::MonitorSelectedAgent()
{
	Vehicle* pAgent=NULL;
	Vehicle* pMoniteredAgent=NULL;
	Vehicle* pPrevMonitoredAgent=NULL;
	double closest_distance=DBL_MAX, distance;

	AgentGroup::iterator pos_agent;
	for(pos_agent=m_agents.begin(); pos_agent != m_agents.end(); ++pos_agent)
	{
		pAgent=(*pos_agent);
		if(!(pAgent->is_marked_for_destruction()))
		{
			if(pAgent->IsCameraFocus())
			{
				pPrevMonitoredAgent=pAgent;
				pAgent->SetCameraFocus(false);
			}
			if(pAgent->IntersectWithMouse(distance))
			{
				if(distance < closest_distance)
				{
					closest_distance=distance;
					pMoniteredAgent=pAgent;
				}
			}
		}
	}
	if(pMoniteredAgent != NULL)
	{
		pMoniteredAgent->SetCameraFocus(true);
	}
}

void GameWorld::RegisterAgent(std::string agent_id, Vehicle* pAgent)
{
	MappedAgentGroup::iterator pos_agent=m_mapped_agents.find(agent_id);
	if(pos_agent != m_mapped_agents.end())
	{
		AgentGroup::iterator pos=std::find(m_agents.begin(), m_agents.end(), pos_agent->second);
		if(pos != m_agents.end())
		{
			delete (*pos);
			m_agents.erase(pos);
		}
	}
	m_mapped_agents[agent_id]=pAgent;
	m_agents.push_back(pAgent);
}

void GameWorld::RegisterObstacle(std::string obstacle_id, Obstacle* pObs)
{
	MappedObstacleGroup::iterator pos_obstacle=m_mapped_obstacles.find(obstacle_id);
	if(pos_obstacle != m_mapped_obstacles.end())
	{
		ObstacleGroup::iterator pos=std::find(m_obs.begin(), m_obs.end(), pos_obstacle->second);
		if(pos != m_obs.end())
		{
			delete (*pos);
			m_obs.erase(pos);
		}
	}
	m_mapped_obstacles[obstacle_id]=pObs;
	m_obs.push_back(pObs);
}

Vehicle* GameWorld::GetRegisteredAgent(std::string agent_id)
{
	std::map<std::string, Vehicle*>::iterator pos_mapped_agent=m_mapped_agents.find(agent_id);
	if(pos_mapped_agent != m_mapped_agents.end())
	{
		return pos_mapped_agent->second;
	}

	return NULL;
}

Obstacle* GameWorld::GetRegisteredObstacle(std::string obs_id)
{
	std::map<std::string, Obstacle*>::iterator pos_mapped_obstacle=m_mapped_obstacles.find(obs_id);
	if(pos_mapped_obstacle != m_mapped_obstacles.end())
	{
		return pos_mapped_obstacle->second;
	}

	return NULL;
}

std::string GameWorld::GetRegisteredAgentGroupStatus() const
{
	std::ostringstream oss;
	oss << "<?xml version=\"1.0\"?>\n";
	oss << "<root>\n";

	MappedAgentGroup::const_iterator pos_mapped_agent;
	for(pos_mapped_agent=m_mapped_agents.begin(); pos_mapped_agent != m_mapped_agents.end(); ++pos_mapped_agent)
	{
		std::string agent_id=pos_mapped_agent->first;
		Vehicle* pAgent=pos_mapped_agent->second;
		const GLVector& position=pAgent->get_position();
		oss << "<agent_status agent_id=\"" << agent_id << "\" p_x=\"" << position.x << "\" p_y=\"" << position.y << "\" p_z=\"" << position.z 
			<< "\" />\n";
	}
	MappedObstacleGroup::const_iterator pos_mapped_obstacle;
	for(pos_mapped_obstacle=m_mapped_obstacles.begin(); pos_mapped_obstacle != m_mapped_obstacles.end(); ++pos_mapped_obstacle)
	{
		std::string obstacle_id=pos_mapped_obstacle->first;
		Obstacle* pObs=pos_mapped_obstacle->second;
		const GLVector& position=pObs->get_position();
		oss << "<agent_status agent_id=\"" << obstacle_id << "\" p_x=\"" << position.x << "\" p_y=\"" << position.y << "\" p_z=\"" << position.z 
			<< "\" />\n";
	}
	oss << "<current_state name=\"" << m_pState->GetStateName() << "\" />\n";

	oss << "</root>\n";
	return oss.str();
}