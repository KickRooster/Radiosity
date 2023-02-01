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

#include <vector>
#include <map>
#include <string>

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
		AssetType_Count,
	};

	class AssetManager
	{
	private:
		const ANSICHAR * sceneFullPath = ".\\Assets\\Scenes";
		const ANSICHAR * prefabFullPath = ".\\Assets\\Prefabs";
		const ANSICHAR * glShaderFullPath = ".\\Assets\\Shaders\\GLSL";
		const ANSICHAR * rlShaderFullPath = ".\\Assets\\Shaders\\RLSL";
		const ANSICHAR * staticMeshFullPath = ".\\Assets\\StaticMeshes";
		const ANSICHAR * textureFullPath = ".\\Assets\\Textures";
		const ANSICHAR * lightmapFullPath = ".\\Assets\\Lightmaps";
		const ANSICHAR * materialFullPath = ".\\Assets\\Materials";

		const ANSICHAR * defaultGLVertexShaderName = "default";
		const ANSICHAR * defaultGLFragmentShaderName = "default";
		const ANSICHAR * defaultRLVertexShaderName = "default";
		const ANSICHAR * defaultRLRayShaderName = "default";
		
		const ANSICHAR * defaultAlbedoTextureName = "default";

		void processScene(const ANSICHAR * extension, const ANSICHAR * fileName, const ANSICHAR * fileFullPathName);
		void processPrefab(const ANSICHAR * extension, const ANSICHAR * fileName, const ANSICHAR * fileFullPathName);
		void processGLShader(const ANSICHAR * extension, const ANSICHAR * fileName, const ANSICHAR * fileFullPathName);
		void processRLShader(const ANSICHAR * extension, const ANSICHAR * fileName, const ANSICHAR * fileFullPathName);
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
		const ANSICHAR * rlVertexShaderExt = ".vert";
		const ANSICHAR * rlRayShaderExt = ".ray";
		const ANSICHAR * rlFrameShaderExt = ".frame";
		const ANSICHAR * staticMeshExt = ".fbx";
		const ANSICHAR * textureTGAExt = ".tga";
		const ANSICHAR * texturePNGExt = ".png";
		const ANSICHAR * textureTIFFExt = ".tif";
		const ANSICHAR * textureInfoExt = ".info";
		const ANSICHAR * materialExt = ".material";
		
		AssetManager();

		std::map<std::string, std::shared_ptr<Scene>> sceneMap;
		std::map<std::string, std::shared_ptr<Prefab>> prefabMap;

		std::map<std::string, std::shared_ptr<GLSL>> glVertexShaderMap;
		std::map<std::string, std::shared_ptr<GLSL>> glFragmentShaderMap;

		std::map<std::string, std::shared_ptr<RLSL>> rlVertexShaderMap;
		std::map<std::string, std::shared_ptr<RLSL>> rlRayShaderMap;
		std::map<std::string, std::shared_ptr<RLSL>> rlFrameShaderMap;

		std::map<std::string, std::shared_ptr<StaticMesh>> staticMeshMap;
		std::map<std::string, std::shared_ptr<TextureInfo>> textureInfoMap;
		std::map<std::string, std::shared_ptr<Texture>> textureMap;
		std::map<std::string, std::shared_ptr<Texture>> lightmapMap;
		std::map<std::string, std::shared_ptr<Material>> materialMap;
		
		void ScanLightmap();
		void Scan();
		void SaveMaterialToDisk();
		void ReloadGLShader();
		void ReloadRLShader();
		void ReloadTexture();
		void ReloadLightmap();
		void SaveLightmap(const ANSICHAR * name, TextureFormat textureFormat, uint8 * pPixels, int32 width, int32 height);
		
		std::shared_ptr<Scene> CreateScene(const ANSICHAR * pName);
		void CreatePrefab(const ANSICHAR * pName);
		void CreateMaterial(const ANSICHAR * pName);

		void SaveAll();

		~AssetManager();
	};
}