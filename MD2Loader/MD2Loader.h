#ifndef _MD2LOADER_H
#define _MD2LOADER_H

#include <fstream>
#include <iostream>
#include <ctime>
#include <memory.h>
#include <cmath>
#include "../GLExtension/glxs.h"

#define MD2_OK         0x0
#define MD2_ERR_MEM    0x1
#define MD2_ERR_FILE   0x2
#define MD2_ERR_FORMAT 0x4


struct MD2Header
{
	int ID;									// File Type - Normally 'IPD2'
	int Version;    
	int TexWidth;   // Texture width
	int TexHeight;  // Texture height 
	int FrameSize;  // Size for frames in bytes
	int nTextures;  // Number of textures
	int nVertices;  // Number of vertices
	int nTexCoords; // Number of UVs
	int nTriangles; // Number of polys
	int nGLCmd;     // Number of GL Commmands
	int nFrames;    // Number of frames
	int TexOffset;  // Offset to texture name(s)
	int UVOffset;   // Offset to UV data
	int FaceOffset; // Offset to poly data
	int FrameOffset;// Offset to first frame  
	int GLCmdOffset;// Offset to GL Cmds 
	int EOFOffset;  // Size of file
};


struct MD2FrameInfo
{
	float Scale[3];
	float Translate[3];
	char Name[16];
};


struct MD2Face
{
	short p1,p2,p3;
	short uv1,uv2,uv3;
};


struct MD2Vtx
 {
  unsigned char Vtx[3];
  unsigned char lNorm;
 };


struct Mesh_Vtx
{
	float x,y,z;
};


struct Mesh_UV
{
	float u,v;
};


struct MD2Frame
{
	Mesh_Vtx *Vtx;
	Mesh_Vtx *Norm;
};


struct MD2TexCoord
{
	short u,v;
};	 


class MD2Obj
{
public:
	MD2Obj();
	~MD2Obj();
	int Load(const char* filename);
	int GetFrameCount();
	char* GetTexName();
	void SetTexture(GLuint TexNum);
	void Draw(int Frame);

private:
	int nFrames,nTri,nVtx,nUV;
	MD2Face *Face;
	MD2Frame *frame;
	Mesh_UV *UV;
	char TexName[64];
	GLuint TexID;

	void CalcNormal(Mesh_Vtx v1,Mesh_Vtx v2,Mesh_Vtx v3,Mesh_Vtx* Result);
};

#endif