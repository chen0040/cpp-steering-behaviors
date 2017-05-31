#ifndef _H_GL_LIGHT_SYSTEM_H
#define _H_GL_LIGHT_SYSTEM_H

#include "GLLight.h"
#include "../GLExtension/glxs.h"
#include <map>

class GLLightSystem
{
public:
	GLLightSystem();
	virtual ~GLLightSystem();

public:
	void SetLight(GLint id, GLLight* pLight);
	void ClearLights();
	bool NoLights() const;

public:
	virtual void Render();
	virtual void PreRender();
	virtual void PostRender();

private:
	std::map<GLint, GLLight*> m_lights;

};
#endif