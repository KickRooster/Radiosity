#pragma once
#include <BaseInclude.h>
#include "..\Asset\IAsset.h"
#include "..\Asset\StaticMesh.h"
#include "..\Asset\Texture.h"
#include "..\Asset\Material.h"
#include "..\Asset\GLSL.h"
#include "..\Asset\Prefab.h"
#include "..\Asset\Scene.h"

#include VECTOR_INCLUDE_PATH
#include MAP_INCLUDE_PATH
#include STRING_INCLUDE_PATH

namespace Core
{
	enum AssetType
	{
		AssetType_None = -1,
		AssetType_Scene,
		AssetType_Prefab,
		AssetType_GLShader,
		AssetType_StaticMesh,
		AssetType_TextureInfo,
		AssetType_Texture,
		AssetType_Lightmap,
		AssetType_Material,
		AssetType_Count,
	};

	class AssetManager
	{
	private:
		const ANSICHAR * sceneFullPath = "C:\\Projects\\Template0\\Assets\\Scenes";
		const ANSICHAR * prefabFullPath = "C:\\Projects\\Template0\\Assets\\Prefabs";
		const ANSICHAR * glShaderFullPath = "C:\\Projects\\Template0\\Assets\\Shaders\\GLSL";
		const ANSICHAR * staticMeshFullPath = "C:\\Projects\\Template0\\Assets\\StaticMeshes";
		const ANSICHAR * textureFullPath = "C:\\Projects\\Template0\\Assets\\Textures";
		const ANSICHAR * lightmapFullPath = "C:\\Projects\\Template0\\Assets\\Lightmaps";
		const ANSICHAR * maskMapFullPath = "C:\\Projects\\Template0\\Assets\\MaskMaps";
		const ANSICHAR * materialFullPath = "C:\\Projects\\Template0\\Assets\\Materials";

		const ANSICHAR * defaultGLVertexShaderName = "default";
		const ANSICHAR * defaultGLFragmentShaderName = "default";

		const ANSICHAR * glBakingVertexShaderName = "bake";
		const ANSICHAR * glBakingFragmentShaderName = "bake";
		const ANSICHAR * bakingMaterialName = "baking";

		const ANSICHAR * defaultAlbedoTextureName = "default";

		//ANSICHAR * projectFullPath = "E:\\Projects\\Template0";

		void processScene(const ANSICHAR * extension, const ANSICHAR * fileName, const ANSICHAR * fileFullPathName);
		void processPrefab(const ANSICHAR * extension, const ANSICHAR * fileName, const ANSICHAR * fileFullPathName);
		void processGLShader(const ANSICHAR * extension, const ANSICHAR * fileName, const ANSICHAR * fileFullPathName);
		void processStaticMesh(const ANSICHAR * extension, const ANSICHAR * fileName, const ANSICHAR * fileFullPathName);
		void processTextureInfo(const ANSICHAR * extension, const ANSICHAR * fileName, const ANSICHAR * fileFullPathName);
		void processTexture(const ANSICHAR * extension, const ANSICHAR * fileName, const ANSICHAR * fileFullPathName);
		void processLightmap(const ANSICHAR * extension, const ANSICHAR * fileName, const ANSICHAR * fileFullPathName);
		void processMaterial(const ANSICHAR * extension, const ANSICHAR * fileName, const ANSICHAR * fileFullPathName);
		void processFolder(AssetType assetType, const ANSICHAR * folderFullPathName, const ANSICHAR * extension0, ...);
		
	public:
		const ANSICHAR * sceneExt = ".scene";
		const ANSICHAR * prefabExt = ".prefab";
		const ANSICHAR * glVertexShaderExt = ".vert";
		const ANSICHAR * glFragmentShaderExt = ".frag";
		const ANSICHAR * staticMeshExt = ".fbx";
		const ANSICHAR * textureTGAExt = ".tga";
		const ANSICHAR * texturePNGExt = ".png";
		const ANSICHAR * textureTIFFExt = ".tif";
		const ANSICHAR * textureInfoExt = ".info";
		const ANSICHAR * materialExt = ".material";
		
		AssetManager();

		ctd::map<ctd::string, std::shared_ptr<Scene>> sceneMap;
		ctd::map<ctd::string, std::shared_ptr<Prefab>> prefabMap;

		ctd::map<ctd::string, std::shared_ptr<GLSL>> glVertexShaderMap;
		ctd::map<ctd::string, std::shared_ptr<GLSL>> glFragmentShaderMap;

		ctd::map<ctd::string, std::shared_ptr<StaticMesh>> staticMeshMap;
		ctd::map<ctd::string, std::shared_ptr<TextureInfo>> textureInfoMap;
		ctd::map<ctd::string, std::shared_ptr<Texture>> textureMap;
		ctd::map<ctd::string, std::shared_ptr<Texture>> lightmapMap;
		ctd::map<ctd::string, std::shared_ptr<Material>> materialMap;
		
		void ScanLightmap();
		void Scan();
		void SaveMaterialToDisk();
		void ReloadGLShader();
		void ReloadTexture();
		void ReloadLightmap();
		void SaveLightmap(const ANSICHAR * name, TextureFormat textureFormat, uint8 * pPixels, int32 width, int32 height);
		
		void CreateScene(const ANSICHAR * pName);
		void CreatePrefab(const ANSICHAR * pName);
		void CreateMaterial(const ANSICHAR * pName);

		void SaveAll();

		~AssetManager();
	};
}