#ifndef _H_GL_ROLE_PREY_H
#define _H_GL_ROLE_PREY_H

#include "GLRole.h"
#include <set>
#include "../GLSteering/GLSteering_Evade.h"

class GLRole_Prey : public GLRole
{
public:
	GLRole_Prey(GameEntity* pAgent);
	virtual ~GLRole_Prey();

public:
	virtual void Update(const long& lElapsedTicks);
	virtual bool IsPenetratable(int TypeId) const;

public:
	void AddPredatorId(int id) { m_predator_ids.insert(id); }
	void SetEvadeSteeringHandler(GLSteering_Evade* pEvadeSteeringHandler) { m_pEvadeSteeringHandler=pEvadeSteeringHandler; }

protected:
	std::set<int> m_predator_ids;
	GLSteering_Evade* m_pEvadeSteeringHandler;
	bool m_bBroadcast;
};

#endif