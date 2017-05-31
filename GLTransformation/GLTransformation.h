#ifndef _H_GL_TRANSFORMATION_H
#define _H_GL_TRANSFORMATION_H

class GLTransformation
{
public:
	GLTransformation() { }
	virtual ~GLTransformation() { }

	virtual void apply()=0;
};
#endif