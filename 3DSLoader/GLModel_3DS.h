#ifndef _H_3DS_MODEL_H
#define _H_3DS_MODEL_H

#include "../GLModel/GLModel.h"
#include "Model_3DS.h"
#include <string>

class GLModel_3DS : public GLModel
{
public:
	GLModel_3DS(const char* fname);
	virtual ~GLModel_3DS();

public:
	virtual void Render(int iFrame);

private:
	CModel3DS* m_loader;
	GLuint m_tex[32];

private:
	bool LoadModel(const char* fname);
};
#endif