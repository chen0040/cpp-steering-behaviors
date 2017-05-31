#include "GLState_AStar.h"
#include "../GameWorld/GameWorld.h"
#include "../GameEntity/GLPlant.h"
#include "../GLSteering/GLSteering_Wander.h"
#include "../GLSteering/GLSteering_Separation.h"
#include "../GLSteering/GLSteering_Alignment.h"
#include "../GLSteering/GLSteering_Cohesion.h"
#include "../GLSteering/GLSteering_Pursuit.h"
#include "../GLSteering/GLSteering_Evade.h"
#include "../GLSteering/GLSteering_Avoidance.h"
#include "../GLMath/GLMath.h"
#include "../GLConsole/GLConsole.h"
#include "../GLRole/GLRole_Predator.h"
#include "../GLRole/GLRole_Prey.h"
#include <cfloat>
#include <set>
#include "../tinyxml/tinyxml.h"
#include "../GLUtil/GLExitSystem.h"
#include "../GLLocomotion/gLLocomotion_Aircraft.h"

GLState_AStar::GLState_AStar()
: GLState()
, m_iLandCraftPreyCount(0)
, m_iAirCraftPreyCount(0)
, m_iTreeCount(0)
, m_bScriptLoaded(false)
{
	this->SetTitleTexture("images\\button_normal_demo_astar.jpg");
	m_state_name="State_AStar";
}

GLState_AStar::~GLState_AStar()
{

}

void GLState_AStar::LoadScript(GameWorld* pWorld)
{
bool bHighEndSystem=pWorld->IsHighEndSystem();

	TiXmlDocument doc;
	if(!doc.LoadFile("GLScripts\\glState_Terrain.xml"))
	{
		GLExitSystem("GLState_AStar::GLState_AStar()", "Failed to load GLState_Terrain.xml");
	}
	TiXmlElement* xml_root=doc.RootElement();
	for(TiXmlElement* xml_level1=xml_root->FirstChildElement(); xml_level1 != NULL; xml_level1=xml_level1->NextSiblingElement())
	{
		if(strcmp(xml_level1->Value(), "parameters")==0)
		{
			for(TiXmlElement* xml_level2=xml_level1->FirstChildElement(); xml_level2 != NULL; xml_level2=xml_level2->NextSiblingElement())
			{
				if(strcmp(xml_level2->Value(), "param")==0)
				{
					if(strcmp(xml_level2->Attribute("name"), "iTreeCount")==0)
					{
						int value;
						if(bHighEndSystem)
						{
							xml_level2->QueryIntAttribute("high_end_value", &value);
						}
						else
						{
							xml_level2->QueryIntAttribute("low_end_value", &value);
						}
						m_iTreeCount=value;
					}
					else if(strcmp(xml_level2->Attribute("name"), "iLandCraftPreyCount")==0)
					{
						int value;
						if(bHighEndSystem)
						{
							xml_level2->QueryIntAttribute("high_end_value", &value);
						}
						else
						{
							xml_level2->QueryIntAttribute("low_end_value", &value);
						}
						m_iLandCraftPreyCount=value;
					}
					else if(strcmp(xml_level2->Attribute("name"), "iAirCraftPreyCount")==0)
					{
						int value;
						if(bHighEndSystem)
						{
							xml_level2->QueryIntAttribute("high_end_value", &value);
						}
						else
						{
							xml_level2->QueryIntAttribute("low_end_value", &value);
						}
						m_iAirCraftPreyCount=value;
					}
					else if(strcmp(xml_level2->Attribute("name"), "AirCraftPrey_Model")==0)
					{
						m_AirCraftPreyModel=bHighEndSystem ? xml_level2->Attribute("high_end_value") : xml_level2->Attribute("low_end_value");
					}
				}
			}
		}
	}
}

void GLState_AStar::Update(GameWorld* pWorld, const long& lElapsedTicks)
{
	pWorld->UpdateAgents(lElapsedTicks);
	pWorld->UpdateObstacles(lElapsedTicks);
}

