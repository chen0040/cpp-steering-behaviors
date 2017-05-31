#ifndef _H_GAME_ENGINE_H
#define _H_GAME_ENGINE_H

//#define GL_MEMORY_LEAK_DETECTION

#ifdef GL_MEMORY_LEAK_DETECTION
#define VLD_MAX_DATA_DUMP 0
#define VLD_AGGREGATE_DUPLICATES 1

#include <vld.h>
#endif

#include <sdl.h>
#include <string>

class GameEngine
{
public:
	GameEngine();
	virtual ~GameEngine();

public:
	bool Init(int iWidth, int iHeight);
	bool Init();
	void Start();

protected:
	void DoThink();
	void DoRender();
	void HandleInput();

public:
	virtual bool SetVideoMode(const int& iWidth, const int& iHeight);
	virtual void AdditionalInit() { }

protected:
	virtual void Think(const long& lElapsedTicks) { }
	virtual void Render(SDL_Surface* pScreen) { }

protected:
	virtual void MouseMoved(const int& iButton, const int & iX, const int& iY, const int& iXRel, const int& iYRel) { }
	virtual void MouseButtonDown(const int& iButton, const int& iX, const int&iY, const int& iXRel, const int& iYRel) { }
	virtual void MouseButtonUp(const int& iButton, const int& iX, const int& iY, const int& iXRel, const int& iYRel) { }
	virtual void KeyDown(const int& iKeyEnum) { }
	virtual void KeyUp(const int& iKeyEnum) { }
	virtual void WindowActive() { }
	virtual void WindowInactive() { }

private:
	int m_iWidth;
	int m_iHeight;
public:
	int GetWidth() const { return m_iWidth; }
	int GetHeight() const { return m_iHeight; }
protected:
	void SetWidth(int iWidth) { m_iWidth=iWidth; }
	void SetHeight(int iHeight) { m_iHeight=iHeight; }

private:
	SDL_Surface* m_pScreen;
public:
	SDL_Surface* GetSurface() { return m_pScreen; }
	void SetSurface(SDL_Surface* pScreen) { m_pScreen=pScreen; }

private:
	long m_lLastTick;
	long m_lFPSTickCounter;
	long m_lFPSCounter;
	long m_lFPSCurrent;
public:
	long GetFPS() const { return m_lFPSCurrent; }

private:
	bool m_bQuit;
	bool m_bMinimized;
protected:
	virtual void Logout() { }

protected:
	std::string m_strTitle;
public:
	void SetTitle(const char* title) { m_strTitle=title; }
	const char* GetTitle() const { return m_strTitle.c_str(); }
};
#endif