#ifndef _H_GL_STATE_AVOIDANCE_H
#define _H_GL_STATE_AVOIDANCE_H

#include "GLState.h"

class Obstacle;
class Vehicle; 

class GLState_Avoidance : public GLState
{
public:
	virtual ~GLState_Avoidance() { }
	static GLState_Avoidance* Instance();

private:
	GLState_Avoidance();
	GLState_Avoidance(const GLState_Avoidance& rhs) { }
	GLState_Avoidance& operator=(const GLState_Avoidance& rhs) { return *this; }

public:
	virtual void Entered(GameWorld* pWorld);
	virtual void Exited(GameWorld* pWorld);
	virtual void Update(GameWorld* pWorld, const long& lElapsedTicks);

protected:
	Obstacle* CreateObstacle(GameWorld* pWorld);
	Vehicle* CreateAgent(GameWorld* pWorld);
	Vehicle* CreatePredator(GameWorld* pWorld);

public:
	virtual void MouseButtonDown(GameWorld* pWorld);

public:
	static const int PREDATOR=1;
	static const int AGENT=2;

protected:
	bool m_bScriptLoaded;
	std::string m_Tower_Model;
	unsigned int m_iObsCount;
	unsigned int m_iPreyCount;

protected:
	void LoadScript(GameWorld* pWorld);
};

#define glState_Avoidance (*(GLState_Avoidance::Instance()))
#endif