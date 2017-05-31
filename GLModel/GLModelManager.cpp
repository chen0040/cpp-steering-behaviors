#include "GLModelManager.h"
#include "../MD2Loader/GLModel_MD2.h"
#include "../3DSLoader/GLModel_3DS.h"
#include <iostream>
#include <algorithm>
#include <cctype>
#include "../GLUtil/GLExitSystem.h"

GLModelManager::GLModelManager()
{

}

GLModelManager::~GLModelManager()
{
	free_memory();
}

void GLModelManager::free_memory()
{
	std::map<std::string, GLModel*>::iterator pos;
	for(pos=m_models.begin(); pos!=m_models.end(); ++pos)
	{
		delete pos->second;
	}
	m_models.clear();
}

GLModelManager* GLModelManager::Instance()
{
	static GLModelManager theInstance;
	return &theInstance;
}

GLModel* GLModelManager::GetModel(const char* fname)
{
	std::string model_file(fname);

	std::map<std::string, GLModel*>::iterator pos=m_models.find(model_file);
	if(pos != m_models.end())
	{
		return pos->second;
	}

	GLModel* pModel=CreateModel(fname);
	m_models[model_file]=pModel;

	return pModel;
}

GLModel* GLModelManager::CreateModel(const char* fname)
{
	std::string model_name=fname;
	size_t dot_pos=model_name.find_last_of(".");

	if(dot_pos==std::string::npos)
	{
		GLExitSystem("GLModelManager::CreateModel(const char* fname)", "Failed to get the file extension");
	}

	std::string ext=model_name.substr(dot_pos+1);
	
	std::transform(ext.begin(), ext.end(), ext.begin(), std::tolower);

	if(ext.compare("md2")==0)
	{
		return new GLModel_MD2(fname);
	}
	else if(ext.compare("3ds")==0)
	{
		//std::cout << "start: " << fname << std::endl;
		return new GLModel_3DS(fname);
		//std::cout << "end: " << fname << std::endl;
	}
	else
	{
		std::cerr << "ext: " << ext << std::endl;
		GLExitSystem("GLModelManager::CreateModel(const char* fname)", "Failed to interpret the file extension");
	}

	return NULL;
}

