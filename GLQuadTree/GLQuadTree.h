#ifndef _H_GL_QUAD_TREE_H
#define _H_GL_QUAD_TREE_H

#include "GLQuadTree_Node.h"
#include <set>

class GameEntity;

class GLQuadTree
{
public:
	GLQuadTree();
	virtual ~GLQuadTree();

protected:
	int m_iLevels;

protected:
	void CreateNodes(int iLevel, GLQuadTree_Node* pParentNode);

public:
	void GetNeighbors(const GameEntity* pAgent, std::set<GameEntity*>& neighbors, double radius);
	void RemoveAgents();
	void AddAgent(GameEntity* pAgent);

public:
	void MakeQuadTree(int iLevels, int iLeft, int iTop, int iRight, int iBottom);
	void Destroy();

protected:
	GLQuadTree_Node* m_pRoot;
};
#endif