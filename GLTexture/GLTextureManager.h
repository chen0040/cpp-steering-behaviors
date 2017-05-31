#ifndef _H_GL_TEXTURE_MANAGER_H
#define _H_GL_TEXTURE_MANAGER_H

#include "GLTexture.h"
#include <map>
#include <string>

class GLTextureManager
{
public:
	virtual ~GLTextureManager();
	static GLTextureManager* Instance();

private:
	GLTextureManager() { }
	GLTextureManager(const GLTextureManager& rhs) { }
	GLTextureManager& operator=(const GLTextureManager& rhs) { return *this; }

public:
	GLTexture* GetTexture(const char* fname);

private:
	std::map<std::string, GLTexture*> m_textures;
};

#define glTextures (*(GLTextureManager::Instance()))
#endif