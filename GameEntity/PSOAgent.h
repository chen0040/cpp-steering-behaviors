#ifndef _H_PSO_AGENT_H
#define _H_PSO_AGENT_H

#include "Vehicle.h"
#include "../GLTestFunc/GLTestFunc.h"

class PSOAgent : public Vehicle
{
public:
	PSOAgent(GameWorld* pWorld);
	virtual ~PSOAgent();

public:
	virtual void Update(const long& lElapsedTicks);
	virtual void Render();
	virtual void PreRender();
	virtual void PostRender();

protected:
	double m_dy;
	double m_ddy;
public:
	void SetVerticalOscilationRange(double ddy) { m_ddy=ddy; }

protected:
	void UpdatePSOGlobalBestPosition();
	void UpdatePSOLocalBestPosition();
	void UpdatePSOVelocity();
	void UpdatePSOPosition(const double& elapsed_time);
	void InitializePSOVelocity();
	double EvaluatePSO(const GLVector& pos);
public:
	void InitializePSOPosition(const GLVector& pos);

protected:
	double m_ftscale;
public:
	void SetBehaviorFunc(GLTestFunc* tf) { m_pBehaviorFunc=tf; }

public:
	void AddTarget_Attraction(GameEntity* pTarget, double weight=1);
	void AddTarget_Repulsion(GameEntity* pEvadeTarget, double weight=0.1);
	void RemoveAllTargets() { m_targets.clear(); }

protected:
	static GLVector m_global_best_position;
	GLVector m_local_best_position;
	double m_C1;
	double m_C2;
	GLTestFunc* m_pBehaviorFunc;
	std::vector<std::pair<GameEntity*, double> > m_targets;
};
#endif