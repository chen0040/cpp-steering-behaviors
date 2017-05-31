#ifndef _H_GL_LIGHT_H
#define _H_GL_LIGHT_H

#include "../GLObject/GLObject.h"
#include "../GLExtension/glxs.h"
#include "../GLVector/GLVector.h"

#define RED_LIGTHT GLVector(1.0, 0.0, 0.0)
#define GREEN_LIGHT GLVector(0.0, 1.0, 0.0)
#define BLUE_LIGHT GLVector(0.0, 0.0, 1.0)
#define WHITE_LIGHT GLVector(1.0, 1.0, 1.0)
#define GRAY_LIGHT GLVector(0.3, 0.3, 0.3)

#define DEFAULT_LIGHT_POSITION GLVector(-2.0, 2.0, 2.0)

class GLLight : GLObject
{
public:
	GLLight(GLint _id);
	GLLight();
	virtual ~GLLight() { }

public:
	virtual void PreRender();
	virtual void PostRender();
	virtual void Render();

public:
	GLint id;
	GLVector position;
	GLVector diffuse;
	GLVector ambient;
	GLVector specular;
};
#endif