#ifndef _H_GL_QUAD_TREE_NODE_H
#define _H_GL_QUAD_TREE_NODE_H

#include "../GLVector/GLVector.h"
#include <set>
#include <map>

class GameEntity;

class GLQuadTree_Node 
{
public:
	GLQuadTree_Node();
	virtual ~GLQuadTree_Node();

public:
	static const size_t TOP_LEFT=0;
	static const size_t BOTTOM_LEFT=1;
	static const size_t TOP_RIGHT=2;
	static const size_t BOTTOM_RIGHT=3;

public:
	double GetWidth() const;
	double GetLength() const;

public:
	std::map<size_t, GLQuadTree_Node*> children;

public:
	void AddAgent(GameEntity* pAgent);
	bool AgentInNode(const GameEntity* pAgent) const;
	void GetNeighbors(const GameEntity* pAgent, std::set<GameEntity*>& neighbors, double radius);
	void RemoveAgents();

public:
	void AllocateChildNodes();
	void SetTopLeftChildNode(GLQuadTree_Node* child);
	void SetTopRightChildNode(GLQuadTree_Node* child);
	void SetBottomLeftChildNode(GLQuadTree_Node* child);
	void SetBottomRightChildNode(GLQuadTree_Node* child);

public:
	std::set<GameEntity*> m_agents;

public:
	GLVector vTopLeft;
	GLVector vBottomRight;
};
#endif