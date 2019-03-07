#include <stdlib.h>
#include <stdio.h>
#include <BaseInclude.h>

#ifdef PLATFORM_WINDOWS

//#define WIN32_LEAN_AND_MEAN
//#include <windows.h>
#include "Framework/Editor/Windows/WindowsEditor.h"
#include <GL/glew.h>
#include <SDL/SDL.h>
#include <SDL/SDL_syswm.h>

using namespace Core;

// 全局变量: 
HWND g_hwnd;
bool         g_MousePressed[3] = { false, false, false };
SDL_Cursor*  g_MouseCursors[ImGuiMouseCursor_::ImGuiMouseCursor_Count_] = { 0 };
SDL_Window * pSDLWindow = nullptr;

InputState g_inputState;
WindowsState g_windowState;

#define WINDOW_POSITION_X 320
#define WINDOW_POSITION_Y 40
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 1000

uint32 g_frameCount = 0;
uint32 g_tickCountPrev = 0;

Bool CreateOpenGLContext(HDC hdc)
{
	PIXELFORMATDESCRIPTOR	pixelFormatDesc;
	int32						pixelFormat;
	Bool					result = false;

	// Ensure the preview box DC uses ARGB pixel format
	ZeroMemory(&pixelFormatDesc, sizeof(pixelFormatDesc));
	pixelFormatDesc.nSize = sizeof(pixelFormatDesc);
	pixelFormatDesc.nVersion = 1;
	pixelFormatDesc.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pixelFormatDesc.iPixelType = PFD_TYPE_RGBA;
	pixelFormatDesc.cColorBits = 32;
	pixelFormatDesc.cDepthBits = 24;
	pixelFormatDesc.cAlphaBits = 8;
	pixelFormatDesc.iLayerType = PFD_MAIN_PLANE;
	pixelFormat = ChoosePixelFormat(hdc, &pixelFormatDesc);

	if (!SetPixelFormat(hdc, pixelFormat, &pixelFormatDesc))
		return False;

	// Create OpenGL rendering context
	HGLRC g_openGLctx = wglCreateContext(hdc);

	if (g_openGLctx == Null)
		return False;

	// Make the new OpenGL context the current rendering context so
	// we can initialise the DeckLink preview helper
	if (!wglMakeCurrent(hdc, g_openGLctx))
		return False;

	// Reset the OpenGL rendering context
	//wglMakeCurrent(Null, Null);

	return result;
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	SDL_Init(SDL_INIT_VIDEO);

	SDL_GL_ResetAttributes();
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	int flag = 0;
	flag |= SDL_GL_CONTEXT_DEBUG_FLAG;

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, flag);

	SDL_GLContext sdlGLContext = nullptr;

	HGLRC lastRc = wglGetCurrentContext();

	int flags = SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;

	g_windowState.xPos = WINDOW_POSITION_X;
	g_windowState.yPos = WINDOW_POSITION_Y;
	g_windowState.width = WINDOW_WIDTH;
	g_windowState.height = WINDOW_HEIGHT;
	g_windowState.titleName = "T";

	pSDLWindow = SDL_CreateWindow(
		g_windowState.titleName,
		g_windowState.xPos,
		g_windowState.yPos,
		g_windowState.width,
		g_windowState.height,
		flags);

	SDL_SysWMinfo info;
	SDL_VERSION(&info.version);

	SDL_GetWindowWMInfo(pSDLWindow, &info);

	g_hwnd = info.info.win.window;

	const char *subsystem = "an unknown system!";

	switch (info.subsystem) {
	case SDL_SYSWM_UNKNOWN:   break;
	case SDL_SYSWM_WINDOWS:   subsystem = "Microsoft Windows(TM)";  break;
	case SDL_SYSWM_X11:       subsystem = "X Window System";        break;
#if SDL_VERSION_ATLEAST(2, 0, 3)
	case SDL_SYSWM_WINRT:     subsystem = "WinRT";                  break;
#endif
	case SDL_SYSWM_DIRECTFB:  subsystem = "DirectFB";               break;
	case SDL_SYSWM_COCOA:     subsystem = "Apple OS X";             break;
	case SDL_SYSWM_UIKIT:     subsystem = "UIKit";                  break;
#if SDL_VERSION_ATLEAST(2, 0, 2)
	case SDL_SYSWM_WAYLAND:   subsystem = "Wayland";                break;
	case SDL_SYSWM_MIR:       subsystem = "Mir";                    break;
#endif
#if SDL_VERSION_ATLEAST(2, 0, 4)
	case SDL_SYSWM_ANDROID:   subsystem = "Android";                break;
#endif
#if SDL_VERSION_ATLEAST(2, 0, 5)
	case SDL_SYSWM_VIVANTE:   subsystem = "Vivante";                break;
#endif
	}

	sdlGLContext = SDL_GL_CreateContext(pSDLWindow);

	SDL_GL_MakeCurrent(pSDLWindow, sdlGLContext);

	SDL_GL_SetSwapInterval(0);

	const unsigned char* keymap = SDL_GetKeyboardState(nullptr);

	//	Initialization
	GLenum result = glewInit();
	IFramework * pFramework = new WindowsEditor();

	pFramework->Initialize(g_windowState.width, g_windowState.height);

	while (1)
	{
		uint32 tickCount = SDL_GetTicks();

		if (g_frameCount == 0)
		{
			//	第一帧
			g_tickCountPrev = tickCount;
		}

		float deltaTime = (tickCount - g_tickCountPrev) * 0.001f;

		g_tickCountPrev = tickCount;

		++g_frameCount;

		SDL_Event e;
		int32 key;

		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
				goto end;

			ImGuiIO& io = ImGui::GetIO();

			int32 button = 0;

			//////////////////////////////////////////////////////////////////////////
			switch (e.type)
			{
			case SDL_MOUSEMOTION:
				io.MousePos.x = static_cast<float>(e.motion.x);
				io.MousePos.y = static_cast<float>(e.motion.y);

				g_inputState.xPos = e.motion.x;
				g_inputState.yPos = e.motion.y;
				break;
			case SDL_MOUSEWHEEL:
				if (e.wheel.x > 0) io.MouseWheelH += 1;
				if (e.wheel.x < 0) io.MouseWheelH -= 1;
				if (e.wheel.y > 0) io.MouseWheel += 1;
				if (e.wheel.y < 0) io.MouseWheel -= 1;

				g_inputState.zPos += e.wheel.y > 0 ? 1 : -1;
				g_inputState.zPos = ClampToLeft(g_inputState.zPos, 1);
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (e.button.button == SDL_BUTTON_LEFT) g_MousePressed[0] = true;
				if (e.button.button == SDL_BUTTON_RIGHT) g_MousePressed[1] = true;
				if (e.button.button == SDL_BUTTON_MIDDLE) g_MousePressed[2] = true;

				if (e.button.button == 1)
					g_inputState.leftBuffonDown = True;
				if (e.button.button == 2)
					g_inputState.middleBuffonDown = True;
				if (e.button.button == 3)
					g_inputState.rightBuffonDown = True;
				break;
			case SDL_MOUSEBUTTONUP:
				if (e.button.button == 1) button = 0;
				if (e.button.button == 2) button = 1;
				if (e.button.button == 3) button = 2;

				if (e.button.button == 1)
					g_inputState.leftBuffonDown = False;

				if (e.button.button == 2)
					g_inputState.middleBuffonDown = False;

				if (e.button.button == 3)
					g_inputState.rightBuffonDown = False;
				break;
			case SDL_KEYDOWN:
			case SDL_KEYUP:
				key = e.key.keysym.scancode;
				IM_ASSERT(key >= 0 && key < IM_ARRAYSIZE(io.KeysDown));
				io.KeysDown[key] = (e.type == SDL_KEYDOWN);
				io.KeyShift = ((SDL_GetModState() & KMOD_SHIFT) != 0);
				io.KeyCtrl = ((SDL_GetModState() & KMOD_CTRL) != 0);
				io.KeyAlt = ((SDL_GetModState() & KMOD_ALT) != 0);
				io.KeySuper = ((SDL_GetModState() & KMOD_GUI) != 0);

				if (e.key.keysym.scancode == SDL_SCANCODE_W)
					g_inputState.wDown = e.type == SDL_KEYDOWN;
				else if (e.key.keysym.scancode == SDL_SCANCODE_S)
					g_inputState.sDown = e.type == SDL_KEYDOWN;
				else if (e.key.keysym.scancode == SDL_SCANCODE_A)
					g_inputState.aDown = e.type == SDL_KEYDOWN;
				else if (e.key.keysym.scancode == SDL_SCANCODE_D)
					g_inputState.dDown = e.type == SDL_KEYDOWN;
				else if (e.key.keysym.scancode == SDL_SCANCODE_Q)
					g_inputState.qDown = e.type == SDL_KEYDOWN;
				else if (e.key.keysym.scancode == SDL_SCANCODE_E)
					g_inputState.eDown = e.type == SDL_KEYDOWN;
				else if (e.key.keysym.scancode == SDL_SCANCODE_LSHIFT)
					g_inputState.leftShiftDown = e.type == SDL_KEYDOWN;

				break;
			case SDL_WINDOWEVENT_SIZE_CHANGED:
				g_windowState.width = e.window.data1;
				g_windowState.height = e.window.data2;
				break;
			case SDL_TEXTINPUT:
				io.AddInputCharactersUTF8(e.text.text);
				break;
			case SDL_WINDOWEVENT_CLOSE:
				break;
			}
		}

		pFramework->Tick(deltaTime, g_windowState.width, g_windowState.height, g_inputState);
		pFramework->Render(g_windowState.width, g_windowState.height);
		pFramework->Bake();

		SDL_GL_SwapWindow(pSDLWindow);
	}

end:

	wglMakeCurrent(NULL, NULL);
	SDL_Quit();

	//delete pFramework;

	return 1;
}
#endif