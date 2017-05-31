#ifndef _H_MD2_MODEL_MANAGER_H
#define _H_MD2_MODEL_MANAGER_H

#include "GLModel.h"
#include <string>
#include <map>

class GLModelManager
{
public:
	virtual ~GLModelManager();
	static GLModelManager* Instance();

public:
	GLModelManager();
	GLModelManager(const GLModelManager& rhs) { }
	GLModelManager& operator=(const GLModelManager& rhs) { return *this; }

public:
	GLModel* GetModel(const char* fname);

public:
	GLModel* CreateModel(const char* fname);

protected:
	std::map<std::string, GLModel*> m_models;

protected:
	void free_memory();
};

#define glModels (*(GLModelManager::Instance()))

#endif