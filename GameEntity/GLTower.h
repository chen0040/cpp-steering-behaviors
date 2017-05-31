#ifndef _H_GL_TOWER_H
#define _H_GL_TOWER_H

#include "Obstacle.h"

class GLTower : public Obstacle
{
public:
	GLTower(GameWorld* pWorld);
	virtual ~GLTower();

public:
	virtual void Update(const long& ElapsedTicks);
	virtual void Render();
	virtual void PreRender();
	virtual void PostRender();
	virtual bool IsPenetratable(GameEntity* pAgent) const;

public:
	void SetCaptureId(int typeId) { m_capture_id=typeId; }

private:
	int m_capture_id;
	int m_has_captures;
	GLuint m_default_tower_display_list;
	bool m_bDefaultTowerDL;
};
#endif