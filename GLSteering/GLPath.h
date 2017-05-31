#ifndef _H_GL_PATH_H
#define _H_GL_PATH_H

#include "../GLObject/GLObject.h"
#include "../GLVector/GLVector.h"

class GLPathNode
{
public:
	GLPathNode();
	virtual ~GLPathNode();

	GLPathNode* m_prev;
	GLPathNode* m_next;
	GLVector m_position;
};

class GLPath : public GLObject
{
public:
	GLPath();
	virtual ~GLPath();

public:
	virtual void Render();
	virtual void PreRender();
	virtual void PostRender();
	virtual void Update(const long& lElapsedTicks)  { }

public:
	void RandomCreate(int number_points, const GLVector& min_point, const GLVector& max_point);
	void AddPoint(const GLVector& position);

public:
	GLVector GetCurrent() const;

protected:
	bool m_bLoop;
public:
	void EnableLooping(bool bLoop) { m_bLoop=bLoop; }

public:
	bool IsEnded() const;
	void Next();

public:
	bool IsNull() { return m_pHead == NULL; }

public:
	void DeletePath();

protected:
	GLPathNode* m_pHead;
	GLPathNode* m_pTail;
	GLPathNode* m_pCurrent;
};
#endif