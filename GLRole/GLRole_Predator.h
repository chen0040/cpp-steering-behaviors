#ifndef _H_GL_PREDATOR_ROLE_H
#define _H_GL_PREDATOR_ROLE_H

#include "../GLVector/GLVector.h"
#include "GLRole.h"
#include "../GLSteering/GLSteering_Pursuit.h"
#include <set>

class GameEntity;

class GLRole_Predator : public GLRole
{
public:
	GLRole_Predator(GameEntity* pAgent);
	virtual ~GLRole_Predator();

public:
	virtual void Update(const long& lElapsedTicks);
	virtual bool IsPenetratable(int TypeId) const;

public:
	void AddPreyId(int id) { m_prey_ids.insert(id); }
	void SetCorpseSceneDuration(double duration);
	void SetPursuitSteeringHandler(GLSteering_Pursuit* pPursuitSteeringHandler) { m_pPursuitSteeringHandler=pPursuitSteeringHandler; }

public:
	void SetKiller(bool bIsKiller) { m_bIsKiller=bIsKiller; }
	bool IsKiller() const { return m_bIsKiller; }

protected:
	bool m_bIsKiller;
	std::set<int> m_prey_ids;
	double m_corpse_scene_duration;
	GLSteering_Pursuit* m_pPursuitSteeringHandler;
};
#endif