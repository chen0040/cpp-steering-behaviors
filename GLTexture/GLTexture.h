#ifndef _H_GL_TEXTURE_H
#define _H_GL_TEXTURE_H

#include <sdl.h>
#include "../GLExtension/glxs.h"

class GLTexture
{
public:
	GLTexture(const char* filename);
	virtual ~GLTexture();

private:
	GLuint m_texture_id;

public:
	GLuint GetTextureId() const { return m_texture_id; }
};
#endif