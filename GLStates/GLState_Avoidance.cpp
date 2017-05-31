#include "GLState_Avoidance.h"
#include "../GameWorld/GameWorld.h"
#include "../GameEntity/GLTower.h"
#include "../GameEntity/Vehicle.h"
#include "../GLMath/GLMath.h"
#include "../GLSteering/GLSteering_Avoidance.h"
#include "../GLSteering/GLSteering_Wander.h"
#include "../GLSteering/GLSteering_Cohesion.h"
#include "../GLSteering/GLSteering_Alignment.h"
#include "../GLSteering/GLSteering_Separation.h"
#include "../GLSteering/GLSteering_Evade.h"
#include "../GLSteering/GLSteering_Pursuit.h"
#include "../GLConsole/GLConsole.h"
#include "../GLRole/GLRole_Predator.h"
#include <cfloat>
#include <set>
#include "../tinyxml/tinyxml.h"
#include "../GLUtil/GLExitSystem.h"
#include "../GLRole/GLRole_Prey.h"

GLState_Avoidance::GLState_Avoidance()
: GLState()
, m_Tower_Model("")
, m_bScriptLoaded(false)
, m_iObsCount(0)
, m_iPreyCount(0)
{
	SetTitleTexture("images\\button_normal_demo_avoidance.jpg");
	m_state_name="State_Avoidance";
}

void GLState_Avoidance::LoadScript(GameWorld* pWorld)
{
	bool bHighEndSystem=pWorld->IsHighEndSystem();

	TiXmlDocument doc;
	if(!doc.LoadFile("GLScripts\\glState_Avoidance.xml"))
	{
		GLExitSystem("GLState_Avoidance::GLState_Avoidance()", "Failed to load glState_Avoidance.xml");
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
					if(strcmp(xml_level2->Attribute("name"), "Tower_Model")==0)
					{
						m_Tower_Model=bHighEndSystem ? xml_level2->Attribute("high_end_value") : xml_level2->Attribute("low_end_value");
					}
					else if(strcmp(xml_level2->Attribute("name"), "iObsCount")==0)
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
						m_iObsCount=value;
					}
					else if(strcmp(xml_level2->Attribute("name"), "iPreyCount")==0)
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
						m_iPreyCount=value;
					}
				}
			}
		}
	}
}

void GLState_Avoidance::Entered(GameWorld *pWorld)
{
	if(!m_bScriptLoaded)
	{
		LoadScript(pWorld);
		m_bScriptLoaded=true;
	}

	pWorld->RemoveObstacles();
	pWorld->RemoveAgents();

	glConsole << "\nState - Avoidance: \n";
	glConsole << "click mouse on the plane to generate new tower building\n";

	//unsigned int iObsCount=pWorld->IsHighEndSystem() ? 9 : 3;
	for(unsigned int i=0; i<m_iObsCount; i++)
	{
		Obstacle* pObs=CreateObstacle(pWorld);
		pWorld->AddObstacle(pObs);
	}

	Vehicle* predator=CreatePredator(pWorld);
	predator->SetColor(1.0, 0.0, 0.0);

	GLRole_Predator* role1=new GLRole_Predator(predator);
	role1->AddPreyId(AGENT);
	predator->AddRole("predator", role1);

	GLSteering_Pursuit* pursuit=new GLSteering_Pursuit(predator);
	role1->SetPursuitSteeringHandler(pursuit);
	pursuit->TurnOn();
	predator->AddSteering("pursuit", pursuit);

	pWorld->AddAgent(predator);

	//unsigned int iPreyCount=pWorld->IsHighEndSystem() ? 9 : 6;
	for(unsigned int i=0; i<m_iPreyCount; i++)
	{
		Vehicle* pPrey=CreateAgent(pWorld);

		GLRole_Prey* role_prey=new GLRole_Prey(pPrey);
		role_prey->AddPredatorId(PREDATOR);
		pPrey->AddRole("prey_role", role_prey);

		GLSteering_Evade* evade=new GLSteering_Evade(pPrey);
		role_prey->SetEvadeSteeringHandler(evade);
		evade->TurnOn();
		pPrey->AddSteering("evade", evade);

		pWorld->AddAgent(pPrey);
	}
}

