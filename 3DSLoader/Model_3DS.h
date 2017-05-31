#ifndef _H_LIB3DS_MODEL_3DS_H
#define _H_LIB3DS_MODEL_3DS_H

#include "../GLExtension/glxs.h"

#include <lib3ds/file.h>
#include <lib3ds/mesh.h>

#include <iostream>
#include <string>
#include <cstring>
#include <cassert>
#include <vector>

// #define MODEL_3DS_USE_DISPLAY_LIST

// Xianshun says:
// interestingly, when i used the display list for old pc
// the speed of rendering seems to slow down quite a lot
// my personal feeling is that display list usually requires
// quite a high memory allocation, and for older pc, the memory
// might actually be a virtual memory which slows down the
// application 

class CModel3DS
{
public:
	CModel3DS(std::string filename);
	virtual void Draw() const;
	
	virtual ~CModel3DS();

protected:
	virtual void CreateVBO();

protected:
	void GetFaces();
	unsigned int m_TotalFaces;
	Lib3dsFile * m_model;
	GLuint m_VertexVBO, m_NormalVBO, m_TexCoordVBO;
#ifdef MODEL_3DS_USE_DISPLAY_LIST
	GLuint m_display_lists;
	std::vector<GLuint> m_dlist;
#else
	Lib3dsVector *m_pVertexVBO, *m_pNormalVBO;
	Lib3dsTexel *m_pTexCoordVBO;
#endif

};

#endif