#include "GameEngine.h"
#include "../GLUtil/GLExitSystem.h"
#include <windows.h>

GameEngine::GameEngine()
: m_lLastTick(0)
, m_lFPSTickCounter(0)
, m_lFPSCounter(0)
, m_lFPSCurrent(0)
, m_bQuit(true)
, m_bMinimized(false)
, m_pScreen(NULL)
, m_iWidth(640)
, m_iHeight(480)
{
	m_strTitle="XSGE GameEngine";
}

GameEngine::~GameEngine()
{

}

bool GameEngine::Init(int iWidth, int iHeight)
{
	atexit(SDL_Quit);

	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
	{
		GLExitSystem("GameEngine::Init(int iWidth, int iHeight)", "SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0");
		return false;
	}

	m_iWidth=iWidth;
	m_iHeight=iHeight;
	if(SetVideoMode(iWidth, iHeight)==false)
	{
		GLExitSystem("GameEngine::Init(int iWidth, int iHeight)", "SetVideoMode(iWidth, iHeight) return false");
		return false;
	}

	AdditionalInit();

	SDL_WM_SetCaption(m_strTitle.c_str(), NULL);

	return true;
}

bool GameEngine::Init()
{
	return Init(m_iWidth, m_iHeight);
}

bool GameEngine::SetVideoMode(const int& iWidth, const int& iHeight)
{
	m_pScreen=SDL_SetVideoMode(iWidth, iHeight, 0, SDL_SWSURFACE | SDL_DOUBLEBUF);

	if(m_pScreen==NULL)
	{
		return false;
	}

	return true;
}

void GameEngine::Start()
{
	m_bQuit=false;
	m_lLastTick=SDL_GetTicks();

	while(!m_bQuit)
	{
		HandleInput();
		if(m_bMinimized)
		{
			WaitMessage();
		}
		else
		{
			DoThink();
			DoRender();
		}
	}
}

void GameEngine::DoThink()
{
	long lElapsedTicks=SDL_GetTicks() - m_lLastTick;
	m_lLastTick=SDL_GetTicks();

	Think(lElapsedTicks);

	m_lFPSTickCounter+=lElapsedTicks;
}

void GameEngine::DoRender()
{
	m_lFPSCounter++;
	if(m_lFPSTickCounter >= 1000)
	{
		m_lFPSCurrent=m_lFPSCounter;
		m_lFPSCounter=0;
		m_lFPSTickCounter=0;
	}

	if(SDL_MUSTLOCK(m_pScreen))
	{
		if(SDL_LockSurface(m_pScreen) < 0)
		{
			return;
		}
	}

	Render(m_pScreen);

	SDL_Flip(m_pScreen);

	if(SDL_MUSTLOCK(m_pScreen))
	{
		SDL_UnlockSurface(m_pScreen);
	}
}

void GameEngine::HandleInput()
{
	SDL_Event event;
	while(SDL_PollEvent(&event))
	{
		switch(event.type)
		{
		case SDL_MOUSEMOTION:
			MouseMoved(event.button.button, event.motion.x, event.motion.y, event.motion.xrel, event.motion.yrel);
			break;
		case SDL_MOUSEBUTTONDOWN:
			MouseButtonDown(event.button.button, event.motion.x, event.motion.y, event.motion.xrel, event.motion.yrel);
			break;
		case SDL_MOUSEBUTTONUP:
			MouseButtonUp(event.button.button, event.motion.x, event.motion.y, event.motion.xrel, event.motion.yrel);
			break;
		case SDL_KEYDOWN:
			if(event.key.keysym.sym == SDLK_ESCAPE)
			{
				m_bQuit=true;
				std::exit(0);
				break;
			}
			KeyDown(event.key.keysym.sym);
			break;
		case SDL_KEYUP:
			KeyUp(event.key.keysym.sym);
			break;
		case SDL_QUIT:
			m_bQuit=true;
			std::exit(0);
			break;
		case SDL_ACTIVEEVENT:
			if(event.active.state & SDL_APPACTIVE)
			{
				if(event.active.gain)
				{
					m_bMinimized=false;
					WindowActive();
				}
				else
				{
					m_bMinimized=true;
					WindowInactive();
				}
			}
			break;
		}
	}
}
