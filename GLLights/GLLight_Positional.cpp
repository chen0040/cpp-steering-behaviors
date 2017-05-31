#include "GLLight_Positional.h"

GLLight_Positional::GLLight_Positional()
{

}

GLLight_Positional::~GLLight_Positional()
{

}

void GLLight_Positional::PreRender()
{
	GLLight::PreRender();
}

void GLLight_Positional::Render()
{
	GLLight::Render();
	
	float _position[4];
	_position[0]=static_cast<float>(position.x);
	_position[1]=static_cast<float>(position.y);
	_position[2]=static_cast<float>(position.z);
	_position[3]=1.0f;

	glLightfv(id, GL_POSITION, _position);

}

void GLLight_Positional::PostRender()
{
	GLLight::PostRender();
}

void GLLight_Positional::Update(const long &lElapsedTicks)
{

}