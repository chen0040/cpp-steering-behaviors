#include "GLLightSystem.h"

GLLightSystem::GLLightSystem()
{
	
}

GLLightSystem::~GLLightSystem()
{
	ClearLights();
}

void GLLightSystem::SetLight(GLint id, GLLight* pLight)
{
	std::map<GLint, GLLight*>::iterator light_pos=m_lights.find(id);
	if(light_pos==m_lights.end())
	{
		m_lights[id]=pLight;
	}
	else
	{
		if(light_pos->second != NULL)
		{
			delete light_pos->second;
		}
		light_pos->second=pLight;
		if(pLight != NULL)
		{
			pLight->id=id;
		}
	}
}

void GLLightSystem::ClearLights()
{
	std::map<GLint, GLLight*>::iterator pos;
	for(pos=m_lights.begin(); pos!=m_lights.end(); ++pos)
	{
		if(pos->second != NULL)
		{
			delete pos->second;
			pos->second=NULL;
		}
	}
	m_lights.clear();
}

bool GLLightSystem::NoLights() const
{
	std::map<GLint, GLLight*>::const_iterator pos;
	for(pos=m_lights.begin(); pos!=m_lights.end(); ++pos)
	{
		if(pos->second != NULL)
		{
			return false;
		}
	}

	return true;
}


void GLLightSystem::Render()
{
	if(NoLights())
	{
		return;
	}

	std::map<GLint, GLLight*>::iterator pos;
	for(pos=m_lights.begin(); pos!=m_lights.end(); ++pos)
	{
		if(pos->second != NULL)
		{
			pos->second->Render();
		}
	}
}

void GLLightSystem::PreRender()
{
	if(NoLights())
	{
		return;
	}

	glEnable(GL_LIGHTING);

	std::map<GLint, GLLight*>::iterator pos;
	for(pos=m_lights.begin(); pos!=m_lights.end(); ++pos)
	{
		if(pos->second != NULL)
		{
			pos->second->PreRender();
		}
	}
}

void GLLightSystem::PostRender()
{
	if(NoLights())
	{
		return;
	}

	std::map<GLint, GLLight*>::iterator pos;
	for(pos=m_lights.begin(); pos!=m_lights.end(); ++pos)
	{
		if(pos->second != NULL)
		{
			pos->second->PostRender();
		}
	}
}