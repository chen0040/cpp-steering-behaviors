#include "MD2Loader.h"

// #define DEBUG_MD2LOADER

MD2Obj::MD2Obj()
	{
	 // Terminate Arrays
		Face=NULL;
		frame=NULL;
		UV=NULL;
		TexName[0]=NULL;

		TexID=0;
	}


MD2Obj::~MD2Obj()
	{
   // Free Arrays
	  if(Face)
				{
				 delete [] Face;
					Face=NULL;
				}

			if(frame)
				{
				 delete [] frame;
					frame=NULL;
				}

			if(UV)
				{
				 delete [] UV;
					UV=NULL;
				}
	}


int MD2Obj::Load(const char *filename)
{
	using namespace std;

	std::ifstream fIn;
	unsigned long fSize;
	unsigned char *data=NULL;

	MD2Header Head;
	long FrameLoop,ItemLoop;
	MD2Vtx *vtx;
	MD2TexCoord *MD2_UV;
	MD2FrameInfo FrameInfo;

	// Clear any existing data
	if(Face)
	{
		delete [] Face;
		Face=NULL;
	}

	if(frame)
	{
		delete [] frame;
		frame=NULL;
	}

	if(UV)
	{
		delete [] UV;
		UV=NULL;
	}


  // Open the specified file
  fIn.open(filename,ios::binary);
    
   
  // Get file size
  fIn.seekg(0,ios_base::end);
  fSize=fIn.tellg();
  fIn.seekg(0,ios_base::beg);

  // Allocate some space
  data=new unsigned char[fSize];

   if(data==NULL)
    {
     fIn.close();
     return MD2_ERR_MEM;
    }

  // Read the file into memory
  fIn.read((char*)data,fSize);

  fIn.close();


		// Get Header data
  memcpy(&Head,data,68);

#ifdef DEBUG_MD2LOADER
	// Dump info about object
	cout<<"ID - "<<data[0]<<data[1]<<data[2]<<data[3]<<"\n";
	cout<<"Version - "<<Head.Version<<"\n";   
	cout<<"Tex Width - "<<Head.TexWidth<<"\n";
	cout<<"Tex Height - "<<Head.TexHeight<<"\n";
	cout<<"Frame Size - "<<Head.FrameSize<<"\n";
	cout<<"Textures - "<<Head.nTextures<<"\n";
	cout<<"Vertices - "<<Head.nVertices<<"\n";
	cout<<"UVs - "<<Head.nTexCoords<<"\n";
	cout<<"Faces - "<<Head.nTriangles<<"\n";
	cout<<"GL cmds - "<<Head.nGLCmd<<"\n";
	cout<<"Frames - "<<Head.nFrames<<"\n";
	cout<<"Skin Offset - "<<Head.TexOffset<<"\n";
	cout<<"UV Offset - "<<Head.UVOffset<<"\n";
	cout<<"Face Offset - "<<Head.FaceOffset<<"\n";
	cout<<"Frame Offset - "<<Head.FrameOffset<<"\n";
	cout<<"GL Offset - "<<Head.GLCmdOffset<<"\n";
	cout<<"Filesize - "<<Head.EOFOffset<<"\n";
#endif


		// A few checks to ensure this is an MD2 file
		 if(Head.ID!=844121161) // chars 'IDP2' as a dword
				return MD2_ERR_FORMAT;

		 if(Head.Version!=8)
				return MD2_ERR_FORMAT;

		 if(Head.EOFOffset!=fSize)
				return MD2_ERR_FORMAT;


		// Grab the info we'll need later
		nFrames=Head.nFrames;
		nTri=Head.nTriangles;
		nVtx=Head.nVertices;
		nUV=Head.nTexCoords;


		// Allocate arrays

		// Frames
		frame=new MD2Frame[nFrames];

		 if(!frame)
				{
				 delete [] data;
					return MD2_ERR_MEM;
				}

  // Frame components
		 for(FrameLoop=0;FrameLoop!=nFrames;++FrameLoop)
				{
				 frame[FrameLoop].Vtx = new Mesh_Vtx[nVtx];
					frame[FrameLoop].Norm= new Mesh_Vtx[nVtx];

					 if(!frame[FrameLoop].Vtx || !frame[FrameLoop].Norm)
							{
							 delete [] data;
								return MD2_ERR_MEM;
							}
				}

		// MD2 vtx buffer
		vtx = new MD2Vtx[Head.nVertices];
  
  // Faces
		Face = new MD2Face[Head.nTriangles];

		// UVs
		UV = new Mesh_UV[nUV];

		// MD2 UV buffer
		MD2_UV = new MD2TexCoord[Head.nTexCoords];


		// Check we've created the arrays
			if(!vtx || !Face || !UV || !MD2_UV)
			 {
				 delete [] data;
					return MD2_ERR_MEM;
				}


		// Extract and convert info from file
  
		// Read first texture name
		 if(Head.nTextures>0)
				{
				 memcpy(TexName,&data[Head.TexOffset],64);

#ifdef DEBUG_MD2LOADER
					cout<<"Texture Name - "<<TexName<<"\n";
#endif
				}

		// Read face data
		memcpy(Face,&data[Head.FaceOffset],Head.nTriangles*sizeof(MD2Face));

		// Read MD2 UV data
  memcpy(MD2_UV,&data[Head.UVOffset],Head.nTexCoords*sizeof(MD2TexCoord));

		// Convert into regular UVs
		 for(ItemLoop=0;ItemLoop!=nUV;++ItemLoop)
				{
				 UV[ItemLoop].u=((float)MD2_UV[ItemLoop].u)/Head.TexWidth;
     UV[ItemLoop].v=((float)MD2_UV[ItemLoop].v)/Head.TexHeight;
				}

			// Finished with MD2 style UVs
			delete [] MD2_UV;



	 // Load frame vertex info
			for(FrameLoop=0;FrameLoop!=nFrames;++FrameLoop)
				{
				 // Get frame conversion data
				 memcpy(&FrameInfo,&data[Head.FrameOffset + (Head.FrameSize * FrameLoop)],sizeof(FrameInfo));

					// Read MD2 style vertex data
     memcpy(vtx,&data[Head.FrameOffset + (Head.FrameSize * FrameLoop) + sizeof(FrameInfo)],nVtx * sizeof(MD2Vtx));

					// Convert vertices
					 for(ItemLoop=0;ItemLoop!=nVtx;++ItemLoop)
							{
							 frame[FrameLoop].Vtx[ItemLoop].x=(vtx[ItemLoop].Vtx[0] * FrameInfo.Scale[0])+FrameInfo.Translate[0];
        frame[FrameLoop].Vtx[ItemLoop].y=(vtx[ItemLoop].Vtx[1] * FrameInfo.Scale[0])+FrameInfo.Translate[1];   
        frame[FrameLoop].Vtx[ItemLoop].z=(vtx[ItemLoop].Vtx[2] * FrameInfo.Scale[0])+FrameInfo.Translate[2];
							}
				}

		// Finished with vtx and filedata
		delete [] vtx;
		delete [] data;


		// Calc normals for each frane
 		for(FrameLoop=0;FrameLoop!=nFrames;FrameLoop++)
				{
						// Calc face normal
				  for(ItemLoop=0;ItemLoop!=nTri;ItemLoop++)
							{
				    CalcNormal(frame[FrameLoop].Vtx[Face[ItemLoop].p1],
						             frame[FrameLoop].Vtx[Face[ItemLoop].p2],
											   					frame[FrameLoop].Vtx[Face[ItemLoop].p3],
														   		&frame[FrameLoop].Norm[ItemLoop]);

							}
				}

  return MD2_OK;
 }


