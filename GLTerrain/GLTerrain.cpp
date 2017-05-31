#include "GLTerrain.h"
#include "../GLUtil/GLExitSystem.h"
#include <sdl_image.h>
#include <iostream>
#include <vector>
#include "../GLMath/GLMath.h"
#include "../GLTerrain/GLTerrain.h"
#include "../GameWorld/GameWorld.h"
#include <cmath>
#include <cfloat>

GLTerrain::GLTerrain()
: m_dMaxHeight(15)
, m_iPatchesPerRow(64)
, m_iPatchesPerCol(64)
, m_pWorld(NULL)
, m_bFrustumEnabled(true)
, m_bLoadedFromImage(false)
{
	
}

void GLTerrain::LoadFromTestFunc(GLTestFunc* tf, int iWidth, int iHeight, GameWorld* pWorld)
{
	this->Cleanup();

	m_bLoadedFromImage=false;
	m_LoadImageName="";

	m_pWorld=pWorld;

	m_iHeight=iWidth;
	m_iWidth=iHeight;

	int iLeftOffset=- m_iWidth / 2;
	int iTopOffset=- m_iHeight / 2;

	GLVector* height_map=new GLVector[m_iHeight * m_iWidth];
	GLVector* height_normal=new GLVector[m_iWidth * m_iHeight];
	GLVector* height_color=new GLVector[m_iWidth * m_iHeight];

	double max_height=-DBL_MAX;
	double min_height=DBL_MAX;
	for(int i=0; i<m_iHeight; i++)
	{
		for(int j=0; j<m_iWidth; j++)
		{
			double height=tf->GetValue(i+iLeftOffset, j+iTopOffset);
			height_map[i * m_iWidth + j]=GLVector(j+iTopOffset, height, i+iLeftOffset);
			if(max_height < height)
			{
				max_height=height;
			}
			if(min_height > height)
			{
				min_height=height;
			}
		}
	}

	double abs_height=max_height - min_height;

	for(int i=0; i<m_iHeight; i++)
	{
		for(int j=0; j<m_iWidth; j++)
		{
			height_map[i * m_iWidth + j].y=m_dMaxHeight * (height_map[i * m_iWidth + j].y - min_height) / abs_height; 
		}
	}

	for(int i=0; i<m_iHeight; i++)
	{
		for(int j=0; j<m_iWidth; j++)
		{
			GLVector center=height_map[i * m_iWidth + j];
			std::vector<GLVector> vs;
			if(i-1 >= 0 && j-1>= 0)
			{
				vs.push_back(height_map[(i-1) * m_iWidth + (j-1)]);
			}
			if(i-1 >= 0 && j+1<m_iWidth)
			{
				vs.push_back(height_map[(i-1) * m_iWidth + (j+1)]);
			}
			if(i+1 < m_iHeight && j+1 < m_iWidth)
			{
				vs.push_back(height_map[(i+1) * m_iWidth + (j+1)]);
			}	
			if(i+1 < m_iHeight && j-1 >= 0)
			{
				vs.push_back(height_map[(i+1) * m_iWidth + (j-1)]);
			}
			for(size_t k=0; k<vs.size(); k++)
			{
				vs[k]=vs[k] - center;
			}
			GLVector normal;
			for(size_t k=0; k<vs.size(); k++)
			{
				normal+=vs[k].cross(vs[(k+1)% vs.size()]);
			}
			normal /= vs.size();
			height_normal[i * m_iWidth + j]=normal;
		}
	}

	for(int i=0; i < m_iHeight; i++)
	{
		for(int j=0; j < m_iWidth; j++)
		{
			SetColor(i, j, height_color[i * m_iWidth + j], height_map);
		}
	}

	m_iPatchWidth=m_iWidth / m_iPatchesPerRow;
	m_iPatchHeight=m_iHeight / m_iPatchesPerCol;
	int iPatchLeft=0;
	int iPatchTop=0;

	m_patches.reserve(m_iPatchesPerRow * m_iPatchesPerCol);
	for(int z=0; z<m_iPatchesPerCol; z++)
	{
		iPatchTop=z * m_iPatchHeight;
		for(int x=0; x<m_iPatchesPerRow; x++)
		{
			iPatchLeft=x * m_iPatchWidth;
			GLTerrainPatch* patch=new GLTerrainPatch();
			patch->Create(iPatchLeft, iPatchTop, m_iPatchWidth, m_iPatchHeight, m_iWidth, m_iHeight, height_map, height_normal, height_color);
			m_patches.push_back(patch);
		}
	}

	delete [] height_map;
	delete [] height_normal;
	delete [] height_color;

	m_obsQuadTree.MakeQuadTree(5, iLeftOffset, iTopOffset, iLeftOffset+m_iWidth, iTopOffset+m_iHeight);
}

