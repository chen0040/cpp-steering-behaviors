#ifndef _H_GL_LAYOUT_H
#define _H_GL_LAYOUT_H

#include "../GLUI/GLUIObj.h"

class GLCommandPanel;

class GLLayout : public GLUIObj
{
public:
	GLLayout();
	virtual ~GLLayout();

public:
	void SetParent(GLCommandPanel* pParent);

public:
	virtual void AddUIObj(GLUIObj* pObj) { }
	virtual void AddUIObj(int row, int col, GLUIObj* pObj) { } // for GLGridLayout
	virtual void AddUIObj(GLUIObj* pObj, int position) { } // for GLBorderLayout

public:
	virtual GLUIObj* GetUIObj(int position) const { return NULL; }
	virtual GLUIObj* GetUIObj(int row, int height) const { return NULL; }

public:
	virtual void RemoveUIObj(int row, int col) { } // for GLGridLayout
	virtual void RemoveUIObj(int position) { } // for GLBorderLayout
	virtual void ClearUIObjs()=0; 

public:
	virtual void ResetUI()=0;

protected:
	GLCommandPanel* m_pParent;

public:
	virtual void Update(const long& lElapsedTicks) { }
};
#endif