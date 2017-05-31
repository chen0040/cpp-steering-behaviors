#include "GLQuadTree_Node.h"
#include <cmath>
#include "../GameEntity/GameEntity.h"
#include <iostream>

GLQuadTree_Node::GLQuadTree_Node()
{

}

GLQuadTree_Node::~GLQuadTree_Node()
{
	if(!children.empty())
	{
		for(size_t i=0; i<4; ++i)
		{
			if(children[i] != NULL)
			{
				delete children[i];
				children[i]=NULL;
			}
		}
	}
	children.clear();
	m_agents.clear();
}

void GLQuadTree_Node::AllocateChildNodes()
{
	if(!children.empty())
	{
		for(size_t i=0; i<4; ++i)
		{
			if(children[i] != NULL)
			{
				delete children[i];
				children[i]=NULL;
			}
		}
	}
	children.clear();
	m_agents.clear();
	for(size_t i=0; i<4; ++i)
	{
		children[i]=NULL;
	}
}

void GLQuadTree_Node::SetTopLeftChildNode(GLQuadTree_Node* child)
{
	this->children[GLQuadTree_Node::TOP_LEFT]=child;
}

void GLQuadTree_Node::SetTopRightChildNode(GLQuadTree_Node* child)
{
	this->children[GLQuadTree_Node::TOP_RIGHT]=child;
}

void GLQuadTree_Node::SetBottomLeftChildNode(GLQuadTree_Node* child)
{
	this->children[GLQuadTree_Node::BOTTOM_LEFT]=child;
}

void GLQuadTree_Node::SetBottomRightChildNode(GLQuadTree_Node* child)
{
	this->children[GLQuadTree_Node::BOTTOM_RIGHT]=child;
}

double GLQuadTree_Node::GetWidth() const
{
	return fabs(vBottomRight.x - vTopLeft.x);
}

double GLQuadTree_Node::GetLength() const
{
	return fabs(vBottomRight.z - vTopLeft.z);
}

void GLQuadTree_Node::AddAgent(GameEntity* pAgent)
{
	/*
	std::cout << "Agent Position: " << pAgent->get_position().toString() << std::endl;
	std::cout << "TopLeft: " << vTopLeft.toString() << std::endl;
	std::cout << "BottomRight: " << vBottomRight.toString() << std::endl;
	*/

	if(AgentInNode(pAgent))
	{
		if(children.empty())
		{
			m_agents.insert(pAgent);
		}
		else
		{
			children[TOP_LEFT]->AddAgent(pAgent);
			children[TOP_RIGHT]->AddAgent(pAgent);
			children[BOTTOM_RIGHT]->AddAgent(pAgent);
			children[BOTTOM_LEFT]->AddAgent(pAgent);
		}
	}
}

bool GLQuadTree_Node::AgentInNode(const GameEntity* pAgent) const
{
	double agent_x=pAgent->get_position().x;
	double agent_z=pAgent->get_position().z;
	double bounding_radius=pAgent->get_bounding_radius();

	double minx=vTopLeft.x;
	double minz=vTopLeft.z;
	double maxx=vBottomRight.x;
	double maxz=vBottomRight.z;

	double radiussq=bounding_radius * bounding_radius;
	double dist1sq=(agent_x - minx) * (agent_x - minx) + (agent_z - minz) * (agent_z - minz);
	if(dist1sq <= radiussq)
	{
		return true;
	}
	double dist2sq=(agent_x - minx) * (agent_x - minx) + (agent_z - maxz) * (agent_z - maxz);
	if(dist2sq <= radiussq)
	{
		return true;
	}
	double dist3sq=(agent_x - maxx) * (agent_x - maxx) + (agent_z - maxz) * (agent_z - maxz);
	if(dist3sq <= radiussq)
	{
		return true;
	}
	double dist4sq=(agent_x - maxx) * (agent_x - maxx) + (agent_z - minz) * (agent_z - minz);
	if(dist4sq <= radiussq)
	{
		return true;
	}
	if(maxx >= agent_x && minx <= agent_x && minz <= agent_z && maxz >= agent_z)
	{
		return true;
	}

	return false;

}

void GLQuadTree_Node::GetNeighbors(const GameEntity* pAgent, std::set<GameEntity*>& neighbors, double radius)
{
	if(AgentInNode(pAgent))
	{
		if(children.empty())
		{
			if(!m_agents.empty())
			{
				neighbors.insert(m_agents.begin(), m_agents.end());
			}
		}
		else
		{
			children[TOP_LEFT]->GetNeighbors(pAgent, neighbors, radius);
			children[TOP_RIGHT]->GetNeighbors(pAgent, neighbors, radius);
			children[BOTTOM_RIGHT]->GetNeighbors(pAgent, neighbors, radius);
			children[BOTTOM_LEFT]->GetNeighbors(pAgent, neighbors, radius);
		}
	}
}

void GLQuadTree_Node::RemoveAgents()
{
	m_agents.clear();
	if(!children.empty())
	{
		children[TOP_LEFT]->RemoveAgents();
		children[TOP_RIGHT]->RemoveAgents();
		children[BOTTOM_RIGHT]->RemoveAgents();
		children[BOTTOM_LEFT]->RemoveAgents();
	}
}