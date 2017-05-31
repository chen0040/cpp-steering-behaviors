#ifndef _H_GL_STATE_GAME_MENU_H
#define _H_GL_STATE_GAME_MENU_H

#include "GLState.h"

class GLCommandPanel;

class GLState_GameMenu : public GLState
{
public:
	static GLState_GameMenu* Instance();
	virtual ~GLState_GameMenu();

private:
	GLState_GameMenu();
	GLState_GameMenu(const GLState_GameMenu& rhs) { }
	GLState_GameMenu& operator=(const GLState_GameMenu& rhs) { return *this; }

public:
	virtual void Entered(GameWorld* pWorld);
	virtual void Exited(GameWorld* pWorld);
	virtual void Update(GameWorld* pWorld, const long& lElapsedTicks);
	virtual void Render(GameWorld* pWorld);

public:
	virtual void MouseButtonDown(GameWorld* pWorld);
	virtual void MouseMoved(GameWorld* pWorld);

protected:
	GLCommandPanel* m_pGUI;
};

#define glState_GameMenu (*(GLState_GameMenu::Instance()))
#endif