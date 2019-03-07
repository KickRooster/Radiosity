#pragma once
#include <BaseInclude.h>
#include <GL\glew.h>
#include "..\3rdParty\DearUI\imgui.h"
#include <SDL\SDL_video.h>

namespace Core
{
	static GLuint       g_FontTexture = 0;
	static int          g_ShaderHandle = 0, g_VertHandle = 0, g_FragHandle = 0;
	static int          g_AttribLocationTex = 0, g_AttribLocationProjMtx = 0;
	static int          g_AttribLocationPosition = 0, g_AttribLocationUV = 0, g_AttribLocationColor = 0;
	static unsigned int g_VboHandle = 0, g_ElementsHandle = 0;

	class GUIWrapper
	{
	private:
		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

		static void setClipboardText(void * pUserData, const ANSICHAR* pText);
		static const ANSICHAR * getClipboardText(void* pUserData);
		void newFrame(int32 width, int32 height);
		Bool createDeviceObjects();
		Bool createFontsTexture();
		void renderDrawData(ImDrawData* draw_data);
		void invalidateDeviceObjects();
	public:
		GUIWrapper();
		void Initialize(uint32 width, uint32 height);
		void Tick(uint32 width, uint32 height, const InputState & inputState);
		void Render(uint32 width, uint32 height);
		~GUIWrapper();
	};
}