Vehicle* GLState_AStar::CreateAirCraftPredator(GameWorld* pWorld)
{
	Vehicle* pAgent=new Vehicle(pWorld);
	pAgent->SetLocomotion(new GLLocomotion_Aircraft(pAgent));
	pAgent->SetZoneId(GameEntity::ZONE_AIR);
	pAgent->SetHoverHeight(20);
	//pAgent->SetMaxForce(9);
	pAgent->SetMass(1);
	pAgent->SetMaxSpeed(12);
	pAgent->SetModel("fighter3.3ds");
	pAgent->GetAnimator().SetMaxFPS(64);
	pAgent->EnableSound(false);

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

Vehicle* GLState_AStar::CreateLandCraftPredator(GameWorld* pWorld)
{
	Vehicle* pAgent=new Vehicle(pWorld);
	//pAgent->SetMaxForce(8);
	pAgent->SetMass(1);
	pAgent->SetMaxSpeed(10);
	pAgent->SetModel("WalkMech.md2");
	pAgent->GetAnimator().SetMaxFPS(64);
	pAgent->EnableSound(false);
	pAgent->SetColor(1, 0, 0);

	GLSteering_Avoidance* avoidance=new GLSteering_Avoidance(pAgent);
	//avoidance->SetMinDetectionBoxLength(10);
	avoidance->SetWeight(20);
	avoidance->TurnOn();
	pAgent->AddSteering("avoidance", avoidance);

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

void GLState_AStar::Entered(GameWorld *pWorld)
{
	if(!m_bScriptLoaded)
	{
		LoadScript(pWorld);
		m_bScriptLoaded=true;
	}
	glConsole << "\nState - Terrain\n";

	pWorld->ShowFloor(false);
	pWorld->ShowTerrain(true);
	if(!pWorld->GetTerrain().IsLoadedFromImage() || pWorld->GetTerrain().GetLoadedImageName().compare("height_map.jpg") !=0)
	{
		pWorld->GetTerrain().LoadFromImage("height_map.jpg", pWorld);
	}
	pWorld->RemoveAgents();
	pWorld->RemoveObstacles();

	Vehicle* air_predator=CreateAirCraftPredator(pWorld);
	air_predator->SetTypeId(AIR_PREDATOR);

	GLRole_Predator* role_air_predator=new GLRole_Predator(air_predator);
	role_air_predator->AddPreyId(AIR_FLOCK);
	role_air_predator->SetKiller(false);
	air_predator->AddRole("predator_role", role_air_predator);

	GLSteering_Pursuit* pursuit_air_predator=new GLSteering_Pursuit(air_predator);
	role_air_predator->SetPursuitSteeringHandler(pursuit_air_predator);
	pursuit_air_predator->TurnOn();
	air_predator->AddSteering("pursuit", pursuit_air_predator);

	pWorld->AddAgent(air_predator);

	//unsigned int iAirCraftPreyCount=pWorld->IsHighEndSystem() ? 9 : 3;
	for(unsigned int i=0; i<m_iAirCraftPreyCount; i++)
	{
		Vehicle* pPrey=CreateAirCraftPrey(pWorld);
		pPrey->SetTypeId(AIR_FLOCK);

		GLRole_Prey* role_prey=new GLRole_Prey(pPrey);
		role_prey->AddPredatorId(AIR_PREDATOR);
		pPrey->AddRole("prey_role", role_prey);

		GLSteering_Evade* evade=new GLSteering_Evade(pPrey);
		role_prey->SetEvadeSteeringHandler(evade);
		evade->TurnOn();
		pPrey->AddSteering("evade", evade);

		pWorld->AddAgent(pPrey);
	}

	Vehicle* land_predator=CreateLandCraftPredator(pWorld);
	land_predator->SetTypeId(LAND_PREDATOR);

	GLRole_Predator* role_land_predator=new GLRole_Predator(land_predator);
	role_land_predator->AddPreyId(LAND_FLOCK);
	land_predator->AddRole("predator_role", role_land_predator);

	GLSteering_Pursuit* pursuit_land_predator=new GLSteering_Pursuit(land_predator);
	role_land_predator->SetPursuitSteeringHandler(pursuit_land_predator);
	pursuit_land_predator->TurnOn();
	land_predator->AddSteering("pursuit", pursuit_land_predator);

	pWorld->AddAgent(land_predator);

	//unsigned int iLandCraftPreyCount=pWorld->IsHighEndSystem() ? 9 : 6;
	for(unsigned int i=0; i<m_iLandCraftPreyCount; i++)
	{
		Vehicle* pPrey=CreateLandCraftPrey(pWorld);
		pPrey->SetTypeId(LAND_FLOCK);

		GLRole_Prey* role_prey=new GLRole_Prey(pPrey);
		role_prey->AddPredatorId(LAND_PREDATOR);
		pPrey->AddRole("prey_role", role_prey);

		GLSteering_Evade* evade=new GLSteering_Evade(pPrey);
		role_prey->SetEvadeSteeringHandler(evade);
		evade->TurnOn();
		pPrey->AddSteering("evade", evade);

		pWorld->AddAgent(pPrey);
	}

	//unsigned int iTreeCount=pWorld->IsHighEndSystem() ? 15 : 3;
	for(unsigned int i=0; i<m_iTreeCount; i++)
	{
		pWorld->AddObstacle(CreateTree(pWorld));
	}
}

void GLState_AStar::Exited(GameWorld *pWorld)
{
	pWorld->RemoveAgents();
	pWorld->RemoveObstacles();

	pWorld->ShowFloor(true);
	pWorld->ShowTerrain(false);
}

void GLState_AStar::MouseButtonDown(GameWorld* pWorld)
{
	if(pWorld->GetMouse().IsRightButtonDown())
	{
		pWorld->MonitorSelectedAgent();
	}
}

GLState_AStar* GLState_AStar::Instance()
{
	static GLState_AStar theInstance;
	return &theInstance;
}

Vehicle* GLState_AStar::CreateLandCraftPrey(GameWorld* pWorld)
{
	Vehicle* pAgent=new Vehicle(pWorld);
	//pAgent->SetMaxForce(8);
	pAgent->SetMass(1);
	pAgent->SetMaxSpeed(10);
	pAgent->SetVisibleDistance(glMath.nextDouble(50, 100));
	pAgent->SetModel("WalkMech.md2");
	pAgent->GetAnimator().SetMaxFPS(64);
	pAgent->EnableSound(false);
	pAgent->set_bound_radius(2);

	/*
	GLSteering_PathFollow* path_follow=new GLSteering_PathFollow(pAgent);
	path_follow->TurnOn();
	path_follow->SetPath(&(pWorld->GetPath()));
	pAgent->AddSteering("path follow", path_follow);
	*/

	GLSteering_Avoidance* avoidance=new GLSteering_Avoidance(pAgent);
	// avoidance->SetMinDetectionBoxLength(10);
	avoidance->SetWeight(3.0);
	avoidance->TurnOn();
	pAgent->AddSteering("avoidance", avoidance);

	GLSteering_Separation* separation=new GLSteering_Separation(pAgent);
	separation->SetWeight(3.0);
	separation->TurnOn();
	pAgent->AddSteering("separation", separation);
	
	GLSteering_Alignment* alignment=new GLSteering_Alignment(pAgent);
	alignment->SetWeight(1.0);
	alignment->TurnOn();
	pAgent->AddSteering("alignment", alignment);

	GLSteering_Cohesion* cohesion=new GLSteering_Cohesion(pAgent);
	cohesion->SetWeight(3.0);
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

Vehicle* GLState_AStar::CreateAirCraftPrey(GameWorld* pWorld)
{
	Vehicle* pAgent=new Vehicle(pWorld);
	pAgent->SetLocomotion(new GLLocomotion_Aircraft(pAgent));
	pAgent->SetZoneId(GameEntity::ZONE_AIR);
	pAgent->SetHoverHeight(20);
	//pAgent->SetMaxForce(15);
	pAgent->SetMass(1);
	pAgent->SetMaxSpeed(20);
	pAgent->SetVisibleDistance(100);
	if(m_AirCraftPreyModel.compare("")==0)
	{
		pAgent->SetModel("fighter1.3ds");
	}
	else
	{
		pAgent->SetModel(m_AirCraftPreyModel.c_str());
	}
	pAgent->GetAnimator().SetMaxFPS(64);
	pAgent->EnableSound(false);
	pAgent->set_bound_radius(4);

	/*
	GLSteering_PathFollow* path_follow=new GLSteering_PathFollow(pAgent);
	path_follow->TurnOn();
	path_follow->SetPath(&(pWorld->GetPath()));
	pAgent->AddSteering("path follow", path_follow);
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

Obstacle* GLState_AStar::CreateTree(GameWorld* pWorld)
{
	double len2=pWorld->GetTerrain().GetLength() / 3.0;
	double width2=pWorld->GetTerrain().GetWidth() / 3.0;

	GLPlant* pObs=new GLPlant(pWorld);
	pObs->set_bound_radius(glMath.nextDouble(2, 4));
	//pObs->SetModel("tree.3ds");
	if(glMath.nextDouble() < 0.3)
	{
		pObs->SetModel("tree2.3ds");
	}
	else if(glMath.nextDouble() < 0.6)
	{
		pObs->SetModel("bigtree.3ds");
	}
	else
	{
		pObs->SetModel("pinetree.3ds");
	}
	
	double x, y, z;
	int counter=0;
	do{
		x=glMath.nextDouble(-width2, width2);
		z=glMath.nextDouble(-len2, len2);
		y=pWorld->GetEnvHeight(x, z);
		if(counter > 10) break;
		counter++;
	}while(y > 0.2);

	pObs->set_position(GLVector(x, y, z));

	return pObs;
}

