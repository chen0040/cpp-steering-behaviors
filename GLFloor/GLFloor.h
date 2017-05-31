#ifndef _H_GAME_PLANE_H
#define _H_GAME_PLANE_H

#include "../GLObject/GLObject.h"
#include "../GLTexture/GLTexture.h"
#include "../GLVector/GLVector.h"
#include "../GLQuadTree/GLQuadTree.h"
#include <vector>

class GameWorld;

class GLFloor : public GLObject
{
public:
	GLFloor();
	virtual ~GLFloor();

public:
	void Init(const char* texture_name, GameWorld* pWorld);

public:
	virtual void Update(const long& lElapsedTicks) { }
	virtual void PreRender();
	virtual void PostRender();
	virtual void Render();

protected:
	virtual void RenderReflection();
	virtual void RenderNormal();

public:
	void SetTexture(GLTexture* pTexture) { m_pTexture=pTexture; }
	GLTexture* GetTexture() const { return m_pTexture; }
	void ResetQuadTree() { m_obsQuadTree.RemoveAgents(); }

protected:
	GLQuadTree m_obsQuadTree;
public:
	GLQuadTree& GetObstacleQuadTree() { return m_obsQuadTree; }

protected:
	void calculate_normal();

public:
	void set_p1(const GLVector& p) { p1=p; calculate_normal(); }
	void set_p2(const GLVector& p) { p2=p; calculate_normal(); }
	void set_p3(const GLVector& p) { p3=p; calculate_normal(); }
	void set_p4(const GLVector& p) { p4=p; calculate_normal(); }
	GLVector get_p1() const { return p1; }
	GLVector get_p2() const { return p2; }
	GLVector get_p3() const { return p3; }
	GLVector get_p4() const { return p4; }

public:
	double GetLength() const;
	void CreateHorizontalPlane(double length);

public:
	static const int DEFAULT_LENGTH=120;

protected:
	GLuint m_display_lists;
	bool m_bDisplayList;
protected:
	void GenDisplayLists();

protected:
	GLVector p1;
	GLVector p2;
	GLVector p3;
	GLVector p4;

protected:
	GLVector m_normal;
	double m_distance_to_origin;
public:
	GLVector normal() const { return m_normal; }
	double distance_to_origin() const { return m_distance_to_origin; }


private:
	GLTexture* m_pTexture;

private:
	std::vector<GLObject*> m_reflected_objects;
public:
	void clear_reflected_objects() { m_reflected_objects.clear(); }
	void add_reflected_object(GLObject* pObj) { m_reflected_objects.push_back(pObj); }

protected:
	GameWorld* m_pWorld;
};
#endif