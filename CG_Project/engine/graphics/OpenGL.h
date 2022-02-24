#pragma once
#include <GL/glew.h>
#include <assert.h>

namespace GL
{
	/*
	*  Initialize GLEW
	*
	*  in the first call
	*  returns true only if glewInit() retruns GLEW_OK.
	* 
	*  then it returns false
	*/
	bool InitOpenGL();

	void glClearError(); // if we don't have valid opengl contex it will loop forever.
	bool glCheckError(); // if we don't have valid opengl contex it will loop forever.
}



#ifdef _DEBUG
#define glCall(x) GL::glClearError(); x; assert(GL::glCheckError() == false)
#else
#define glCall(x) x
#endif