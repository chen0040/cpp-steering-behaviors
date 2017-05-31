#ifndef _H_GL_TERRAIN_PATCH_H
#define _H_GL_TERRAIN_PATCH_H

#include "../GLVector/GLVector.h"
#include <sdl.h>
#include "../GLExtension/glxs.h"
#include <vector>
#include "../GLShape/GLCube.h"

class GLTerrainPatch
{
public:
	GLTerrainPatch();
	virtual ~GLTerrainPatch();

public:
	virtual void Render();

public:
	double GetHeight(int x, int z) const;
	GLVector GetHeightVector(const GLVector& base) const;

public:
	void Create(int iPatchLeft, int iPatchTop, int iPatchWidth, int iPatchHeight, int iMapWidth, int iMapHeight, const GLVector* height_map, const GLVector* height_normal, const GLVector* height_color);

protected:
	GLVector* m_height_map;
	GLVector* m_height_normal;
	GLVector* m_height_color;
	GLuint m_VertexVBO, m_NormalVBO, m_ColorVBO, m_IndexVBO;
	GLCube m_bounding_cube;

public:
	GLCube GetBoundingCube() const { return m_bounding_cube; }

protected:
	int m_iPatchWidth;
	int m_iPatchHeight;
	int m_iPatchLeft;
	int m_iPatchTop;
	GLint m_iIndexCount;
};
#endif