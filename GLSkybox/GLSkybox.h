#ifndef _H_GL_SKY_BOX_H
#define _H_GL_SKY_BOX_H

#include "../GLObject/GLObject.h"
#include <string>

class GLSkybox : public GLObject
{
public:
	GLSkybox();
	virtual ~GLSkybox();

public:
	virtual void Update(const long& lElapsedTicks);
	virtual void Render();
	virtual void PreRender();
	virtual void PostRender();

public:
	std::string m_front_texture;
	std::string m_back_texture;
	std::string m_left_texture;
	std::string m_right_texture;
	std::string m_top_texture;
	std::string m_bottom_texture;

public:
	void LoadScript(const char* filename);

protected:
	GLuint m_display_lists;
	bool m_bDisplayList;
protected:
	void GenDisplayLists();

};
#endif