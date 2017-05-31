#ifndef _H_GL_LIGHT_POSITIONAL_H
#define _H_GL_LIGHT_POSITIONAL_H

#include "GLLight.h"

class GLLight_Positional : public GLLight
{
public:
	GLLight_Positional();
	virtual ~GLLight_Positional();

public:
	virtual void Update(const long& lElapsedTicks);
	virtual void Render();
	virtual void PreRender();
	virtual void PostRender();

protected:
	GLVector m_position;
};
#endif