// A few Get() and Set()s to access the private data members
int MD2Obj::GetFrameCount()
{
	return nFrames;
}


char* MD2Obj::GetTexName()
{
	if(TexName)	return TexName;
	else	return NULL;
}


void MD2Obj::SetTexture(GLuint TexNum)
{
	TexID=TexNum;
}


// Draw the specified frame
void MD2Obj::Draw(int Frame)
{
	int Part;

	// Limit frame range
	if(Frame>=nFrames)	Frame=0;

	glBindTexture(GL_TEXTURE_2D,TexID);
  
	for(Part=0;Part<nTri;++Part)
	{
		glBegin(GL_TRIANGLES);
		glNormal3f(frame[Frame].Norm[Part].x,frame[Frame].Norm[Part].y,frame[Frame].Norm[Part].z);
		glTexCoord2f(UV[Face[Part].uv1].u,UV[Face[Part].uv1].v);
		glVertex3f(frame[Frame].Vtx[Face[Part].p1].x,frame[Frame].Vtx[Face[Part].p1].y,frame[Frame].Vtx[Face[Part].p1].z);
		glTexCoord2f(UV[Face[Part].uv2].u,UV[Face[Part].uv2].v);
		glVertex3f(frame[Frame].Vtx[Face[Part].p2].x,frame[Frame].Vtx[Face[Part].p2].y,frame[Frame].Vtx[Face[Part].p2].z);
		glTexCoord2f(UV[Face[Part].uv3].u,UV[Face[Part].uv3].v);
		glVertex3f(frame[Frame].Vtx[Face[Part].p3].x,frame[Frame].Vtx[Face[Part].p3].y,frame[Frame].Vtx[Face[Part].p3].z);
		glEnd();
	}  
 }

void MD2Obj::CalcNormal(Mesh_Vtx v1,Mesh_Vtx v2,Mesh_Vtx v3,Mesh_Vtx* Result)
{
	double v1x,v1y,v1z,v2x,v2y,v2z;
	double nx,ny,nz;
	double vLen;

	// Calculate vectors
	v1x = v1.x - v2.x;
	v1y = v1.y - v2.y;
	v1z = v1.z - v2.z;

	v2x = v2.x - v3.x;
	v2y = v2.y - v3.y;
	v2z = v2.z - v3.z;

	// Get cross product of vectors

	nx = (v1y * v2z) - (v1z * v2y);
	ny = (v1z * v2x) - (v1x * v2z);
	nz = (v1x * v2y) - (v1y * v2x);

	// Normalise final vector
	vLen = sqrt( (nx * nx) + (ny * ny) + (nz * nz) );

	Result->x = (float) (nx / vLen);
	Result->y = (float) (ny / vLen);
	Result->z = (float) (nz / vLen);
}
