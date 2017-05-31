#include "GLTextureManager.h"

GLTextureManager::~GLTextureManager()
{
	std::map<std::string, GLTexture*>::iterator pos;
	for(pos=m_textures.begin(); pos!=m_textures.end(); ++pos)
	{
		delete pos->second;
	}
}

GLTextureManager* GLTextureManager::Instance()
{
	static GLTextureManager theInstance;
	return &theInstance;
}

GLTexture* GLTextureManager::GetTexture(const char *fname)
{
	std::string texture_name=fname;

	std::map<std::string, GLTexture*>::iterator pos=m_textures.find(texture_name);
	if(pos != m_textures.end())
	{
		return pos->second;
	}

	GLTexture* pTexture=new GLTexture(fname);
	m_textures[texture_name]=pTexture;

	return pTexture;

}