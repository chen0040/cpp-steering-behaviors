#include "PSOAgent.h"
#include "../GameWorld/GameWorld.h"
#include "../GLMath/GLMath.h"
#include "../GLTestFunc/GLTestFunc_PotentialField.h"
#include "../GLTestFunc/GLTestFunc_InverseMexicanHat.h"
#include <iostream>
#include <glut.h>

GLVector PSOAgent::m_global_best_position;

PSOAgent::PSOAgent(GameWorld* pWorld)
: Vehicle(pWorld)
, m_C1(1)
, m_C2(2)
, m_ftscale(1)
, m_pBehaviorFunc(NULL)
, m_dy(0)
, m_ddy(0.1)
{
	EnableWorldWrap(false);
	SetBehaviorFunc(&glTestFunc_PotentialField);
	// SetBehaviorFunc(&glTestFunc_InverseMexicanHat);
	InitializePSOVelocity();
}

PSOAgent::~PSOAgent()
{

}

void PSOAgent::PreRender()
{
	Vehicle::PreRender();
	glPushMatrix();

	if(m_dHeight > 0)
	{
		double yvar=m_ddy;
		m_dy+=glMath.nextDouble() * (yvar * 2) - yvar;
		if(m_dy > yvar) m_dy=yvar;
		if(m_dy < -yvar) m_dy=-yvar;

		glTranslated(0, m_dy, 0);
	}
}

void PSOAgent::PostRender()
{
	glPopMatrix();

	Vehicle::PostRender();
}

void PSOAgent::Render()
{
	if(m_animator.GetModel() != NULL)
	{
		glTranslated(0, m_dHeight, 0);
		Vehicle::Render();
	}
	else
	{
		glTranslated(0, m_dHeight, 0);
		glColor3d(m_vColor.x, m_vColor.y, m_vColor.z);

		glutSolidSphere(m_bounding_radius / 2, 10, 10);
		
		/*
		int r=static_cast<int>(m_bounding_radius);
		glDisable(GL_CULL_FACE);
		glBegin(GL_QUADS);
		glVertex3i(r, 0, r);
		glVertex3i(r, 0, -r);
		glVertex3i(-r, 0, -r);
		glVertex3i(-r, 0, r);
		glEnd();
		glEnable(GL_CULL_FACE);
		*/
	}
}

// Xianshun says:
// this function evaluate the potential of the point pos in the current force field landscape
// the lower the potential the more likely the swarm agent will move to
double PSOAgent::EvaluatePSO(const GLVector& pos)
{
	if(m_pBehaviorFunc != NULL)
	{
		m_pBehaviorFunc->SetScale(m_ftscale);

		double potential=0;
		for(size_t i=0; i<m_targets.size(); i++)
		{
			GLVector target_position=m_targets[i].first->get_position();
			double dx=target_position.x - pos.x;
			double dz=target_position.z - pos.z;

			potential+=m_pBehaviorFunc->GetValue(dx, dz) * m_targets[i].second;
		}

		return potential;
	}
	else
	{
		double potential=0;
		for(size_t i=0; i<m_targets.size(); i++)
		{
			GLVector target_position=m_targets[i].first->get_position();
			double dx=target_position.x - pos.x;
			double dz=target_position.z - pos.z;
			double d=sqrt(dx * dx + dz * dz);
			if(d==0) d=1E-10;
			potential+=(-1 / d) * m_targets[i].second;
		}
		
		return potential;
	}
}

void PSOAgent::InitializePSOVelocity()
{
	this->m_vVelocity.x=glMath.nextDouble();
	this->m_vVelocity.y=glMath.nextDouble();
	this->m_vVelocity.z=glMath.nextDouble();

	this->m_vVelocity *= glMath.nextDouble(-this->GetMaxSpeed(), this->GetMaxSpeed());
	this->m_vVelocity = this->m_vVelocity.Truncate(this->GetMaxSpeed());
}

void PSOAgent::InitializePSOPosition(const GLVector& pos)
{
	this->set_position(pos);
	m_local_best_position=this->get_position();
	
	UpdatePSOLocalBestPosition();
	UpdatePSOGlobalBestPosition();
}

void PSOAgent::UpdatePSOPosition(const double& elapsed_time)
{
	m_position+=m_vVelocity * elapsed_time;
	this->set_position(m_position);
}

void PSOAgent::AddTarget_Attraction(GameEntity* pTarget, double weight)
{
	m_targets.push_back(std::make_pair<>(pTarget, weight));
}

void PSOAgent::AddTarget_Repulsion(GameEntity* pTarget, double weight)
{
	m_targets.push_back(std::make_pair<>(pTarget, -weight));
}

void PSOAgent::UpdatePSOVelocity()
{
	double r1=glMath.nextDouble();
	double r2=glMath.nextDouble();
	double r3=glMath.nextDouble();

	double w=0.5 + r3 / 2;

	this->m_vVelocity = this->m_vVelocity * w + (m_local_best_position - m_position) * r1 * m_C1 + (m_global_best_position - m_position) * r2 * m_C2;
	this->m_vVelocity.Truncate(this->GetMaxSpeed());
}

void PSOAgent::UpdatePSOGlobalBestPosition()
{
	double current_potential=EvaluatePSO(this->m_position);
	double global_best_potential=EvaluatePSO(m_global_best_position);
	if(current_potential < global_best_potential)
	{
		m_global_best_position=this->m_position;
	}
}

void PSOAgent::UpdatePSOLocalBestPosition()
{
	double current_potential=EvaluatePSO(this->m_position);
	double local_best_potential=EvaluatePSO(m_local_best_position);
	if(current_potential < local_best_potential)
	{
		m_local_best_position=this->m_position;
	}
}

void PSOAgent::Update(const long& lElapsedTicks)
{
	MovingEntity::Update(lElapsedTicks);

	double elapsed_time=(double)lElapsedTicks / 1000.0;

	UpdatePSOGlobalBestPosition();
	UpdatePSOLocalBestPosition();
	UpdatePSOVelocity();
	UpdatePSOPosition(elapsed_time);

	double speedSq=m_vVelocity.lengthSq();
	double speed=sqrt(speedSq);
	if(speedSq > 0.00000001)
	{
		m_vHeading=m_vVelocity / speed;
		m_vSide=GLVector(-m_vHeading.z, m_vHeading.y, m_vHeading.x);
	}

	if(m_pLocomotion != NULL)
	{
		m_pLocomotion->Update(lElapsedTicks);
	}

	if(m_animator.GetModel() != NULL)
	{
		UpdateFPS(speed);
		m_animator.Update(lElapsedTicks);
	}

	this->UpdateRoleAction(lElapsedTicks);
}