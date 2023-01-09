#pragma once
#include "../../IFramework.h"
#include "../../../RHI/OpenGLRHI/OpenGLDevice.h"
#include "../../../RHI/OpenRLRHI/RLDevice.h"
#include "../../../GUI/Button.h"
#include "../../../3rdParty/DearUI/imgui.h"
#include "../../../3rdParty/DearUI/ImGuizmo.h"
#include "../../../GUI/GUIWrapper.h"
#include "../../../RHI/OpenGLRHI/GLFrameBuffer.h"
#include "../../../RHI/OpenRLRHI/RLFrameBuffer.h"
#include "../../../Helper/AssetManager.h"
#include "../../../Settings/LightmappingSetting.h"
#include "../../../Helper/AtlasManager.h"
#include "../../../Asset/Scene.h"
#include "Matrix3x3.h"

#include VECTOR_INCLUDE_PATH


namespace Core
{
	class WindowsEditor : public IFramework
	{
		const ctd::string startSceneName = "default";

		std::shared_ptr<AssetManager> m_assetManager;

		std::unique_ptr<OpenGLDevice> m_GLDevice;
		std::unique_ptr<RLDevice> m_RLDevice;
		std::shared_ptr<Scene> m_scene;
		std::unique_ptr<GUIWrapper> m_guiWrapper;

		std::unique_ptr<GLFrameBuffer> m_GLFrameBuffer;
		std::unique_ptr<GLTexture> m_GLColorAttach;			//	uint8

		std::unique_ptr<GLFrameBuffer> m_GLGBufferFrameBuffer;
		std::unique_ptr<GLTexture> m_GLPositionAttach;
		std::unique_ptr<GLTexture> m_GLNormalAttach;
		std::unique_ptr<GLTexture> m_GLMaskAttach;
		float* m_pMaskRawData;

		std::unique_ptr<RLTexture2D> m_RLBakingObjectPosition;	//	float
		std::unique_ptr<RLTexture2D> m_RLBakingObjectNormal;	//	float

		std::unique_ptr<RLFrameBuffer> m_RLBakeFrameBuffer;
		std::unique_ptr<RLTexture2D> m_RLBakeColorAttach;
		std::unique_ptr<RLBuffer> m_RLBakePackingBuffer;
		std::shared_ptr<GLTexture> m_GLVisibilityTexture;
		std::unique_ptr<RLTexture2D> m_RLHammersleyTexture;
		std::unique_ptr<RLBuffer> m_rlShootingPrimitiveBuffer;

		Bool m_baking;
		float m_thresold;
		float m_currentMaxY;

		//	Visibility Pass
		const int32 PrimitiveIDTextureWidth = 1024;
		const int32 PrimitiveIDTextureHeight = 1024;
		std::unique_ptr<GLFrameBuffer> m_visibilityPassFrameBuffer;
		std::shared_ptr<GLTexture> m_primitiveIDCubeMap;

		//	Reconstrucsion Pass
		std::unique_ptr<GLFrameBuffer> m_reconstructionPassFrameBuffer;
		std::unique_ptr<GLTexture> m_RadiosityTexture;
		std::shared_ptr<GLImageTexture> m_RadiosityImage0;
		std::shared_ptr<GLImageTexture> m_RadiosityImage1;
		std::shared_ptr<GLImageTexture> m_ResidualImage0;
		std::shared_ptr<GLImageTexture> m_ResidualImage1;
		float* m_pRadiosityImageRawData;
		float* m_pResidualImageRawData;

		Matrix3x3 AdobeRGBD65RGBToXYZ;
		Matrix3x3 AdobeRGBD65XYZToRGB;
		
		int32 m_frameCount;
		Bool m_LightmapLoadFromDisk;
		
		Object * m_pSelectedObject;

		//	Editor Builtin Resources
		std::shared_ptr<Material> m_arealLightMaterial;
		std::shared_ptr<Material> m_DrawGBufferMaterial;
		std::shared_ptr<Material> m_DrawIDMaterial;
		std::shared_ptr<Material> m_ComputeFormFactorMaterial;
		std::shared_ptr<Material> m_PickShooterMaterial;
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
		std::shared_ptr<Object> InstantiateObject(std::shared_ptr<Object> object);
		std::shared_ptr<Object> InstantiateAreaLight(std::shared_ptr<Object> object);
		std::shared_ptr<Object> createAreaLight(int32 Index);
		std::unique_ptr<Object> CreateObject(const Primitive& Primitive);
		void SaveLightmap(std::string Name, float* RadiosityImageRawData, int32 Width, int32 Height);
		
		uint32 ReverseBits(uint32 Value);
		Vector2 Hammersley(uint32 Index, uint32 NumSamples);
		Vector3 UniformSampleHemisphere(float u, float v);
		const int32 SamplerCount = 512;
		
		CubeMatrices CubeMatrices;
		
	public:
		WindowsEditor();
		virtual void Initialize(int32 width, int32 height) override;
		virtual void Tick(float deltaTime, int32 width, int32 height, InputState & inputState) override;
		virtual void Render(int32 width, int32 height) override;
		virtual void Bake() override;
		virtual ~WindowsEditor();
	};
}