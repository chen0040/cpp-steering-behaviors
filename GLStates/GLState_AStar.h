#ifndef _H_GL_STATE_A_STAR_H
#define _H_GL_STATE_A_STAR_H

#include "GLState.h"

class GameWorld;
class Vehicle;
class Obstacle;

class GLState_AStar : public GLState
{
private:
	GLState_AStar();
	GLState_AStar(const GLState_AStar& rhs) { }
	GLState_AStar& operator=(const GLState_AStar& rhs) { return *this; }

public:
	static GLState_AStar* Instance();
	virtual ~GLState_AStar();

public:
	virtual void Entered(GameWorld* pWorld);
	virtual void Update(GameWorld* pWorld, const long& lElapsedTicks);
	virtual void Exited(GameWorld* pWorld);
	virtual void MouseButtonDown(GameWorld* pWorld);

protected:
	Vehicle* CreateAirCraftPrey(GameWorld* pWorld);
	Vehicle* CreateAirCraftPredator(GameWorld* pWorld);
	Vehicle* CreateLandCraftPrey(GameWorld* pWorld);
	Vehicle* CreateLandCraftPredator(GameWorld* pWorld);
	Obstacle* CreateTree(GameWorld* pWorld);

public:
	static const int AIR_FLOCK=1;
	static const int AIR_PREDATOR=2;

	static const int LAND_FLOCK=3;
	static const int LAND_PREDATOR=4;

protected:
	bool m_bScriptLoaded;
	unsigned int m_iAirCraftPreyCount;
	unsigned int m_iLandCraftPreyCount;
	unsigned int m_iTreeCount;
	std::string m_AirCraftPreyModel;

protected:
	void LoadScript(GameWorld* pWorld);
};

#define glState_AStar (*(GLState_AStar::Instance()))
#endif