void GLTerrain::LoadFromImage(const char* img_map, GameWorld* pWorld)
{
	this->Cleanup();

	m_bLoadedFromImage=true;
	m_LoadImageName=img_map;

	m_pWorld=pWorld;

	SDL_Surface* height_map_surface=IMG_Load(img_map);
	if(height_map_surface==NULL)
	{
		GLExitSystem("GLTerrain::GLTerrain(const char* img_map)", "Failed to load image for terrain");
	}

	int nColors=height_map_surface->format->BytesPerPixel;
	Uint8* pixels=(Uint8*)(height_map_surface->pixels);

	m_iHeight=height_map_surface->h;
	m_iWidth=height_map_surface->w;

	int iLeftOffset=- m_iWidth / 2;
	int iTopOffset=- m_iHeight / 2;

	GLVector* height_map=new GLVector[m_iHeight * m_iWidth];
	GLVector* height_normal=new GLVector[m_iWidth * m_iHeight];
	GLVector* height_color=new GLVector[m_iWidth * m_iHeight];

	double max_height=0;
	for(int i=0; i<m_iHeight; i++)
	{
		for(int j=0; j<m_iWidth; j++)
		{
			double height=(double)pixels[(i * height_map_surface->pitch + j * nColors)];
			height_map[i * m_iWidth + j]=GLVector(j+iTopOffset, height, i+iLeftOffset);
			if(max_height < height)
			{
				max_height=height;
			}
		}
	}

	SDL_FreeSurface(height_map_surface);

	for(int i=0; i<m_iHeight; i++)
	{
		for(int j=0; j<m_iWidth; j++)
		{
			height_map[i * m_iWidth + j].y=m_dMaxHeight * height_map[i * m_iWidth + j].y / max_height; 
		}
	}

	for(int i=0; i<m_iHeight; i++)
	{
		for(int j=0; j<m_iWidth; j++)
		{
			GLVector center=height_map[i * m_iWidth + j];
			std::vector<GLVector> vs;
			if(i-1 >= 0 && j-1>= 0)
			{
				vs.push_back(height_map[(i-1) * m_iWidth + (j-1)]);
			}
			if(i-1 >= 0 && j+1<m_iWidth)
			{
				vs.push_back(height_map[(i-1) * m_iWidth + (j+1)]);
			}
			if(i+1 < m_iHeight && j+1 < m_iWidth)
			{
				vs.push_back(height_map[(i+1) * m_iWidth + (j+1)]);
			}	
			if(i+1 < m_iHeight && j-1 >= 0)
			{
				vs.push_back(height_map[(i+1) * m_iWidth + (j-1)]);
			}
			for(size_t k=0; k<vs.size(); k++)
			{
				vs[k]=vs[k] - center;
			}
			GLVector normal;
			for(size_t k=0; k<vs.size(); k++)
			{
				normal+=vs[k].cross(vs[(k+1)% vs.size()]);
			}
			normal /= vs.size();
			height_normal[i * m_iWidth + j]=normal.normalize();
		}
	}

	for(int i=0; i < m_iHeight; i++)
	{
		for(int j=0; j < m_iWidth; j++)
		{
			SetColor(i, j, height_color[i * m_iWidth + j], height_map);
		}
	}

	m_iPatchWidth=m_iWidth / m_iPatchesPerRow;
	m_iPatchHeight=m_iHeight / m_iPatchesPerCol;
	int iPatchLeft=0;
	int iPatchTop=0;

	m_patches.reserve(m_iPatchesPerRow * m_iPatchesPerCol);
	for(int z=0; z<m_iPatchesPerCol; z++)
	{
		iPatchTop=z * m_iPatchHeight;
		for(int x=0; x<m_iPatchesPerRow; x++)
		{
			iPatchLeft=x * m_iPatchWidth;
			GLTerrainPatch* patch=new GLTerrainPatch();
			patch->Create(iPatchLeft, iPatchTop, m_iPatchWidth, m_iPatchHeight, m_iWidth, m_iHeight, height_map, height_normal, height_color);
			m_patches.push_back(patch);
		}
	}

	delete [] height_map;
	delete [] height_normal;
	delete [] height_color;

	m_obsQuadTree.MakeQuadTree(5, iLeftOffset, iTopOffset, iLeftOffset+m_iWidth, iTopOffset+m_iHeight);

}

