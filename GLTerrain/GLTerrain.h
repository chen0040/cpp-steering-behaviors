#ifndef _H_GL_TERRAIN_H
#define _H_GL_TERRAIN_H

#include "../GLVector/GLVector.h"
#include <sdl.h>
#include "../GLExtension/glxs.h"
#include "GLTerrainPatch.h"
#include "../GLQuadTree/GLQuadTree.h"
#include "../GLTestFunc/GLTestFunc.h"
#include <vector>
#include <string>

class GameWorld;

class GLTerrain
{
public:
	GLTerrain();
	virtual ~GLTerrain();

public:
	virtual void PreRender();
	virtual void PostRender();
	virtual void Render();

protected:
	void Cleanup();

public:
	double GetHeight(int x, int z) const;
	GLVector GetHeightVector(const GLVector& base) const;

protected:
	bool m_bLoadedFromImage;
	std::string m_LoadImageName;
public:
	void LoadFromImage(const char* img_map, GameWorld* pWorld);
	void LoadFromTestFunc(GLTestFunc* tf, int iWidth, int iHeight, GameWorld* pWorld);
	void ResetQuadTree() { m_obsQuadTree.RemoveAgents(); }
public:
	bool IsLoadedFromImage() { return m_bLoadedFromImage; }
	std::string GetLoadedImageName() const { return m_LoadImageName; }

protected:
	double m_dMaxHeight;
	int m_iHeight;
	int m_iWidth;

	int m_iPatchesPerRow;
	int m_iPatchesPerCol;

	int m_iPatchWidth;
	int m_iPatchHeight;

	std::vector<GLTerrainPatch*> m_patches;

protected:
	GLQuadTree m_obsQuadTree;
public:
	GLQuadTree& GetObstacleQuadTree() { return m_obsQuadTree; }

protected:
	void SetColor(int i, int j, GLVector& vColor, const GLVector* height_map);

public:
	double GetWidth() const { return m_iWidth; }
	double GetLength() const { return m_iHeight; }

protected:
	GameWorld* m_pWorld;
	bool m_bFrustumEnabled;
};
#endif