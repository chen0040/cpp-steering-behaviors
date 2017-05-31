#ifndef _H_GL_PACKET_H
#define _H_GL_PACKET_H

#include "GLParticle.h"

class GLPacket : public GLParticle
{
public:
	GLPacket() : GLParticle(NULL) { }
	virtual ~GLPacket() { }
public:
	virtual void Render();

};
#endif