#ifndef _H_MD2_MODEL_H
#define _H_MD2_MODEL_H

#include "../GLModel/GLModel.h"
#include "MD2Loader.h"
#include <string>

class GLModel_MD2 : public GLModel
{
public:
	GLModel_MD2(const char* fname);
	virtual ~GLModel_MD2();

public:
	virtual void Render(int iFrame);

private:
	MD2Obj m_loader;
	GLuint m_tex[32];

protected:
	void SetCurrentFrame(int iFrame);
	int GetCurrentFrame() const { return m_iCurrentFrame; }
	int m_iCurrentFrame;

private:
	bool LoadModel(const char* fname);
};
#endif