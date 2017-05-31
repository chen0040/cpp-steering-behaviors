#ifndef _H_GL_GRID_LAYOUT_H
#define _H_GL_GRID_LAYOUT_H

#include "GLLayout.h"
#include <vector>

class GLGridLayout : public GLLayout
{
public:
	GLGridLayout(int rows, int cols);
	virtual ~GLGridLayout();

public:
	virtual void AddUIObj(int row, int col, GLUIObj* pObj);
	virtual void RemoveUIObj(int row, int col);
	virtual void ClearUIObjs();
	virtual GLUIObj* GetUIObj(int row, int col) const;
	
protected:
	virtual void ResetUI();

public:
	virtual void MouseMoved(const int& iButton, const int& iX, const int& iY, const int& iXRel, const int& iYRel);
	virtual void MouseButtonDown(const int& iButton, const int& iX, const int & iY, const int& iXRel, const int& iYRel);
	virtual void MouseButtonUp(const int& iButton, const int& iX, const int & iY, const int& iXRel, const int& iYRel);

public:
	virtual void Render();

public:
	std::vector<GLUIObj*> m_ui;
	int m_rows;
	int m_cols;
};
#endif