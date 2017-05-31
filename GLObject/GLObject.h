#ifndef _H_GL_OBJECT_H
#define _H_GL_OBJECT_H

#include <sdl.h>
#include "../GLExtension/glxs.h"

//#define GL_OBJECT_STORE_GL_STATES

class GLObject
{
public:
	GLObject();
	virtual ~GLObject();

public:
	virtual void Update(const long& lElapsedTicks)=0;
	virtual void Render() { }

public:
#ifdef GL_OBJECT_STORE_GL_STATES
	virtual void PreRender();
	virtual void PostRender();
#else
	virtual void PreRender()=0;
	virtual void PostRender()=0;
#endif

#ifdef GL_OBJECT_STORE_GL_STATES
private:
	GLboolean m_GL_DEPTH_TEST;
	GLboolean m_GL_FOG;
	GLboolean m_GL_LIGHTING;

	GLboolean m_GL_TEXTURE_2D;
	GLboolean m_GL_COLOR_MATERIAL;
#endif

};
#endif