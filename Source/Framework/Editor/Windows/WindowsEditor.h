#pragma once
#include "../../IFramework.h"
#include "../../../RHI/OpenGLRHI/OpenGLDevice.h"
#include "../../../GUI/Button.h"
#include "../../../3rdParty/DearUI/imgui.h"
#include "../../../3rdParty/DearUI/ImGuizmo.h"
#include "../../../GUI/GUIWrapper.h"
#include "../../../RHI/OpenGLRHI/GLFrameBuffer.h"
#include "../../../Helper/AssetManager.h"
#include "../../../Settings/LightmappingSetting.h"
#include "../../../Helper/AtlasManager.h"
#include "../../../Asset/Scene.h"
#include "../../../Math/SH.h"

#include VECTOR_INCLUDE_PATH


namespace Core
{
	class WindowsEditor : public IFramework
	{
	private:
		const ctd::string startSceneName = "default";

		std::shared_ptr<AssetManager> m_assetManager;

		std::unique_ptr<OpenGLDevice> m_GLDevice;
		std::shared_ptr<Scene> m_scene;
		std::unique_ptr<GUIWrapper> m_guiWrapper;

		std::unique_ptr<GLFrameBuffer> m_GLFrameBuffer;
		std::unique_ptr<GLTexture> m_GLColorAttach;			//	uint8

		Bool m_baking;

		//	Visibility Pass
		const int32 PrimitiveIDTextureWidth = 1024;
		const int32 PrimitiveIDTextureHeight = 1024;
		std::unique_ptr<GLFrameBuffer> m_visibilityPassFrameBuffer;
		std::unique_ptr<GLTexture> m_primitiveIDTexture;
		float * m_pPrimitiveIDRawData;

		//	Reconstrucsion Pass
		std::unique_ptr<GLFrameBuffer> m_reconstructionPassFrameBuffer;
		std::unique_ptr<GLTexture> m_RadiorityTexture;
		std::unique_ptr<GLTexture> m_residualTexture;
		//float* m_pRadiosityRawData;
		//float* m_residualRawData;
		
		int32 m_frameCount;
		
		Object * m_pSelectedObject;

		//	Editor Builtin Resources
		std::shared_ptr<Material> m_arealLightMaterial;
		std::shared_ptr<Material> m_DrawIDMaterial;
		std::shared_ptr<StaticMesh> m_areaLightMesh;
		std::shared_ptr<StaticMesh> m_postprocessMesh;

		void createBuiltinResources();

		void panelSceneObjects();
		void popupNamingTick(const ANSICHAR *pName, const ANSICHAR *pExtension, InputState & inputState, Bool & pressedOK, ctd::string & fileName);
		void popupFileSelectingTick(FileType fileType, const ANSICHAR *pName, bool * pfileSelections, int32 & selectedIndex, int32 & selectedIndexLastFrame, InputState & inputState, Bool & pressedOK, ctd::string & fileNameWithExt);
		void panelAssets(InputState & inputState) ;
		void menuBar();
		void guizmoTick(const ImVec2 & regionTopLeft, const ImVec2 & regionSize, void * pViewMatrix, void * pProjectionMatrix, Object * pSelectedObject);
		//	Create immediately.
		std::shared_ptr<Object> createObject(std::weak_ptr<Prefab> prefab);
		//	Create for serialized.
		std::shared_ptr<Object> createObject(std::shared_ptr<Object> object);
		std::shared_ptr<Object> createAreaLight();

	public:
		WindowsEditor();
		virtual void Initialize(int32 width, int32 height) override;
		virtual void Tick(float deltaTime, int32 width, int32 height, InputState & inputState) override;
		virtual void Render(int32 width, int32 height) override;
		virtual void Bake() override;
		virtual ~WindowsEditor();
	};
}