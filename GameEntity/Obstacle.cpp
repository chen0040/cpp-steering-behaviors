#include "Obstacle.h"
#include "../GameWorld/GameWorld.h"
#include <glut.h>
#include "../GLMath/GLMath.h"
#include "../GLModel/GLModelManager.h"
#include <cmath>
#include <iostream>
//#include "../GLTexture/GLTextureManager.h"

Obstacle::Obstacle(GameWorld* pWorld)
: GameEntity(pWorld)
, m_pModel(NULL)
{
	this->ShowDot(false);
}

void Obstacle::Render()
{
	// auxSolidCylinder(m_bounding_radius, 10);
	
	if(m_pModel == NULL)
	{
		if(is_tagged())
		{
			glTranslated(0.0, 3.0, 0.0);
			glScaled(1.0, 6.0, 1.0);
		}
		else
		{
			glTranslated(0.0, 1.5, 0.0);
			glScaled(1.0, 3.0, 1.0);
		}

		double side=m_bounding_radius * sin(glMath.PI() / 4);

		glDisable(GL_CULL_FACE);
		glutSolidCube(side);
		glEnable(GL_CULL_FACE);
		/*
		GLTexture* pTexture=glTextures.GetTexture("Cube.bmp");
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, pTexture->GetTextureId());
		glutSolidCube(side);
		glDisable(GL_TEXTURE_2D);
		*/
	}
	else
	{
		//std::cout << "PreRender: " << m_pModel->toString() << std::endl;
		m_pModel->PreRender();
		//std::cout << "Render: " << m_pModel->toString() << std::endl;
		m_pModel->Render(0);
		//std::cout << "PostRender: " << m_pModel->toString() << std::endl;
		m_pModel->PostRender();
	}
}

void Obstacle::SetModel(const char* model_name)
{
	//std::cout << "start" << model_name << std::endl;
	m_pModel=glModels.GetModel(model_name);
	//std::cout << "suc" << model_name << std::endl;
}