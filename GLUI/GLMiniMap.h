#ifndef _H_GL_MINI_MAP_H
#define _H_GL_MINI_MAP_H

#include "GLUIObj.h"
#include "GLActionListener.h"
#include <string>

class GLSphericalCamera;
class GameWorld;

class GLMiniMap : public GLUIObj
{
public:
	GLMiniMap();
	virtual ~GLMiniMap();

public:
	virtual void Render();

public:
	virtual void MouseMoved(const int& iButton, const int& iX, const int& iY, const int& iXRel, const int& iYRel);
	virtual void MouseButtonDown(const int& iButton, const int& iX, const int& iY, const int& iXRel, const int& iYRel);
	virtual void MouseButtonUp(const int& iButton, const int& iX, const int& iY, const int& iXRel, const int& iYRel);

protected:
	virtual bool IsMouseEventCaptured(const int& iX, const int& iY);

protected:
	std::list<GLActionListener*> m_listeners;

protected:
	float m_alpha;
	GLSphericalCamera* m_pCamera;
	GameWorld* m_pWorld;
	
public:
	void SetCamera(GLSphericalCamera* pCamera) { m_pCamera=pCamera; }
	void SetWorld(GameWorld* pWorld) { m_pWorld=pWorld; }
};
#endif