#ifndef _H_GL_LIGHT_DIRECTIONAL_H
#define _H_GL_LIGHT_DIRECTIONAL_H

#include "GLLight.h"

class GLLight_Directional : public GLLight
{
public:
	GLLight_Directional();
	virtual ~GLLight_Directional();

public:
	virtual void Update(const long& lElapsedTicks);
	virtual void Render();
	virtual void PreRender();
	virtual void PostRender();

protected:
	GLVector m_position;
};
#endif