#include "Model_3DS.h"
#include "../GLExtension/glxs.h"
#include <sstream>

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

// Load 3DS model
CModel3DS::CModel3DS(std::string filename)
{
	m_TotalFaces = 0;
	
	m_model = lib3ds_file_load(filename.c_str());
	// If loading the model failed, we throw an exception
	if(!m_model)
	{
		std::ostringstream oss;
		oss << "Unable to load " << filename;
		throw oss.str().c_str();
	}

	CreateVBO();
}

// Destructor
CModel3DS::~CModel3DS()
{
	if(glXSys.IsVBOSupport())
	{
		glDeleteBuffers(1, &m_VertexVBO);
		glDeleteBuffers(1, &m_NormalVBO);
		glDeleteBuffers(1, &m_TexCoordVBO);
	}
	else
	{
#ifdef MODEL_3DS_USE_DISPLAY_LIST
		glDeleteLists(m_display_lists, m_TotalFaces);
#else
		delete [] m_pNormalVBO;
		delete [] m_pVertexVBO;
		delete [] m_pTexCoordVBO;
#endif
	}
	
	if(m_model != NULL)
	{
		lib3ds_file_free(m_model);
	}
}

// Copy vertices and normals to the memory of the GPU
void CModel3DS::CreateVBO()
{
	assert(m_model != NULL);
	
	// Calculate the number of faces we have in total
	GetFaces();
	
	// Allocate memory for our vertices and normals
	Lib3dsVector * vertices = new Lib3dsVector[m_TotalFaces * 3];
	Lib3dsVector * normals = new Lib3dsVector[m_TotalFaces * 3];
	Lib3dsTexel* texCoords = new Lib3dsTexel[m_TotalFaces * 3];
	
	Lib3dsMesh * mesh;
	unsigned int FinishedFaces = 0;
	// Loop through all the meshes
	for(mesh = m_model->meshes;mesh != NULL;mesh = mesh->next)
	{
		lib3ds_mesh_calculate_normals(mesh, &normals[FinishedFaces*3]);
		// Loop through every face
		
		for(unsigned int cur_face = 0; cur_face < mesh->faces;cur_face++)
		{
			Lib3dsFace * face = &mesh->faceL[cur_face];
			for(unsigned int i = 0;i < 3;i++)
			{
				if(mesh->texels)
		        {
					memcpy(&texCoords[FinishedFaces*3 + i], mesh->texelL[face->points[i]], sizeof(Lib3dsTexel)); 
                }     
				memcpy(&vertices[FinishedFaces*3 + i], mesh->pointL[face->points[i]].pos, sizeof(Lib3dsVector));
			}
			FinishedFaces++;
		}
	}
	
	if(glXSys.IsVBOSupport())
	{
		// Generate a Vertex Buffer Object and store it with our vertices
		glGenBuffers(1, &m_VertexVBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Lib3dsVector) * 3 * m_TotalFaces, vertices, GL_STATIC_DRAW);
		
		// Generate another Vertex Buffer Object and store the normals in it
		glGenBuffers(1, &m_NormalVBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_NormalVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Lib3dsVector) * 3 * m_TotalFaces, normals, GL_STATIC_DRAW);

		// Generate a third VBO and store the texture coordinates in it. 
		 glGenBuffers(1, &m_TexCoordVBO);
		 glBindBuffer(GL_ARRAY_BUFFER, m_TexCoordVBO);
		 glBufferData(GL_ARRAY_BUFFER, sizeof(Lib3dsTexel) * 3 * m_TotalFaces, texCoords, GL_STATIC_DRAW); 

		 // Clean up our allocated memory
		delete [] vertices;
		delete [] normals;
		delete [] texCoords;
	}
	else
	{
#ifdef MODEL_3DS_USE_DISPLAY_LIST
		m_dlist.reserve(m_TotalFaces);
		m_display_lists=glGenLists(m_TotalFaces);
		for(GLuint i=0; i < this->m_TotalFaces; i++)
		{
			glNewList(m_display_lists+i, GL_COMPILE);
			glBegin(GL_TRIANGLES);
			for(int j=0; j<3; j++)
			{
				glNormal3f(normals[i * 3 + j][0], normals[i * 3 + j][1], normals[i * 3 + j][2]);
				glTexCoord3f(texCoords[i * 3 + j][0], texCoords[i * 3 + j][1], texCoords[i * 3 + j][2]);
				glVertex3f(vertices[i * 3 + j][0], vertices[i * 3 + j][1], vertices[i * 3 + j][2]);	
			}
			glEnd();
			glEndList();
			m_dlist.push_back(i);
		}
#else
		m_pVertexVBO=vertices;
		m_pNormalVBO=normals;
		m_pTexCoordVBO=texCoords;
#endif
	}

	// We no longer need lib3ds
	lib3ds_file_free(m_model);
	m_model = NULL;
}

// Count the total number of faces this model has
void CModel3DS::GetFaces()
{
	assert(m_model != NULL);
	
	m_TotalFaces = 0;
	Lib3dsMesh * mesh;
	// Loop through every mesh
	for(mesh = m_model->meshes;mesh != NULL;mesh = mesh->next)
	{
		// Add the number of faces this mesh has to the total faces
		m_TotalFaces += mesh->faces;
	}
}

// Render the model using Vertex Buffer Objects
void CModel3DS::Draw() const
{
	assert(m_TotalFaces != 0);
	
	if(glXSys.IsVBOSupport())
	{
		// Enable vertex and normal arrays
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);  
		
		// Bind the vbo with the normals
		glBindBuffer(GL_ARRAY_BUFFER, m_NormalVBO);
		// The pointer for the normals is NULL which means that OpenGL will use the currently bound vbo
		glNormalPointer(GL_FLOAT, 0, NULL);

		glBindBuffer(GL_ARRAY_BUFFER, m_TexCoordVBO);     
		glTexCoordPointer(2, GL_FLOAT, 0, NULL);     
		
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexVBO);
		glVertexPointer(3, GL_FLOAT, 0, NULL);
		
		// Render the triangles
		glDrawArrays(GL_TRIANGLES, 0, m_TotalFaces * 3);
		
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY); 
	}
	else
	{
#ifdef MODEL_3DS_USE_DISPLAY_LIST
		glListBase(m_display_lists);
		glCallLists(this->m_TotalFaces, GL_UNSIGNED_INT, &m_dlist[0]);
#else
		for(unsigned int i = 0; i < m_TotalFaces; i++)
		{
			glBegin(GL_TRIANGLES);
			for(unsigned int j=0; j<3; j++)
			{
				glNormal3f(m_pNormalVBO[i * 3 + j][0], m_pNormalVBO[i * 3 + j][1], m_pNormalVBO[i * 3 + j][2]);
				glTexCoord3f(m_pTexCoordVBO[i * 3 + j][0], m_pTexCoordVBO[i * 3 + j][1], m_pTexCoordVBO[i * 3 + j][2]);
				glVertex3f(m_pVertexVBO[i * 3 + j][0], m_pVertexVBO[i * 3 + j][1], m_pVertexVBO[i * 3 + j][2]);
			}
			glEnd();
		}
#endif

	}
}
