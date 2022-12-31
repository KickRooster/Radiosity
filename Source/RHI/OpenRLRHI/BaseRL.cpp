#include "BaseRL.h"

namespace Core
{
	void ErrorCatcher(RLenum error, const void* privateData, size_t privateSize, const char* message, void* userData)
	{
		printf("Error caught:\n%s\nExiting\n", message);

		return;
	}

	RLenum rlCheckError_(const ANSICHAR *file, int32 line)
	{
		RLenum errorCode;

		if ((errorCode = rlGetError()) != RL_NO_ERROR)
		{
			switch (errorCode)
			{
			case RL_INVALID_ENUM:
				errorCode = RL_INVALID_ENUM;
				break;
			case RL_INVALID_VALUE:
				errorCode = RL_INVALID_VALUE;
				break;
			case RL_INVALID_OPERATION:
				errorCode = RL_INVALID_OPERATION;
				break;
			case RL_OUT_OF_MEMORY:
				errorCode = RL_OUT_OF_MEMORY;
				break;
			case RL_INVALID_FRAMEBUFFER_OPERATION:
				errorCode = RL_INVALID_FRAMEBUFFER_OPERATION;
				break;
			case RL_UNIMPLEMENTED:
				errorCode = RL_UNIMPLEMENTED;
				break;
			}
			//std::cout << error << " | " << file << " (" << line << ")" << std::endl;
		}

		return errorCode;
	}

}