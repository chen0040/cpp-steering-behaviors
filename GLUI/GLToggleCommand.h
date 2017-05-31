#ifndef _H_GL_TOGGLE_COMMAND_H
#define _H_GL_TOGGLE_COMMAND_H

#include "GLUIObj.h"
#include "GLActionListener.h"
#include <string>

class GLToggleCommand : public GLUIObj
{
public:
	GLToggleCommand(GLTexture* pCommandTextureOn, GLTexture* pHoverTexureOn, GLTexture* pCommandTextureOff, GLTexture* pHoverTexureOff);
	virtual ~GLToggleCommand();

public:
	virtual void Render();

public:
	virtual void MouseMoved(const int& iButton, const int& iX, const int& iY, const int& iXRel, const int& iYRel);
	virtual void MouseButtonDown(const int& iButton, const int& iX, const int& iY, const int& iXRel, const int& iYRel);
	virtual void MouseButtonUp(const int& iButton, const int& iX, const int& iY, const int& iXRel, const int& iYRel);
	virtual void ToggleOn(bool bOn) { m_bOn=bOn; }

protected:
	virtual bool IsMouseEventCaptured(const int& iX, const int& iY);

public:
	void AddActionListener(GLActionListener* pListener) { m_listeners.push_back(pListener); }

protected:
	GLTexture* m_pCommandTextureOn;
	GLTexture* m_pMouseHoverTextureOn;
	GLTexture* m_pCommandTextureOff;
	GLTexture* m_pMouseHoverTextureOff;

protected:
	GLTexture* m_pButtonTexture;
	bool m_bOn;

protected:
	std::list<GLActionListener*> m_listeners;
};
#endif