#include "GLState_PSO.h"
#include "../GameWorld/GameWorld.h"
#include "../GameEntity/PSOAgent.h"
#include "../GLMath/GLMath.h"
#include "../GLConsole/GLConsole.h"
#include "../GLSteering/GLSteering_Evade.h"
#include "../GLSteering/GLSteering_Pursuit.h"
#include "../GLSteering/GLSteering_Wander.h"
#include "../GameEntity/Obstacle.h"
#include "../GLRole/GLRole_Predator.h"
#include <cfloat>
#include <algorithm>
#include "../tinyxml/tinyxml.h"
#include "../GLUtil/GLExitSystem.h"
#include "../GameEntity/GLRadar.h"

const int GLState_PSO::PREY_1=1;
const int GLState_PSO::PSO_AGENT_1=2;

GLState_PSO::GLState_PSO()
: GLState()
, m_predator(NULL)
, m_PSOTarget(NULL)
, m_PSOAgent_model("")
, m_bScriptLoaded(false)
, m_iPSOAgentCount(0)
{
	this->SetTitleTexture("images\\button_normal_demo_pso.jpg");
	m_state_name="State_PSO";
}

GLState_PSO::~GLState_PSO()
{

}

void GLState_PSO::LoadScript(GameWorld* pWorld)
{
	bool bHighEndSystem=pWorld->IsHighEndSystem();

	TiXmlDocument doc;
	if(!doc.LoadFile("GLScripts\\glState_PSO.xml"))
	{
		GLExitSystem("GLState_PSO::GLState_PSO()", "Failed to load glState_PSO.xml");
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
					if(strcmp(xml_level2->Attribute("name"), "PSOAgent_Model")==0)
					{
						m_PSOAgent_model=bHighEndSystem ? xml_level2->Attribute("high_end_value") : xml_level2->Attribute("low_end_value");
					}
					else if(strcmp(xml_level2->Attribute("name"), "iPSOAgentCount")==0)
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
						m_iPSOAgentCount=value;
					}
				}
			}
		}
	}
}

void GLState_PSO::Update(GameWorld* pWorld, const long& lElapsedTicks)
{
	AgentGroup& agents=pWorld->GetMutableAgents();
	AgentGroup::iterator pos_agent;
	GameEntity* pAgent=NULL;

	pos_agent=std::find(agents.begin(), agents.end(), m_PSOTarget);
	if(pos_agent == agents.end())
	{
		m_PSOTarget=this->CreatePSOTarget(pWorld);
		m_PSOTarget->SetTypeId(PREY_1);

		double length=pWorld->GetTerrain().GetLength();
		double width=pWorld->GetTerrain().GetWidth();
		double length2=length / 2;
		double width2=width / 2;

		m_PSOTarget->set_position(GLVector(glMath.nextDouble(-width2, width2), 0, glMath.nextDouble(-length2, length2)));
		
		for(pos_agent=agents.begin(); pos_agent != agents.end(); ++pos_agent)
		{
			pAgent=(*pos_agent);
			if(pAgent->GetTypeId()==PSO_AGENT_1)
			{
				PSOAgent* pso_agent=static_cast<PSOAgent*>(*pos_agent);
				pso_agent->RemoveAllTargets();
				pso_agent->AddTarget_Attraction(m_PSOTarget, 1);
				pso_agent->AddTarget_Repulsion(m_predator, 0.08);
			}
		}

		pWorld->AddAgent(m_PSOTarget);
	}

	pWorld->UpdateObstacles(lElapsedTicks);
	
	for(pos_agent=agents.begin(); pos_agent != agents.end(); ++pos_agent)
	{
		pAgent=(*pos_agent);
		pAgent->Update(lElapsedTicks);
	}
}

void GLState_PSO::Entered(GameWorld *pWorld)
{
	if(!m_bScriptLoaded)
	{
		this->LoadScript(pWorld);
		m_bScriptLoaded=true;
	}

	glConsole << "\nState - PSO\n";

	pWorld->ShowFloor(false);
	pWorld->ShowTerrain(true);
	if(!pWorld->GetTerrain().IsLoadedFromImage() || pWorld->GetTerrain().GetLoadedImageName().compare("height_map.jpg") !=0 )
	{
		pWorld->GetTerrain().LoadFromImage("height_map.jpg", pWorld);	
	}
	pWorld->RemoveAgents();
	pWorld->RemoveObstacles();

	m_predator=this->CreatePredator(pWorld);
	m_PSOTarget=this->CreatePSOTarget(pWorld);
	m_PSOTarget->SetTypeId(PREY_1);
	pWorld->AddAgent(m_predator);
	pWorld->AddAgent(m_PSOTarget);

	/*
	GLSteering_Pursuit* pursuit=new GLSteering_Pursuit(m_predator);
	pursuit->TurnOn();
	GLSteering_Evade* evade=new GLSteering_Evade(m_PSOTarget);
	evade->TurnOn();
	
	evade->SetEvadeTarget(m_predator);
	pursuit->SetTarget(m_PSOTarget);

	m_predator->AddSteering("pursuit", pursuit);
	m_PSOTarget->AddSteering("evade", evade);
	*/

	Obstacle* obs1=this->CreateObstacle(pWorld);
	pWorld->AddObstacle(obs1);
	obs1->set_position(GLVector(0, 0, 0));
	
	Obstacle* obs2=this->CreateObstacle(pWorld);
	pWorld->AddObstacle(obs2);
	obs2->set_position(GLVector(-54, 0, -54));

	//unsigned int iPSOAgentCount=pWorld->IsHighEndSystem() ? 64 : 32; //128 : 64;
	for(unsigned int i=0; i<m_iPSOAgentCount; i++)
	{
		PSOAgent* pso_agent=this->CreatePSOAgent(pWorld);
		pso_agent->SetTypeId(PSO_AGENT_1);
		pso_agent->AddTarget_Attraction(m_PSOTarget, 1);
		pso_agent->AddTarget_Repulsion(m_predator, 0.08);
		//pso_agent->AddTarget_Repulsion(obs1, 0.01);
		pWorld->AddAgent(pso_agent);
	}	
}

