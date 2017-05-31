#ifndef _H_GL_UI_OBJ_H
#define _H_GL_UI_OBJ_H

#include <list>
#include "../GLTexture/GLTexture.h"
#include "../GLShape/GLRect.h"

class GLUIObj
{
public:
	GLUIObj();
	virtual ~GLUIObj();

public:
	virtual void Render()=0;
	virtual void PreRender() { }
	virtual void PostRender() { }
	virtual void Update(const long& lElapsedTicks) { }

public:
	virtual void MouseMoved(const int& iButton, const int& iX, const int& iY, const int& iXRel, const int& iYRel) { }
	virtual void MouseButtonDown(const int& iButton, const int& iX, const int& iY, const int& iXRel, const int& iYRel) { }
	virtual void MouseButtonUp(const int& iButton, const int& iX, const int& iY, const int& iXRel, const int& iYRel) { }

public:
	virtual void EventNotified_MouseButtonDown(GLUIObj* pSender);
	virtual void EventNotified_MouseButtonUp(GLUIObj* pSender);
	virtual void EventNotified_MouseMoved(GLUIObj* pSender);

public:
	void AddObserver(GLUIObj* pObserver);

protected:
	bool m_bVisible;
public:
	void SetVisible(bool bVisible) { m_bVisible=bVisible; }

protected:
	std::list<GLUIObj*> m_obs;

protected:
	GLRect m_bounding_rectangle;

public:
	GLRect GetInsettedRect() const
	{
		GLRect rect=m_bounding_rectangle;

		rect.left+=m_inset.left;
		rect.right-=m_inset.right;
		rect.top+=m_inset.top;
		rect.bottom-=m_inset.bottom;

		return rect;
	}

	GLRect GetBoundingRect() const
	{
		return m_bounding_rectangle;
	}

	void SetBoundingRect(double left, double right, double top, double bottom)
	{
		m_bounding_rectangle.left=left;
		m_bounding_rectangle.right=right;
		m_bounding_rectangle.top=top;
		m_bounding_rectangle.bottom=bottom;
		ResetUI();
	}

	void SetSize(double width, double height);
	void SetWidth(double width);
	void SetHeight(double height);

protected:
	virtual void ResetUI() { }

protected:
	GLRect m_inset;

public:
	GLRect GetInset() const { return m_inset; }
	void SetInset(const GLRect& inset) { m_inset=inset; }
};
#endif