#ifndef _H_OBSTACLE_H
#define _H_OBSTACLE_H

#include "GameEntity.h"
#include "../GLModel/GLModel.h"

class Obstacle : public GameEntity
{
public:
	Obstacle(GameWorld* pWorld);
	virtual ~Obstacle() { }

public:
	virtual void Render();
	virtual void Update(const long& lElapsedTicks) { }

public:
	void SetModel(const char* model_name);

protected:
	GLModel* m_pModel;
};

#endif 