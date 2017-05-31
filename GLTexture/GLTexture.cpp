#include "GLTexture.h"
#include "../GLUtil/GLExitSystem.h"
#include <string>
#include <sstream>
#include <sdl_image.h>
#include <gl/glu.h>

GLTexture::GLTexture(const char* fname)
{
	//Xianshun says:
	//SDL_DisplayFormatAlpha is only efficient if the pSurface is to be used to blit to the video surface
	//in this case, the pSurface is only used to load the data, so this function is not particularly useful
	/*
	SDL_Surface* pRaw=IMG_Load(fname);
	SDL_Surface* pSurface=SDL_DisplayFormatAlpha(pRaw);
	SDL_FreeSurface(pRaw);
	*/
	SDL_Surface* pSurface=IMG_Load(fname);

	
	// SDL_SetColorKey(pSurface, SDL_SRCCOLORKEY, SDL_MapRGB(pSurface->format, 255, 255, 255));
	if(pSurface==NULL)
	{
		std::ostringstream oss;
		oss << "Failed to load texture image" << fname;
		GLExitSystem("GLTexture::GLTexture(const char* fname)", oss.str().c_str());
	}

	GLenum format=GL_RGBA;
	GLint nColor=pSurface->format->BytesPerPixel;
	if(nColor==4)
	{
		if(pSurface->format->Rmask & 0x000000ff)
		{
			format=GL_RGBA;
		}
		else
		{
			format=GL_BGRA;
		}
	}
	else if(nColor==3)
	{
		if(pSurface->format->Rmask & 0x0000ff)
		{
			format=GL_RGB;
		}
		else
		{
			format=GL_BGR;
		}
	}
	else
	{
		GLExitSystem("GLTexture::GLTexture(const char* fname)", "Unrecognize image format");
	}

	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &m_texture_id);

	glBindTexture(GL_TEXTURE_2D, m_texture_id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Xianshun says:
	//gluScaleImage() to scale image to have width of 2^n and height of 2^m. for opengl prior to 2.0, the texture dimension is required to have this format of power of 2

	glTexImage2D(GL_TEXTURE_2D, 0, nColor, pSurface->w, pSurface->h, 0, format, GL_UNSIGNED_BYTE, pSurface->pixels);

	SDL_FreeSurface(pSurface);
}

GLTexture::~GLTexture()
{
	glDeleteTextures(1, &m_texture_id);
}
