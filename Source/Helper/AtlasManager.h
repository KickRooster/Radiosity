#pragma once
#include <BaseInclude.h>
#include "..\3rdParty\RectangleBinPack\MaxRectsBinPack.h"
#include "AssetManager.h"

namespace Core
{
	struct Atlas
	{
	public:
		rbp::MaxRectsBinPack binInstance;
		std::unique_ptr<uint8[]> rawData;
		std::unique_ptr<float[]> rawDataFloat;
		ctd::vector<rbp::Rect> rects;
		ctd::vector<Bool> rectAppendeds;
	};

	class AtlasManager
	{
	private:
		std::weak_ptr<AssetManager> m_assetManager;
		const rbp::MaxRectsBinPack::FreeRectChoiceHeuristic heuristic = rbp::MaxRectsBinPack::RectBestShortSideFit; // This can be changed individually even for each rectangle packed.
		const int32 channelCount = 4;

		ctd::vector<std::unique_ptr<Atlas>> m_atlasList;
		int32 m_lastAtlasIndex = -1;
		int32 m_currentAtlasIndex = -1;
		Bool m_atlasChanged = False;

	public:
		AtlasManager(std::shared_ptr<AssetManager> assetManager);
		rbp::Rect Insert(int32 chartWidth, int32 chartHeight);
		int32 GetCurrentLightmapIndex()const;
		Bool AtlasChanged() const;
		Bool IsFirstAtlas() const;
		//void AppendLastAtlasRawData(uint8 * pRawData);
		void AppendLastAtlasRawDataFloat(float *pRawDataFloat);
		void AppendAtlasRawDataFloat(int32 atlasIndex, int32 rectCount, float *pRawDataFloat);
		void SetRawData(int32 index, uint8 * pRawData);
		uint8 * GetCurrentAtlasRawData() const;
		int32 GetAtlasCount() const;
		Atlas * GetAtlas(int32 index) const;
		int32 GetCurrentAtlasIndex() const;
		int32 GetCurrentAtlasRectCount() const;
		void LateUpdate();
		void SaveLightmap();
		void SaveMaskMap();
		void CleanUpRawData();
		void CleanUpAtlasList();
		void ResetRectState();
		~AtlasManager();
	};
}