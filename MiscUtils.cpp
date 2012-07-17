
#include "MiscUtils.h"
#include <GL\glew.h>

void GLError(unsigned int error)
{
	switch (error)
	{
	case GL_INVALID_ENUM:
		printf("GL_INVALID_ENUM");
		break;
	case GL_INVALID_OPERATION:
		printf("GL_INVALID_OPERATION");
		break;
	case GL_INVALID_VALUE:
		printf("GL_INVALID_VALUE");
		break;
	case GL_STACK_UNDERFLOW:
		printf("GL_STACK_UNDERFLOW");
		break;
	case GL_STACK_OVERFLOW:
		printf("GL_STACK_OVERFLOW");
		break;
	case GL_OUT_OF_MEMORY:
		printf("GL_OUT_OF_MEMORY");
		break;
	}
}