#include "BaseGL.h"

namespace Core
{
	GLenum glCheckError_(const ANSICHAR *file, int32 line)
	{
		//	ע�͵���,�������ٴ�.��Ϊ̫��ʱ.
		//return GL_NO_ERROR;

		GLenum errorCode;

		if ((errorCode = glGetError()) != GL_NO_ERROR)
		{
			switch (errorCode)
			{
			case GL_INVALID_ENUM:
				errorCode = GL_INVALID_ENUM;
				break;
			case GL_INVALID_VALUE:
				errorCode = GL_INVALID_VALUE;
				break;
			case GL_INVALID_OPERATION:
				errorCode = GL_INVALID_OPERATION;
				break;
			case GL_STACK_OVERFLOW:
				errorCode = GL_STACK_OVERFLOW;
				break;
			case GL_STACK_UNDERFLOW:
				errorCode = GL_STACK_UNDERFLOW;
				break;
			case GL_OUT_OF_MEMORY:
				errorCode = GL_OUT_OF_MEMORY;
				break;
			case GL_INVALID_FRAMEBUFFER_OPERATION:
				errorCode = GL_INVALID_FRAMEBUFFER_OPERATION;
				break;
			}

			//std::cout << error << " | " << file << " (" << line << ")" << std::endl;
		}

		return errorCode;
	}
}