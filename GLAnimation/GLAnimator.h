#ifndef _H_GL_ANIMATOR_H
#define _H_GL_ANIMATOR_H

#include "../GLObject/GLObject.h"
#include "../GLModel/GLModel.h"

class GLAnimator : public GLObject
{
public:
	GLAnimator()
		: GLObject()
		, m_pModel(NULL)
		, m_iFPSTickCounter(0)
		, m_iCurrentFrame(0)
		, m_bStop(true)
		, m_bSoundEnabled(true)
	{
		this->SetFPS(24);
		this->SetMaxFPS(48);
	}

	virtual ~GLAnimator() { }

public:
	virtual void Update(const long& lElapsedTicks)
	{
		if(m_bStop)
		{
			return;
		}

		m_pModel->EnableSound(m_bSoundEnabled);

		if(m_pModel->GetFrameCount() > 0)
		{
			m_iFPSTickCounter+=lElapsedTicks;
			int iOldFrame=m_iCurrentFrame;
			if(m_iFPSTickCounter >= m_iTicksPerFrame)
			{
				m_iCurrentFrame=(m_iCurrentFrame + 1) % m_iFrameCount;
				m_iFPSTickCounter=0;
			}

			if(m_bSoundEnabled)
			{
				if(iOldFrame != m_iCurrentFrame)
				{
					m_pModel->PlaySound(m_iCurrentFrame);
				}
			}
		}
	}

	void SetFPS(int frames)
	{
		m_iFPS=frames;
		if(m_iFPS==0)
		{
			m_bStop=true;
			m_iCurrentFrame=0;
		}
		else
		{
			m_bStop=false;
			m_iTicksPerFrame=1000 / frames;
		}
	}
	void SetMaxFPS(int frames)
	{
		m_iMaxFPS=frames;
	}
	void ResetAnimation()
	{
		m_iCurrentFrame=0;
		m_iFPSTickCounter=0;
	}

	int GetFPS() const { return m_iFPS; }
	int GetMaxFPS() const { return m_iMaxFPS; }

	virtual void Render()
	{
		m_pModel->PreRender();
		m_pModel->Render(m_iCurrentFrame);
		m_pModel->PostRender();
	}

	virtual void PreRender()
	{
#ifdef GL_OBJECT_STORE_GL_STATES
		GLObject::PreRender();
#endif
	}

	virtual void PostRender()
	{
#ifdef GL_OBJECT_STORE_GL_STATES
		GLObject::PostRender();
#endif
	}
	void SetModel(GLModel* pModel)
	{
		m_pModel=pModel;
		m_pModel->EnableSound(m_bSoundEnabled);
		m_iFrameCount=pModel->GetFrameCount();
	}
	GLModel* GetModel() const { return m_pModel; }

protected:
	GLModel* m_pModel;
	int m_iFPSTickCounter;
	int m_iFPS;
	int m_iTicksPerFrame;
	int m_iCurrentFrame;
	int m_iFrameCount;
	int m_iMaxFPS;
	bool m_bStop;

public:
	void EnableSound(bool bEnabled) 
	{ 
		m_bSoundEnabled=bEnabled; 
	}
protected:
	bool m_bSoundEnabled;
};
#endif