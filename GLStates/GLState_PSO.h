#ifndef _H_GL_STATE_PSO_H
#define _H_GL_STATE_PSO_H

#include "GLState.h"

class GameWorld;
class Vehicle;
class PSOAgent;
class Obstacle;

class GLState_PSO : public GLState
{
private:
	GLState_PSO();
	GLState_PSO(const GLState_PSO& rhs) { }
	GLState_PSO& operator=(const GLState_PSO& rhs) { return *this; }

public:
	static GLState_PSO* Instance();
	virtual ~GLState_PSO();

public:
	virtual void Entered(GameWorld* pWorld);
	virtual void Update(GameWorld* pWorld, const long& lElapsedTicks);
	virtual void Exited(GameWorld* pWorld);
	virtual void MouseButtonDown(GameWorld* pWorld);

protected:
	PSOAgent* CreatePSOAgent(GameWorld* pWorld);
	Vehicle* CreatePSOTarget(GameWorld* pWorld);
	Vehicle* CreatePredator(GameWorld* pWorld);
	Obstacle* CreateObstacle(GameWorld* pWorld);

public:
	static const int PREY_1;
	static const int PSO_AGENT_1;

protected:
	Vehicle* m_predator;
	Vehicle* m_PSOTarget;
	std::string m_PSOAgent_model;
	unsigned int m_iPSOAgentCount;
	bool m_bScriptLoaded;

protected:
	void LoadScript(GameWorld* pWorld);
};

#define glState_PSO (*(GLState_PSO::Instance()))
#endif