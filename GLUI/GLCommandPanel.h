#ifndef _H_GL_COMMAND_PANEL_H
#define _H_GL_COMMAND_PANEL_H

#include "GLLayout.h"
#include "GLUIObj.h"

class GLCommandPanel : public GLUIObj
{
public:
	GLCommandPanel(GLLayout* pLayout=NULL);
	virtual ~GLCommandPanel();

public:
	void SetLayout(GLLayout* pLayout);

public:
	virtual void PreRender();
	virtual void PostRender();
	virtual void Render();

protected:
	void RenderPanel();

public:
	virtual void MouseButtonDown(const int& iButton, const int& iX, const int& iY, const int& iXRel, const int& iYRel);
	virtual void MouseButtonUp(const int& iButton, const int& iX, const int& iY, const int& iXRel, const int& iYRel);
	virtual void MouseMoved(const int& iButton, const int& iX, const int& iY, const int& iXRel, const int& iYRel);

protected:
	virtual void ResetUI() { m_pLayout->ResetUI(); }

public:
	virtual void AddUIObj(GLUIObj* pObj);
	virtual void AddUIObj(int row, int col, GLUIObj* pObj); // for GLGridLayout
	virtual void AddUIObj(GLUIObj* pObj, int position); // for GLBorderLayout

public:
	virtual GLUIObj* GetUIObj(int row, int col) const;
	virtual GLUIObj* GetUIObj(int position) const;

public:
	virtual void RemoveUIObj(int row, int col); // for GLGridLayout
	virtual void RemoveUIObj(int position); //for GLBorderLayout
	virtual void ClearUIObjs(); 

public:
	virtual void EventNotified_MouseButtonDown(GLUIObj* pSender);
	virtual void EventNotified_MouseButtonUp(GLUIObj* pSender);
	virtual void EventNotified_MouseMoved(GLUIObj* pSender);

protected:
	GLLayout* m_pLayout;

protected:
	GLUIObj* m_pEventReceiver_MouseButtonDown;
	GLUIObj* m_pEventReceiver_MouseButtonUp;
	GLUIObj* m_pEventReceiver_MouseMoved;

public:
	GLUIObj* GetEventReceiver_MouseButtonDown() const { return m_pEventReceiver_MouseButtonDown; }
	GLUIObj* GetEventRecevier_MouseButtonUp() const { return m_pEventReceiver_MouseButtonUp; }
	GLUIObj* GetEventReceiver_MouseMoved() const { return m_pEventReceiver_MouseMoved; }

public:
	void ShowPanel(bool bShow) { m_bShowPanel=bShow; }

protected:
	bool m_bShowPanel;
};
#endif