#include "GLSteering_Composite.h"
#include "../GameEntity/Vehicle.h"
#include "../GLMath/GLMath.h"
#include <algorithm>

GLSteering_Composite::GLSteering_Composite(Vehicle* pVehicle)
: GLSteering(pVehicle)
{
	m_type="composite";
}

GLSteering_Composite::~GLSteering_Composite()
{
	std::list<GLSteering*>::iterator pos;
	for(pos=m_steerings.begin(); pos!=m_steerings.end(); ++pos)
	{
		delete *pos;
	}

	m_steerings.clear();
}

GLVector GLSteering_Composite::calculate()
{
	GLVector vForce;
	std::list<GLSteering*>::iterator pos=m_steerings.begin();
	GLSteering* pSteering=NULL;
	bool untagged=true;
	while(pos != m_steerings.end())
	{
		pSteering=*pos;
		if(pSteering->On())
		{
			if(pSteering->is_a("cohesion") || pSteering->is_a("alignment") || pSteering->is_a("separation"))
			{
				if(untagged)
				{
					m_pVehicle->tag_neighbors(m_pVehicle->GetVisibleDistance());
					untagged=false;
				}
			}
			
			GLVector vSteeringForce=pSteering->calculate() * pSteering->GetWeight();

			/*
			double force=vSteeringForce.length();
			if(force > glMath.Epsilon())
			{
				if(pSteering->is_a("avoidance"))
				{
					std::cout << "steering: " << pSteering->get_name() << std::endl;
					std::cout << "force: " <<  force << std::endl;
				}
			}
			*/

			if(!AccumulateForce(vForce, vSteeringForce))
			{
				return vForce;
			}
		}
		++pos;
	}

	return vForce;
}

void GLSteering_Composite::AddSteering(const char* sname, GLSteering* pSteering)
{
	m_steerings.push_back(pSteering);
	std::string strname=sname;
	m_steering_map[strname]=pSteering;
}

void GLSteering_Composite::RemoveSteering(const char* sname)
{
	std::string strname=sname;
	std::map<std::string, GLSteering*>::iterator pos_map_steering=m_steering_map.find(strname);
	if(pos_map_steering == m_steering_map.end())
	{
		return;
	}
	GLSteering* pSteering=pos_map_steering->second;
	m_steering_map.erase(pos_map_steering);
	std::list<GLSteering*>::iterator pos_steering=std::find(m_steerings.begin(), m_steerings.end(), pSteering);
	if(pos_steering==m_steerings.end())
	{
		return;
	}
	m_steerings.erase(pos_steering);
	delete pSteering;
	pSteering=NULL;
}

GLSteering* GLSteering_Composite::GetSteering(std::string sname)
{
	std::map<std::string, GLSteering*>::iterator pos=m_steering_map.find(sname);

	if(pos != m_steering_map.end())
	{
		return pos->second;
	}

	return NULL;
}

bool GLSteering_Composite::AccumulateForce(GLVector & vForce, const GLVector& vSteeringForce)
{
	double dForce=vForce.length();

	double remaining_force=m_pVehicle->GetMaxForce() - dForce;

	if(remaining_force < 0) return false;

	vForce+=vSteeringForce;
	vForce=vForce.Truncate(m_pVehicle->GetMaxForce());

	return true;
}

void GLSteering_Composite::TurnOn()
{
	GLSteering::TurnOn();

	std::list<GLSteering*>::iterator pos_steering;
	for(pos_steering=m_steerings.begin(); pos_steering != m_steerings.end(); ++pos_steering)
	{
		(*pos_steering)->TurnOn();
	}
}

