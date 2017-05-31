#include "GLLight_Directional.h"

GLLight_Directional::GLLight_Directional()
{

}

GLLight_Directional::~GLLight_Directional()
{

}

void GLLight_Directional::PreRender()
{
	GLLight::PreRender();
}

void GLLight_Directional::Render()
{
	GLLight::Render();

	position=position.normalize();
	
	float _position[4];
	_position[0]=static_cast<float>(position.x);
	_position[1]=static_cast<float>(position.y);
	_position[2]=static_cast<float>(position.z);
	_position[3]=0.0f;

	glLightfv(id, GL_POSITION, _position);

}

void GLLight_Directional::PostRender()
{
	GLLight::PostRender();
}

void GLLight_Directional::Update(const long &lElapsedTicks)
{

}