#pragma once
#include <BaseInclude.h>
#include "..\Asset\IAsset.h"
#include "..\Asset\StaticMesh.h"
#include "..\Asset\Texture.h"
#include "..\Asset\Material.h"
#include "..\Asset\GLSL.h"
#include "..\Asset\RLSL.h"
#include "..\Asset\Prefab.h"
#include "..\Asset\Scene.h"
#include "..\Asset\FBX.h"

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
		AssetType_RLShader,
		AssetType_StaticMesh,
		AssetType_TextureInfo,
		AssetType_Texture,
		AssetType_Lightmap,
		AssetType_Material,
		AssetType_FBX,
		AssetType_Count,
	};

	class AssetManager
	{
	private:
		const ANSICHAR * sceneFullPath = "G:\\Projects\\Template0\\Assets\\Scenes";
		const ANSICHAR * prefabFullPath = "G:\\Projects\\Template0\\Assets\\Prefabs";
		const ANSICHAR * glShaderFullPath = "G:\\Projects\\Template0\\Assets\\Shaders\\GLSL";
		const ANSICHAR * rlShaderFullPath = "G:\\Projects\\Template0\\Assets\\Shaders\\RLSL";
		const ANSICHAR * meshFullPath = "G:\\Projects\\Template0\\Assets\\Meshes";
		const ANSICHAR * textureFullPath = "G:\\Projects\\Template0\\Assets\\Textures";
		const ANSICHAR * lightmapFullPath = "G:\\Projects\\Template0\\Assets\\Lightmaps";
		const ANSICHAR * maskMapFullPath = "G:\\Projects\\Template0\\Assets\\MaskMaps";
		const ANSICHAR * materialFullPath = "G:\\Projects\\Template0\\Assets\\Materials";
		const ANSICHAR * fbxFullPath = "G:\\Projects\\Template0\\Assets\\FBXs";

		const ANSICHAR * defaultGLVertexShaderName = "default";
		const ANSICHAR * defaultGLFragmentShaderName = "default";
		const ANSICHAR * defaultRLVertexShaderName = "default";
		const ANSICHAR * defaultRLRayShaderName = "default";

		const ANSICHAR * glBakingVertexShaderName = "bake";
		const ANSICHAR * glBakingFragmentShaderName = "bake";
		const ANSICHAR * bakingMaterialName = "baking";

		const ANSICHAR * defaultAlbedoTextureName = "white";

		//ANSICHAR * projectFullPath = "E:\\Projects\\Template0";

		void processScene(const ANSICHAR * extension, const ANSICHAR * fileName, const ANSICHAR * fileFullPathName);
		void processPrefab(const ANSICHAR * extension, const ANSICHAR * fileName, const ANSICHAR * fileFullPathName);
		void processGLShader(const ANSICHAR * extension, const ANSICHAR * fileName, const ANSICHAR * fileFullPathName);
		void processRLShader(const ANSICHAR * extension, const ANSICHAR * fileName, const ANSICHAR * fileFullPathName);
		void processMesh(const ANSICHAR * extension, const ANSICHAR * fileName, const ANSICHAR * fileFullPathName);
		void processTextureInfo(const ANSICHAR * extension, const ANSICHAR * fileName, const ANSICHAR * fileFullPathName);
		void processTexture(const ANSICHAR * extension, const ANSICHAR * fileName, const ANSICHAR * fileFullPathName);
		void processLightmap(const ANSICHAR * extension, const ANSICHAR * fileName, const ANSICHAR * fileFullPathName);
		void processMaskMap(const ANSICHAR * extension, const ANSICHAR * fileName, const ANSICHAR * fileFullPathName);
		void processMaterial(const ANSICHAR * extension, const ANSICHAR * fileName, const ANSICHAR * fileFullPathName);
		void processFBX(const ANSICHAR * extension, const ANSICHAR * fileName, const ANSICHAR * fileFullPathName);
		void processFolder(AssetType assetType, const ANSICHAR * folderFullPathName, const ANSICHAR * extension0, ...);
		
	public:
		const ANSICHAR * sceneExt = ".scene";
		const ANSICHAR * prefabExt = ".prefab";
		const ANSICHAR * glVertexShaderExt = ".vert";
		const ANSICHAR * glFragmentShaderExt = ".frag";
		const ANSICHAR * rlVertexShaderExt = ".vert";
		const ANSICHAR * rlRayShaderExt = ".ray";
		const ANSICHAR * rlFrameShaderExt = ".frame";
		const ANSICHAR * meshExt = ".fbx";
		const ANSICHAR * textureTGAExt = ".tga";
		const ANSICHAR * texturePNGExt = ".png";
		const ANSICHAR * textureTIFFExt = ".tif";
		const ANSICHAR * textureInfoExt = ".info";
		const ANSICHAR * materialExt = ".material";
		const ANSICHAR * fbxExt = ".fbx";

		AssetManager();

		ctd::map<ctd::string, std::shared_ptr<Scene>> sceneMap;
		ctd::map<ctd::string, std::shared_ptr<Prefab>> prefabMap;

		ctd::map<ctd::string, std::shared_ptr<GLSL>> glVertexShaderMap;
		ctd::map<ctd::string, std::shared_ptr<GLSL>> glFragmentShaderMap;

		ctd::map<ctd::string, std::shared_ptr<RLSL>> rlVertexShaderMap;
		ctd::map<ctd::string, std::shared_ptr<RLSL>> rlRayShaderMap;
		ctd::map<ctd::string, std::shared_ptr<RLSL>> rlFrameShaderMap;

		ctd::map<ctd::string, std::shared_ptr<StaticMesh>> meshMap;
		ctd::map<ctd::string, std::shared_ptr<TextureInfo>> textureInfoMap;
		ctd::map<ctd::string, std::shared_ptr<Texture>> textureMap;
		ctd::map<ctd::string, std::shared_ptr<Texture>> lightmapMap;
		ctd::map<ctd::string, std::shared_ptr<Texture>> maskMapMap;
		ctd::map<ctd::string, std::shared_ptr<Material>> materialMap;
		ctd::map<ctd::string, std::shared_ptr<FBX>> fbxMap;

		void ScanLightmap();
		void Scan();
		void SaveMaterialToDisk();
		void ReloadGLShader();
		void ReloadRLShader();
		void ReloadTexture();
		void ReloadLightmap();
		void ReloadMaskMap();
		void SaveLightmap(const ANSICHAR * name, TextureFormat textureFormat, uint8 * pPixels, int32 width, int32 height);
		void SaveMaskMap(const ANSICHAR * name, TextureFormat textureFormat, uint8 * pPixels, int32 width, int32 height);

		void CreateScene(const ANSICHAR * pName);
		void CreatePrefab(const ANSICHAR * pName);
		void CreateMaterial(const ANSICHAR * pName);

		void SaveAll();

		~AssetManager();
	};
}