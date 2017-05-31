#include "GLTerrainPatch.h"
#include "../GLUtil/GLExitSystem.h"
#include <sdl_image.h>
#include <iostream>
#include <vector>
#include <cassert>
#include "../GLMath/GLMath.h"
#include "../GLExtension/glxs.h"

GLTerrainPatch::GLTerrainPatch()
: m_height_map(NULL)
, m_height_normal(NULL)
, m_height_color(NULL)
{
	
}

void GLTerrainPatch::Create(int iPatchLeft, int iPatchTop, int iPatchWidth, int iPatchHeight, int iMapWidth, int iMapHeight, const GLVector* height_map, const GLVector* height_normal, const GLVector* height_color)
{
	assert(m_height_map == NULL);
	assert(m_height_normal == NULL);
	assert(m_height_color == NULL);
//	assert(m_indices.empty());

	m_iPatchHeight=iPatchHeight+1;
	m_iPatchWidth=iPatchWidth+1;
	m_iPatchLeft=iPatchLeft;
	m_iPatchTop=iPatchTop;

	int iPatchDimension=m_iPatchHeight * m_iPatchWidth;
	int iMapDimension=iMapWidth * iMapHeight;

	m_height_map=new GLVector[iPatchDimension];
	m_height_normal=new GLVector[iPatchDimension];
	m_height_color=new GLVector[iPatchDimension];

	for(int z=0; z<m_iPatchHeight; z++)
	{
		for(int x=0; x<m_iPatchWidth; x++)
		{
			int map_index=(m_iPatchTop + z)* iMapWidth + m_iPatchLeft + x; 
			int patch_index=z * m_iPatchWidth + x;

			if(map_index >= iMapDimension || map_index < 0)
			{
				continue;
			}

			m_height_map[patch_index]=height_map[map_index];
			m_height_normal[patch_index]=height_normal[map_index];
			m_height_color[patch_index]=height_color[map_index];

			m_bounding_cube.Expand(height_map[map_index]);
		}
	}
	m_bounding_cube.Expand(m_iPatchWidth * 0.1);

	if(glXSys.IsVBOSupport())
	{
		std::vector<unsigned int> m_indices;

		for(int i=0; i<m_iPatchHeight-1; i++)
		{
			for(int j=0; j<m_iPatchWidth/*-1*/; j++)
			{
				unsigned int index1=i * m_iPatchWidth + j;
				unsigned int index2=(i+1) * m_iPatchWidth + j;

				/*
				unsigned int index3=(i+1) * m_iPatchWidth + (j+1);
				unsigned int index4=i * m_iPatchWidth + (j + 1);
				*/
				
				/*
				m_indices.push_back(index4);
				m_indices.push_back(index3);
				*/

				m_indices.push_back(index2);
				m_indices.push_back(index1);
			}
		}

		glGenBuffers(1, &m_VertexVBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLVector)* iPatchDimension, m_height_map, GL_STATIC_DRAW);

		glGenBuffers(1, &m_NormalVBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_NormalVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLVector) * iPatchDimension, m_height_normal, GL_STATIC_DRAW);

		glGenBuffers(1, &m_ColorVBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_ColorVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLVector) * iPatchDimension, m_height_color, GL_STATIC_DRAW);

		m_iIndexCount=static_cast<GLsizei>(m_indices.size());
		glGenBuffers(1, &m_IndexVBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexVBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * m_iIndexCount, &m_indices[0], GL_STATIC_DRAW);
	}
}

GLTerrainPatch::~GLTerrainPatch()
{
	if(glXSys.IsVBOSupport())
	{
		glDeleteBuffers(1, &m_VertexVBO);
		glDeleteBuffers(1, &m_NormalVBO);
		glDeleteBuffers(1, &m_ColorVBO);
		glDeleteBuffers(1, &m_IndexVBO);
	}

	if(m_height_map != NULL)
	{
		delete [] m_height_map;
		m_height_map=NULL;
	}
	if(m_height_normal != NULL)
	{
		delete [] m_height_normal;
		m_height_normal=NULL;
	}
	if(m_height_color != NULL)
	{
		delete [] m_height_color;
		m_height_color=NULL;
	}
}

GLVector GLTerrainPatch::GetHeightVector(const GLVector& base) const
{
	GLVector height_vector=base;

	int x=static_cast<int>(base.x);
	int z=static_cast<int>(base.z);

	height_vector.y=GetHeight(x, z);
	
	return height_vector;
}

double GLTerrainPatch::GetHeight(int x, int z) const
{
	x-=m_iPatchLeft;
	z-=m_iPatchTop;

	if(x < 0 || x >=m_iPatchWidth)
	{
		return 0;
	}
	if(z < 0 || z >= m_iPatchHeight)
	{
		return 0;
	}

	return m_height_map[z * m_iPatchWidth + x].y;
}

void GLTerrainPatch::Render()
{
	if(glXSys.IsVBOSupport())
	{
		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);
		glEnableClientState(GL_VERTEX_ARRAY);

		glBindBuffer(GL_ARRAY_BUFFER, m_ColorVBO);
		glColorPointer(3, GL_DOUBLE, 0, NULL);

		glBindBuffer(GL_ARRAY_BUFFER, m_NormalVBO);
		glNormalPointer(GL_DOUBLE, 0, NULL);

		glBindBuffer(GL_ARRAY_BUFFER, m_VertexVBO);
		glVertexPointer(3, GL_DOUBLE, 0, NULL);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexVBO);
		glDrawElements(GL_QUAD_STRIP, m_iIndexCount, GL_UNSIGNED_INT, NULL);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
	}
	else
	{
		for(int i=0; i<m_iPatchHeight-1; i++)
		{
			glBegin(GL_TRIANGLE_STRIP);
			for(int j=0; j<m_iPatchWidth; j++)
			{
				int index=(i+1) * m_iPatchWidth + j;
				glColor3d(m_height_color[index].x, m_height_color[index].y, m_height_color[index].z);
				glNormal3d(m_height_normal[index].x, m_height_normal[index].y, m_height_normal[index].z);
				glVertex3d(m_height_map[index].x, m_height_map[index].y, m_height_map[index].z);

				index=i * m_iPatchWidth + j;
				glColor3d(m_height_color[index].x, m_height_color[index].y, m_height_color[index].z);
				glNormal3d(m_height_normal[index].x, m_height_normal[index].y, m_height_normal[index].z);
				glVertex3d(m_height_map[index].x, m_height_map[index].y, m_height_map[index].z);
			}
			glEnd();
		}
	}
}
