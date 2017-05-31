#include "GLLight.h"

GLLight::GLLight(GLint _id)
: GLObject()
, id(_id)
{
	position=DEFAULT_LIGHT_POSITION;
	diffuse=GRAY_LIGHT;
	specular=WHITE_LIGHT;
	ambient=GRAY_LIGHT;
}

GLLight::GLLight()
: GLObject()
, id(GL_LIGHT0)
{
	position=DEFAULT_LIGHT_POSITION;
	diffuse=GRAY_LIGHT;
	specular=WHITE_LIGHT;
	ambient=GRAY_LIGHT;
}

void GLLight::PreRender()
{
#ifdef GL_OBJECT_STORE_GL_STATES
	GLObject::PreRender();
#endif
	glEnable(id);
}

void GLLight::PostRender()
{
	glDisable(id);
#ifdef GL_OBJECT_STORE_GL_STATES
	GLObject::PostRender();
#endif
}

void GLLight::Render()
{
	float diffuse_params[4];
	diffuse_params[0]=static_cast<float>(diffuse.x);
	diffuse_params[1]=static_cast<float>(diffuse.y);
	diffuse_params[2]=static_cast<float>(diffuse.z);
	diffuse_params[3]=1.0f;
	glLightfv(id, GL_DIFFUSE, diffuse_params);

	float ambient_params[4];
	ambient_params[0]=static_cast<float>(ambient.x);
	ambient_params[1]=static_cast<float>(ambient.y);
	ambient_params[2]=static_cast<float>(ambient.z);
	ambient_params[3]=1.0f;
	if(!(ambient.x==0 && ambient.y==0 && ambient.z==0))
	{
		glLightfv(id, GL_AMBIENT, ambient_params);
	}

	float specular_params[4];
	specular_params[0]=static_cast<float>(specular.x);
	specular_params[1]=static_cast<float>(specular.y);
	specular_params[2]=static_cast<float>(specular.z);
	specular_params[3]=1.0f;
	if(!(specular.x==0 && specular.y==0 && specular.z==0))
	{
		glLightfv(id, GL_SPECULAR, specular_params);
	}
}