GLState_Avoidance* GLState_Avoidance::Instance()
{
	static GLState_Avoidance theInstance;
	return &theInstance;
}

void GLState_Avoidance::Exited(GameWorld *pWorld)
{
	pWorld->RemoveObstacles();
	pWorld->RemoveAgents();
}

void GLState_Avoidance::Update(GameWorld *pWorld, const long &lElapsedTicks)
{
	pWorld->UpdateAgents(lElapsedTicks);
	pWorld->UpdateObstacles(lElapsedTicks);
}

Obstacle* GLState_Avoidance::CreateObstacle(GameWorld* pWorld)
{
	double side=pWorld->GetFloor().GetLength() / 2.0;

	GLTower* pObs=new GLTower(pWorld);
	pObs->set_bound_radius(2);
	pObs->set_position(GLVector(glMath.nextDouble(-side, side), 0, glMath.nextDouble(-side, side)));
	if(m_Tower_Model.compare("") != 0)
	{
		pObs->SetModel(m_Tower_Model.c_str());
	}
	pObs->SetCaptureId(AGENT);

	return pObs;
}

Vehicle* GLState_Avoidance::CreateAgent(GameWorld* pWorld)
{
	Vehicle* pAgent=new Vehicle(pWorld);
	pAgent->SetMass(1);
	//pAgent->SetMaxForce(6.0);
	pAgent->SetMaxSpeed(8.0);
	pAgent->SetModel("WalkMech.md2");
	pAgent->GetAnimator().SetMaxFPS(64);
	pAgent->SetTypeId(AGENT);
	pAgent->EnableSound(false);

	double side=pWorld->GetFloor().GetLength() / 4.0;
	pAgent->set_position(GLVector(glMath.nextDouble(-side, side), 0, glMath.nextDouble(-side, side)));

	GLSteering_Avoidance* avoidance=new GLSteering_Avoidance(pAgent);
	// avoidance->SetMinDetectionBoxLength(10);
	avoidance->SetWeight(10);
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
	cohesion->SetWeight(1.0);
	cohesion->TurnOn();
	pAgent->AddSteering("cohesion", cohesion);

	GLSteering_Wander* wander=new GLSteering_Wander(pAgent);
	wander->SetWanderDistance(5.0);
	wander->SetWanderJitter(1.0);
	wander->SetWanderRadius(3.0);
	wander->SetWeight(1.0);
	wander->TurnOn();
	pAgent->AddSteering("wander", wander);

	pAgent->SetVisibleDistance(30.0);

	return pAgent;
}

Vehicle* GLState_Avoidance::CreatePredator(GameWorld* pWorld)
{
	Vehicle* pAgent=new Vehicle(pWorld);
	//pAgent->SetMaxForce(6);
	pAgent->SetMass(1);
	pAgent->SetMaxSpeed(12);
	pAgent->SetModel("WalkMech.md2");
	pAgent->GetAnimator().SetMaxFPS(64);
	pAgent->SetTypeId(PREDATOR);
	pAgent->EnableSound(false);

	GLSteering_Avoidance* avoidance=new GLSteering_Avoidance(pAgent);
	// avoidance->SetMinDetectionBoxLength(10);
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

void GLState_Avoidance::MouseButtonDown(GameWorld* pWorld)
{
	if(pWorld->GetMouse().IsLeftButtonDown())
	{
		pWorld->GetCamera().PreTransform();
		pWorld->GetCamera().Transform();
		GLRay mouse_ray=pWorld->GetMouse().GetRay();
		pWorld->GetCamera().PostTransform();

		GLVector intersection_point;
		if(mouse_ray.intersect(&(pWorld->GetFloor()), intersection_point))
		{
			Obstacle* pObs=CreateObstacle(pWorld);
			pObs->set_position(intersection_point);
			pWorld->AddObstacle(pObs);
		}
	}
	else if(pWorld->GetMouse().IsRightButtonDown())
	{
		pWorld->MonitorSelectedAgent();
	}
}