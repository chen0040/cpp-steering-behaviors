#ifndef _H_GL_ROLE_H
#define _H_GL_ROLE_H

class GameEntity;

class GLRole
{
public:
	GLRole(GameEntity* pAgent);
	virtual ~GLRole();

public:
	virtual void Update(const long& lElapsedTicks)=0;
	virtual bool IsPenetratable(int TypeId) const=0;

protected:
	GameEntity* m_pAgent;
};

#endif