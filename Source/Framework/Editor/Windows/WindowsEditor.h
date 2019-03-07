#pragma once
#include "../../IFramework.h"
#include "../../../RHI/OpenGLRHI/OpenGLDevice.h"
#include "../../../GUI/Button.h"
#include "../../../3rdParty/DearUI/imgui.h"
#include "../../../3rdParty/DearUI/ImGuizmo.h"
#include "../../../GUI/GUIWrapper.h"
#include "../../../RHI/OpenGLRHI/GLFrameBuffer.h"
#include "../../../RHI/OpenRLRHI/RLDevice.h"
#include "../../../RHI/OpenRLRHI/RLFrameBuffer.h"
#include "../../../RHI/OpenRLRHI/RLTexture2D.h"
#include "../../../RHI/OpenRLRHI/RLBuffer.h"
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
		struct GI
		{
			RLtexture texture; // Full of random values for samplinging the hemisphere on a diffuse bounce.
			int32 enabled;       // Enable/disable global illumination.
		};

		struct BakingKeyFrameInfo
		{
			Bool atlasChanged;
			Bool isFirstAtlas;
			int32 atlasIndex;
			int32 rectCount;
		};

		const ctd::string startSceneName = "21century";

		std::shared_ptr<AssetManager> m_assetManager;

		std::unique_ptr<OpenGLDevice> m_GLDevice;
		std::unique_ptr<RLDevice> m_RLDevice;
		std::shared_ptr<Scene> m_scene;
		std::unique_ptr<GUIWrapper> m_guiWrapper;

		std::unique_ptr<GLFrameBuffer> m_GLFrameBuffer;
		std::unique_ptr<GLTexture> m_GLColorAttach;			//	uint8
		std::unique_ptr<RLFrameBuffer> m_RLFrameBuffer;
		std::unique_ptr<RLTexture2D> m_RLColorAttach;		//	float	这个不用浮点类型,Profiler.exe抓到的color buffer的r和g会调换顺序.
		std::unique_ptr<RLBuffer> m_RLPackingBuffer;
		std::unique_ptr<GLTexture> m_GLRayTracedViewTexture;//	float

		Bool m_baking;
		const int32 glBakingBufferSize = 1024;
		std::unique_ptr<GLFrameBuffer> m_GLBakeFrameBuffer;
		std::unique_ptr<GLTexture> m_GLBakeColorAttach0;		//	float
		std::unique_ptr<GLTexture> m_GLBakeColorAttach1;		//	float

		std::unique_ptr<RLTexture2D> m_RLBakingObjectPosition;	//	float
		std::unique_ptr<RLTexture2D> m_RLBakingObjectNormal;	//	float

		std::unique_ptr<RLFrameBuffer> m_RLBakeFrameBuffer;
		std::unique_ptr<RLTexture2D> m_RLBakeColorAttach;		//	float
		std::unique_ptr<RLBuffer> m_RLBakePackingBuffer;
		std::unique_ptr<GLTexture> m_GLBakeViewTexture;			//	uint8

		std::unique_ptr<RLTexture2D> m_RLRandomValuesTexture;	//	float
		std::unique_ptr<RLTexture2D> m_RLJitterTexture;			//	float

		std::unique_ptr<GLFrameBuffer> m_GLSH4FrameBuffer;
		std::shared_ptr<GLTexture> m_GLSH4ColorAttach0;		//	float
		std::shared_ptr<GLTexture> m_GLSH4ColorAttach1;		//	float
		std::shared_ptr<GLTexture> m_GLSH4ColorAttach2;		//	float
		std::shared_ptr<GLTexture> m_GLSH4ColorAttach3;		//	float

		GI * m_GIUniformBufferData;
		std::shared_ptr<RLBuffer> m_rlGIUniformBuffer;
		ctd::vector<Vector4> m_jitters;
		int32 m_frameCount;
		int32 m_progressiveCountPerFrame;
		Bool m_isRenderingRL;
		Bool m_GIEnabled;

		std::unique_ptr<GLTexture> m_GLBakingAccumulation;					//	float
		std::unique_ptr<GLFrameBuffer> m_GLPostprocessFrameBuffer;
		std::unique_ptr<GLTexture> m_GLPostprocessColorAttach;				//	uint8
		std::unique_ptr<GLFrameBuffer> m_GLBakingPostprocessFrameBuffer;
		std::unique_ptr<GLTexture> m_GLBakingPostprocessColorAttach;		//	uint8
		std::unique_ptr<GLFrameBuffer> m_GLDilationFrameBuffer;
		std::unique_ptr<GLTexture> m_GLDilationColorAttach;		//	uint8
		std::unique_ptr<GLBuffer> m_GLPackingBuffer;

		int32 debugingProgressiveCount;

		const int32 bakingRTSize = LightmappingSetting::Instance()->lightmapSize;

		std::unique_ptr<AtlasManager> m_altasManager;

		float * m_pPositionRawData;
		float * m_pNormalRawData;

		Object * m_pSelectedObject;

		//	Editor Builtin Resources
		std::shared_ptr<Material> m_directionalLightMaterial;
		std::shared_ptr<Material> m_terrainMaterial;
		std::shared_ptr<Material> m_bakingMaterial;
		std::shared_ptr<Material> m_postprocessAverageMaterial;
		std::shared_ptr<Material> m_postprocessDilationMaterial;
		std::shared_ptr<Material> m_postprocessSH4Material;
		std::shared_ptr<StaticMesh> m_directionLightMesh;
		std::shared_ptr<StaticMesh> m_terrainMesh;
		std::shared_ptr<StaticMesh> m_postprocessMesh;
		std::shared_ptr<Object> m_postprocessAverageObject;

		void createBuiltinResources();

		void panelSceneObjects();
		void popupNamingTick(const ANSICHAR *pName, const ANSICHAR *pExtension, InputState & inputState, Bool & pressedOK, ctd::string & fileName);
		void popupFileSelectingTick(FileType fileType, const ANSICHAR *pName, bool * pfileSelections, int32 & selectedIndex, int32 & selectedIndexLastFrame, InputState & inputState, Bool & pressedOK, ctd::string & fileNameWithExt);
		void panelAssets(InputState & inputState) ;
		void menuBar();
		void guizmoTick(const ImVec2 & regionTopLeft, const ImVec2 & regionSize, void * pViewMatrix, void * pProjectionMatrix, Object * pSelectedObject);
		std::shared_ptr<Object> createObject(std::weak_ptr<Prefab> prefab);
		std::shared_ptr<Object> createObject(std::shared_ptr<Object> object);
		std::shared_ptr<Object> createPostprocessAverageObject();
		std::shared_ptr<Object> createDirectinalLight();
		std::shared_ptr<Object> createTerrain();

	public:
		WindowsEditor();
		virtual void Initialize(int32 width, int32 height) override;
		virtual void Tick(float deltaTime, int32 width, int32 height, InputState & inputState) override;
		virtual void Render(int32 width, int32 height) override;
		virtual void Bake() override;
		virtual ~WindowsEditor();
	};
}