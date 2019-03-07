/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright(c) 2006-2013 Caustic Graphics, Inc.    All rights reserved.        *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef __OPENRL_H__
#define __OPENRL_H__

#include <OpenRL/rlplatform.h>
#include <OpenRL/rl.h>
#include <stddef.h>

typedef void * OpenRLContext;
typedef RLintptr OpenRLContextAttribute;
typedef void (*OpenRLNotify)(
	RLenum error,
	const void * private_data,
	size_t private_size,
	const char * message,
	void * user_data);
	
/* OpenRL context creation attributes */
enum {
	kOpenRL_RequireHardwareAcceleration = 1,
	kOpenRL_ExcludeCPUCores = 2,
	kOpenRL_DisableHyperthreading = 3,
	kOpenRL_DisableStats = 4,
	kOpenRL_DisableProfiling = 5,
	kOpenRL_DisableTokenStream = 6,
    kOpenRL_EnableRayPrefixShaders = 10 // External software (such as our Maya plugin) relies on the exact values of the
                                        // private attributes, these cannot be re-ordered.
};

#define RL_CONTEXT_STOPPED_CST 0x0580

#ifdef __cplusplus
extern "C" {
#endif
	
RL_APICALL OpenRLContext OpenRLCreateContext(
	const OpenRLContextAttribute * attributes,
	OpenRLNotify notify,
	void * user_data);
RL_APICALL void OpenRLStopContext(OpenRLContext ctx);
RL_APICALL int OpenRLDestroyContext(OpenRLContext ctx);
RL_APICALL int OpenRLSetCurrentContext(OpenRLContext ctx);
RL_APICALL OpenRLContext OpenRLGetCurrentContext();

RL_APICALL unsigned int OpenRLGetHardwareAcceleratorCount();

RL_APICALL void OpenRLLogMessagesToStdout(RLenum, const void *, size_t, const char *, void *);
RL_APICALL void OpenRLLogMessagesToStderr(RLenum, const void *, size_t, const char *, void *);

#ifdef __cplusplus
}
#endif
		
#endif
