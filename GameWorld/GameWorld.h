#ifndef _H_GAME_WORLD_H
#define _H_GAME_WORLD_H

#include <vector>
#include <set>
#include <map>
#include <string>
#include "../GLExtension/glxs.h"
#include "../GameEngine/GameEngine.h"
#include "../GameEntity/GameEntity.h"
#include "../GLCamera/GLSphericalCamera.h"
#include "../GLLights/GLLightSystem.h"
#include "../GLFloor/GLFloor.h"
#include "../GLSkybox/GLSkybox.h"
#include "../GLInput/GLMouse.h"
#include "../GLInput/GLKeyboard.h"
#include "../GLUI/GLCommandPanel.h"
#include "../GLUI/GLBorderLayout.h"
#include "../GLStates/GLState.h"
#include "../GLSteering/GLPath.h"
#include "../GLShape/GLPlane.h"
#include "../GLTerrain/GLTerrain.h"
#include "../GLShape/GLFrustum.h"
#include "../GameEntity/Vehicle.h"
#include "../GameEntity/Obstacle.h"

typedef std::map<std::string, Vehicle*> MappedAgentGroup;
typedef std::map<std::string, Obstacle*> MappedObstacleGroup;
typedef std::vector<Vehicle*> AgentGroup;
typedef std::vector<Obstacle*> ObstacleGroup;
typedef std::map<std::string, std::pair<GLCommandPanel*, std::map<int, GLCommandPanel*> > > GUIPageGroup;
typedef std::vector<std::string> GUIPageNameGroup;

class GameWorld : public GameEngine
{
public:
	GameWorld();
	virtual ~GameWorld();

protected:
	virtual void Think(const long& lElapsedTicks);
	virtual void Render(SDL_Surface* pScreen);

public:
	void Render3DWorld();

protected:
	virtual void KeyDown(const int& iKeyEnum);
	virtual void KeyUp(const int& iKeyEnum);

protected:
	virtual void MouseMoved(const int& iButton, const int& iX, const int& iY, const int& iXRel, const int& iYRel);
	virtual void MouseButtonDown(const int& iButton, const int& iX, const int& iY, const int& iXRel, const int& iYRel);
	virtual void MouseButtonUp(const int& iButton, const int& iX, const int& iY, const int& iXRel, const int& iYRel);
	
protected:
	virtual void glInit();
	virtual void glLightInit();
	virtual void glCameraInit();
	virtual void glFloorInit();
	virtual void glGUIInit();
	virtual void glFontInit();
	virtual void glTerrainInit();
	virtual void glSkyboxInit();
	virtual void glLuaInit();

protected:
	void CreateConsolePanel_Demo(std::string panel_name);
	void CreateConsolePanel_Command();
public:
	void CreateConsolePanel_Page(std::string panel_name);

protected:
	void UpdateCameraThroughKeyboard(const double& elapsed_time);
	void UpdateCameraThroughMousePosition(const double& elapsed_time);

public:
	std::vector<GLPlane> GetWalls() const;

public:
	void WorldWrap(GameEntity* pAgent);
	void WorldBound(GameEntity* pAgent);

public:
	void ChangeState(GLState* pState);

protected:
	virtual bool SetVideoMode(const int& iWidth, const int& iHeight);
	virtual void AdditionalInit();

protected:
	AgentGroup m_agents;
	MappedAgentGroup m_mapped_agents;
	ObstacleGroup m_obs;
	MappedObstacleGroup m_mapped_obstacles;
	GLSkybox m_skybox;
	GLSphericalCamera m_camera;
	GLLightSystem m_light_system;
	GLMouse m_mouse;
	GLKeyboard m_keyboard;
	bool m_bLoaded;

protected:
	GLCommandPanel m_commands;
	GLCommandPanel* m_pSouthPanel;
public:
	GLCommandPanel* GetSouthPanel() { return m_pSouthPanel; }

protected:
	GUIPageGroup m_gui_page_group;
	std::string m_current_gui_page_name;
	GUIPageNameGroup m_gui_page_names;
public:
	GLCommandPanel* GetEastPanel() { return m_gui_page_group[m_current_gui_page_name].second[GLBorderLayout::EAST]; }
	GLCommandPanel* GetWestPanel() { return m_gui_page_group[m_current_gui_page_name].second[GLBorderLayout::WEST]; }
	GLCommandPanel* GetEastPanel(std::string page_name) { return m_gui_page_group[page_name].second[GLBorderLayout::EAST]; }
	GLCommandPanel* GetWestPanel(std::string page_name) { return m_gui_page_group[page_name].second[GLBorderLayout::WEST]; }
	void SetCurrentGUIPage(std::string page_name) { m_current_gui_page_name=page_name; }	

public:
	GLSphericalCamera& GetCamera() { return m_camera; }
	GLFloor& GetFloor() { return m_floor; }
	GLTerrain& GetTerrain() { return m_terrain; }
	GLMouse& GetMouse() { return m_mouse; }
	GLKeyboard& GetKeyboard() { return m_keyboard; }

public:
	const AgentGroup& GetConstAgents() { return m_agents; }
	AgentGroup& GetMutableAgents() { return m_agents; }
	GUIPageNameGroup& GetGUIPages() { return m_gui_page_names; }
	const ObstacleGroup& GetConstObstacles() const { return m_obs; }
	void UpdateObstacles(const long& lElapsedTicks);
	void UpdateAgents(const long& lElapsedTicks);
	void GetNeighborObstacles(const GameEntity* pAgent, std::set<GameEntity*>& obs, double radius);
	void AddObstacle(Obstacle* pAgent);
	void AddAgent(Vehicle* pAgent);
	void RemoveAgents();
	void RemoveObstacles();
	void RegisterAgent(std::string agent_id, Vehicle* pAgent);
	void RegisterObstacle(std::string obs_id, Obstacle* pObs);
	void MonitorSelectedAgent();
	Vehicle* GetRegisteredAgent(std::string agent_id);
	Obstacle* GetRegisteredObstacle(std::string obs_id);
	std::string GetRegisteredAgentGroupStatus() const;

protected:
	GLState* m_pState;
	GLState* m_pPrevState;
public:
	GLState* GetPrevState() { return m_pPrevState; }
	GLState* GetCurrentState() { return m_pState; }

protected:
	GLPath m_path;
public:
	GLPath& GetPath() { return m_path; }

protected:
	bool m_bPaused;
public:
	void Pause();
	void UnPause();
	bool IsPaused() const;

public:
	double GetEnvHeight(double x, double z) const;
	double GetEnvWidth() const;
	double GetEnvLength() const;

protected:
	double m_scroll_rate;

protected:
	void LoadGameEngineSettings();

protected:
	GLTerrain m_terrain;
	bool m_bShowTerrain;
public:
	void ShowTerrain(bool bShown) { m_bShowTerrain=bShown; }

protected:
	GLFloor m_floor;
	bool m_bShowFloor;
public:
	void ShowFloor(bool bShown) { m_bShowFloor=bShown; }

protected:
	bool m_bShowFPS;
	void ShowFPS();
protected:
	bool m_bShowAuthor;
	void ShowAuthor();

protected:
	bool m_bHighEndSystem;
public:
	bool IsHighEndSystem() const { return m_bHighEndSystem; }

protected:
	GLFrustum m_frustum;
	bool m_bFrustumEnabled;
public:
	GLFrustum& GetFrustum() { return m_frustum; }
};

#endif