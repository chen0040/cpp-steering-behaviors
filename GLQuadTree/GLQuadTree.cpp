#include "GLQuadTree.h"
#include "../GameEntity/GameEntity.h"
#include <iostream>

GLQuadTree::GLQuadTree()
: m_pRoot(NULL)
{
	
}

GLQuadTree::~GLQuadTree()
{
	Destroy();
}

void GLQuadTree::Destroy()
{
	if(m_pRoot != NULL)
	{
		delete m_pRoot;
		m_pRoot=NULL;
	}
}

void GLQuadTree::CreateNodes(int iLevel, GLQuadTree_Node* pParentNode)
{
	if(iLevel >= m_iLevels)
	{
		return;
	}

	pParentNode->AllocateChildNodes();

	double dPatchWidth=pParentNode->GetWidth() / 2.0;
	double dPatchHeight=pParentNode->GetLength() / 2.0;

	GLQuadTree_Node* cTopLeft=new GLQuadTree_Node();
	cTopLeft->vTopLeft=pParentNode->vTopLeft;
	cTopLeft->vBottomRight=pParentNode->vTopLeft + GLVector(dPatchWidth, 0, dPatchHeight);
	CreateNodes(iLevel+1, cTopLeft);
	pParentNode->SetTopLeftChildNode(cTopLeft);

	GLQuadTree_Node* cTopRight=new GLQuadTree_Node();
	cTopRight->vTopLeft=pParentNode->vTopLeft + GLVector(dPatchWidth, 0, 0);
	cTopRight->vBottomRight=pParentNode->vBottomRight + GLVector(0, 0, -dPatchHeight);
	CreateNodes(iLevel+1, cTopRight);
	pParentNode->SetTopRightChildNode(cTopRight);

	GLQuadTree_Node* cBottomLeft=new GLQuadTree_Node();
	cBottomLeft->vTopLeft=pParentNode->vTopLeft + GLVector(0, 0, dPatchHeight);
	cBottomLeft->vBottomRight=pParentNode->vBottomRight + GLVector(-dPatchWidth, 0, 0);
	CreateNodes(iLevel+1, cBottomLeft);
	pParentNode->SetBottomLeftChildNode(cBottomLeft);

	GLQuadTree_Node* cBottomRight=new GLQuadTree_Node();
	cBottomRight->vTopLeft=pParentNode->vTopLeft + GLVector(dPatchWidth, 0, dPatchHeight);
	cBottomRight->vBottomRight=pParentNode->vBottomRight;
	CreateNodes(iLevel+1, cBottomRight);
	pParentNode->SetBottomRightChildNode(cBottomRight);
}

void GLQuadTree::MakeQuadTree(int iLevels, int iLeft, int iTop, int iRight, int iBottom)
{
	Destroy();

	m_iLevels=iLevels;

	m_pRoot=new GLQuadTree_Node();
	m_pRoot->vTopLeft.x=iLeft;
	m_pRoot->vTopLeft.z=iTop;
	m_pRoot->vBottomRight.x=iRight;
	m_pRoot->vBottomRight.z=iBottom;

	CreateNodes(1, m_pRoot);
}

void GLQuadTree::GetNeighbors(const GameEntity* pAgent, std::set<GameEntity*>& neighbors, double radius)
{
	m_pRoot->GetNeighbors(pAgent, neighbors, radius);
}

void GLQuadTree::RemoveAgents()
{
	m_pRoot->RemoveAgents();
}

void GLQuadTree::AddAgent(GameEntity* pAgent)
{
	m_pRoot->AddAgent(pAgent);
}