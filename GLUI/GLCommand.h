#ifndef _H_GL_COMMAND_H
#define _H_GL_COMMAND_H

#include "GLUIObj.h"
#include "GLActionListener.h"
#include <string>

class GLCommand : public GLUIObj
{
public:
	GLCommand(GLTexture* pCommandTexture, GLTexture* pHoverTexure);
	virtual ~GLCommand();

public:
	virtual void Render();

public:
	virtual void MouseMoved(const int& iButton, const int& iX, const int& iY, const int& iXRel, const int& iYRel);
	virtual void MouseButtonDown(const int& iButton, const int& iX, const int& iY, const int& iXRel, const int& iYRel);
	virtual void MouseButtonUp(const int& iButton, const int& iX, const int& iY, const int& iXRel, const int& iYRel);

protected:
	virtual bool IsMouseEventCaptured(const int& iX, const int& iY);

protected:
	void SetMouseHoverTexture(GLTexture* pTexture) { m_pMouseHoverTexture=pTexture; }
	void SetCommandTexture(GLTexture* pTexture) { m_pCommandTexture=pTexture; }

public:
	void AddActionListener(GLActionListener* pListener) { m_listeners.push_back(pListener); }

protected:
	GLTexture* m_pCommandTexture;
	GLTexture* m_pMouseHoverTexture;

protected:
	GLTexture* m_pButtonTexture;

public:
	std::string m_text;
	double m_text_offset;
public:
	void SetText(std::string text, double text_offset=10) { m_text=text; m_text_offset=text_offset; }

protected:
	std::list<GLActionListener*> m_listeners;
};
#endif