GLTerrain::~GLTerrain()
{
	this->Cleanup();
}

void GLTerrain::Cleanup()
{
	std::vector<GLTerrainPatch*>::iterator pos;
	for(pos=m_patches.begin(); pos != m_patches.end(); ++pos)
	{
		if((*pos) != NULL)
		{
			delete (*pos);
			(*pos)=NULL;
		}
	}
	m_patches.clear();
}

void GLTerrain::PreRender()
{
	glShadeModel(GL_SMOOTH);
	glEnable(GL_COLOR_MATERIAL);
	glPushMatrix();
}

void GLTerrain::PostRender()
{
	glPopMatrix();
	glDisable(GL_COLOR_MATERIAL);
	glShadeModel(GL_FLAT);
}

GLVector GLTerrain::GetHeightVector(const GLVector& base) const
{
	GLVector height_vector=base;

	int x=static_cast<int>(floor(base.x));
	int z=static_cast<int>(floor(base.z));

	height_vector.y=GetHeight(x, z);
	
	return height_vector;
}

double GLTerrain::GetHeight(int x, int z) const
{
	x+=m_iWidth / 2;
	z+=m_iHeight / 2;

	if(x < 0 || x >= m_iWidth || z < 0 || z >= m_iHeight)
	{
		return 0;
	}

	int iPatchX=x / m_iPatchWidth;
	int iPatchZ=z / m_iPatchHeight;

	int patch_index=iPatchZ * m_iPatchesPerRow + iPatchX;

	if(patch_index >= static_cast<int>(m_patches.size()))
	{
		return 0;
	}

	return m_patches[patch_index]->GetHeight(x, z);
}

void GLTerrain::Render()
{
	std::vector<GLTerrainPatch*>::const_iterator pos;
	GLTerrainPatch* patch=NULL;

	for(pos=m_patches.begin(); pos != m_patches.end(); ++pos)
	{
		patch=*pos;
		if(m_bFrustumEnabled==false || this->m_pWorld->GetFrustum().CubeInFrustum(patch->GetBoundingCube()))
		{
			patch->Render();
		}
	}
	/*
	for(int i=0; i<m_iPatchesPerCol; i++)
	{
		for(int j=0; j<m_iPatchesPerRow; j++)
		{
			int patch_index=i * m_iPatchesPerRow + j;
			if(m_bFrustumEnabled==false || this->m_pWorld->GetFrustum().CubeInFrustum(m_patches[patch_index]->GetBoundingCube()))
			{
				m_patches[patch_index]->Render();
			}
		}
	}
	*/
}

void GLTerrain::SetColor(int i, int j, GLVector& vColor, const GLVector* height_map)
{
	double r, g, b;
	int index=i * m_iWidth + j;
	if(height_map[index].y < m_dMaxHeight * 0.05)
	{
		r=0.0;
		g=glMath.nextDouble() * 0.05 + 0.7;
		b=0.0;
	}
	else if(height_map[index].y < m_dMaxHeight * 0.3)
	{
		r=0.0;
		g=0.8;
		b=0.0;
	}
	else if(height_map[index].y < m_dMaxHeight * 0.6)
	{
		double c=glMath.nextDouble() * 0.02 + 0.5;

		r=c;
		g=c;
		b=c;
	}
	else if(height_map[index].y < m_dMaxHeight * 0.7)
	{
		double c=glMath.nextDouble() * 0.02 + 0.6;

		r=c;
		g=c;
		b=c;
	}
	else if(height_map[index].y < m_dMaxHeight * 0.8)
	{
		double c=glMath.nextDouble() * 0.02 + 0.8;
		r=c;
		g=c;
		b=c;
	}
	else
	{
		r=1;
		g=1;
		b=1;
	}

	vColor.x=r;
	vColor.y=g;
	vColor.z=b;
}