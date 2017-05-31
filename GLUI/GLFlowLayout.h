#ifndef _H_GL_FLOW_LAYOUT_H
#define _H_GL_FLOW_LAYOUT_H

#include "GLLayout.h"
#include <vector>

class GLFlowLayout : public GLLayout
{
public:
	GLFlowLayout(int alignment=HORIZONTAL_ALIGNMENT);
	virtual ~GLFlowLayout();

public:
	virtual void Render();

protected:
	virtual void ResetUI();
	virtual void ClearUIObjs();

public:
	virtual void AddUIObj(GLUIObj* pObj);

public:
	virtual void MouseMoved(const int& iButton, const int& iX, const int& iY, const int& iXRel, const int& iYRel);
	virtual void MouseButtonDown(const int& iButton, const int& iX, const int & iY, const int& iXRel, const int& iYRel);
	virtual void MouseButtonUp(const int& iButton, const int& iX, const int & iY, const int& iXRel, const int& iYRel);

public:
	static const int HORIZONTAL_ALIGNMENT=0;
	static const int VERTICAL_ALIGNMENT=1;

protected:
	int m_alignment;
public:
	void SetAlignment(int alignment) { m_alignment=alignment; }

public:
	std::vector<GLUIObj*> m_ui;
};
#endif