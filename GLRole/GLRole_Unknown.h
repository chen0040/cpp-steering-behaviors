#ifndef _H_GL_ROLE_NON_UNKNOWN_H
#define _H_GL_ROLE_NON_UNKNOWN_H

#include "GLRole.h"

class GameEntity;

class GLRole_Unknown : public GLRole
{
public:
	GLRole_Unknown(GameEntity* pAgent);
	virtual ~GLRole_Unknown();

public:
	virtual void Update(const long& lElapsedTicks);
	virtual bool IsPenetratable(int TypeId) const { return false; }
};
#endif