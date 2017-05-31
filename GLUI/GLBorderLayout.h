#ifndef _H_GL_BORDER_LAYOUT_H
#define _H_GL_BORDER_LAYOUT_H

#include "GLLayout.h"
#include <vector>

class GLBorderLayout : public GLLayout
{
public:
	GLBorderLayout();
	virtual ~GLBorderLayout();

public:
	virtual void Render();

protected:
	virtual void ResetUI();

public:
	virtual void AddUIObj(GLUIObj* pObj, int position);
	virtual void RemoveUIObj(int position);
	virtual void ClearUIObjs();
	virtual GLUIObj* GetUIObj(int position) const;

public:
	virtual void MouseMoved(const int& iButton, const int& iX, const int& iY, const int& iXRel, const int& iYRel);
	virtual void MouseButtonDown(const int& iButton, const int& iX, const int & iY, const int& iXRel, const int& iYRel);
	virtual void MouseButtonUp(const int& iButton, const int& iX, const int & iY, const int& iXRel, const int& iYRel);

public:
	static const int SOUTH=0;
	static const int NORTH=1;
	static const int EAST=2;
	static const int WEST=3;
	static const int CENTER=4;

protected:
	double NORTH_PROP;
	double SOUTH_PROP;
	double WEST_PROP;
	double EAST_PROP;

protected:
	bool m_bStretchHorizontally;

public:
	void SetNorthProp(double prop) { NORTH_PROP=prop; }
	void SetSouthProp(double prop) { SOUTH_PROP=prop; }
	void SetWestProp(double prop) { WEST_PROP=prop; }
	void SetEastProp(double prop) { EAST_PROP=prop; }

public:
	void StretchHorizontally(bool flag) { m_bStretchHorizontally=flag; }

protected:
	static const int COUNT=5;

public:
	std::vector<GLUIObj*> m_ui;
};
#endif