Obstacle* GLState_PSO::CreateObstacle(GameWorld* pWorld)
{
	double width =pWorld->GetTerrain().GetWidth() / 2.0;
	double length=pWorld->GetTerrain().GetLength() / 2.0;

	/*
	for(unsigned int i=0; i<10; i++)
	{
		dx=glMath.nextDouble(-width, width);
		dz=glMath.nextDouble(-length, length);
		if(pWorld->GetEnvHeight(dx, dz) < 0.1)
		{
			break;
		}
	}
	*/

	GLRadar* pObs=new GLRadar(pWorld);
	pObs->set_bound_radius(glMath.nextDouble(2, 4));
	pObs->SetTransmisionPoint(GLVector(0, 14, 0));
	pObs->SetModel("radio_dish.3ds");
	
	pObs->ShowDot(true);

	return pObs;
}

void GLState_PSO::Exited(GameWorld *pWorld)
{
	pWorld->RemoveAgents();
	pWorld->RemoveObstacles();

	pWorld->ShowFloor(true);
	pWorld->ShowTerrain(false);
}

void GLState_PSO::MouseButtonDown(GameWorld* pWorld)
{
	if(pWorld->GetMouse().IsRightButtonDown())
	{
		pWorld->MonitorSelectedAgent();
	}
}

GLState_PSO* GLState_PSO::Instance()
{
	static GLState_PSO theInstance;
	return &theInstance;
}

PSOAgent* GLState_PSO::CreatePSOAgent(GameWorld* pWorld)
{
	PSOAgent* pAgent=new PSOAgent(pWorld);
	pAgent->SetMaxSpeed(5);
	pAgent->EnableSound(false);
	pAgent->set_bound_radius(1.0);
	//pAgent->set_bound_radius(1.5);
	pAgent->SetColor(1, 1, 0);
	pAgent->SetHoverHeight(5);
	pAgent->SetVerticalOscilationRange(0.2);
	if(m_PSOAgent_model.compare("")!=0)
	{
		pAgent->SetModel(m_PSOAgent_model.c_str());
	}
	
	
	GLRole_Predator* role=new GLRole_Predator(pAgent);
	role->AddPreyId(PREY_1);
	role->SetCorpseSceneDuration(2);
	pAgent->AddRole("predator_role", role);

	double length=pWorld->GetTerrain().GetLength();
	double width=pWorld->GetTerrain().GetWidth();
	double length2=length / 2;
	double width2=width / 2;


	pAgent->InitializePSOPosition(GLVector(glMath.nextDouble(-width2, width2), 0, glMath.nextDouble(-length2, length2)));
	
	return pAgent;
}

Vehicle* GLState_PSO::CreatePSOTarget(GameWorld* pWorld)
{
	Vehicle* pAgent=new Vehicle(pWorld);
	pAgent->SetModel("WalkMech.md2");

	pAgent->SetMass(1);
	//pAgent->SetMaxForce(2.0);
	pAgent->SetMaxSpeed(10.0);
	pAgent->GetAnimator().SetMaxFPS(50);
	pAgent->set_position(GLVector(-4, 0, -8));
	pAgent->EnableSound(false);
	pAgent->SetColor(0, 1, 0);

	GLSteering_Wander* pAgentSteering2=new GLSteering_Wander(pAgent);
	pAgentSteering2->TurnOn();
	pAgentSteering2->SetWanderDistance(5);
	pAgentSteering2->SetWanderRadius(3);
	pAgentSteering2->SetWanderJitter(1);

	pAgent->AddSteering("wander", pAgentSteering2);

	return pAgent;
}

Vehicle* GLState_PSO::CreatePredator(GameWorld* pWorld)
{
	Vehicle* pAgent=new Vehicle(pWorld);
	pAgent->SetModel("WalkMech.md2");

	pAgent->set_position(GLVector(4, 0, 8));
	pAgent->SetMass(1);
	//pAgent->SetMaxForce(2.0);
	pAgent->SetMaxSpeed(5.0);
	pAgent->GetAnimator().SetMaxFPS(35);
	pAgent->EnableSound(false);

	GLSteering_Wander* pAgentSteering2=new GLSteering_Wander(pAgent);
	pAgentSteering2->SetWanderDistance(5);
	pAgentSteering2->SetWanderRadius(3);
	pAgentSteering2->SetWanderJitter(1);
	pAgentSteering2->TurnOn();

	pAgent->AddSteering("wander", pAgentSteering2);
	pAgent->SetColor(1.0, 0.0, 0.0);

	return pAgent;
}