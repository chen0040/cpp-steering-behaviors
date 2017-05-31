#ifndef _H_GL_MODEL_H
#define _H_GL_MODEL_H

#include "../GLObject/GLObject.h"
#include "../GLTransformation/GLTransformation.h"
#include <vector>
#include <string>
#include <map>
#include <list>

class GLModel 
{
public:
	GLModel(const char* fname);
	virtual ~GLModel();

public:
	virtual void Render(int iFrame)=0;
	virtual void PlaySound(int iCurrentFrame);
	virtual void PreRender();
	virtual void PostRender();

protected:
	bool LoadTexture(const char* texture_name, GLuint texture_id);
private:
	bool LoadConfiguration(const char* fname);

protected:
	std::vector<std::string> m_texture_list;
private:
	void RemoveTextures() { m_texture_list.clear(); }
	void AddTexture(const char* texture_name);

protected:
	int m_iFrameCount;
public:
	int GetFrameCount() const { return m_iFrameCount; }

protected:
	double m_Height2WidthRatio;
public:
	void SetHeight2WidthRatio(double r) { m_Height2WidthRatio=r; }
	double GetHeight2WidthRatio() const { return m_Height2WidthRatio; }

protected:
	std::list<GLTransformation*> m_transformations;
public:
	void clear_transformations();
	void add_transformation(GLTransformation* pTransformation);

protected:
	bool m_bSoundEnabled;
	std::map<std::string, std::pair<std::string, int> > m_sounds;
public:
	void EnableSound(bool bEnabled);

protected:
	std::string m_model_name;
public:
	std::string toString() { return m_model_name; }

protected:
	bool m_bTexture2DEnabled;
	bool m_bAlphaBlend;
};
#endif