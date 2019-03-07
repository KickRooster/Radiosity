#include "AtlasManager.h"
#include "..\Settings\LightmappingSetting.h"
#include "..\Math\MathGlobal.h"

#include VECTOR_INCLUDE_PATH

namespace Core
{
	AtlasManager::AtlasManager(std::shared_ptr<AssetManager> assetManager)
	{
		m_assetManager = assetManager;
	}

	rbp::Rect AtlasManager::Insert(int32 chartWidth, int32 chartHeight)
	{
		Bool inserted = False;
		rbp::Rect packedRect;

		m_currentAtlasIndex = -1;

		for (int32 i = 0; i < m_atlasList.size(); ++i)
		{
			packedRect = m_atlasList[i]->binInstance.Insert(chartWidth, chartHeight, heuristic);

			if (packedRect.height > 0)
			{
				inserted = True;
				m_currentAtlasIndex = i;

				m_atlasList[i]->rects.push_back(packedRect);
				m_atlasList[i]->rectAppendeds.push_back(False);

				break;
			}
		}

		if (inserted)
		{
			if (m_currentAtlasIndex == m_lastAtlasIndex)
				m_atlasChanged = False;
			else
				m_atlasChanged = True;
		}
		else
		{
			int32 atlasWidth = LightmappingSetting::Instance()->lightmapSize;
			int32 atlasHeight = LightmappingSetting::Instance()->lightmapSize;

			std::unique_ptr<Atlas> atlas = std::make_unique<Atlas>();
			atlas->binInstance.Init(atlasWidth, atlasHeight);

			packedRect = atlas->binInstance.Insert(chartWidth, chartHeight, heuristic);

			atlas->rects.push_back(packedRect);
			atlas->rectAppendeds.push_back(False);

			atlas->rawData = std::make_unique<uint8[]>(atlasWidth * atlasHeight * sizeof(uint8) * channelCount);
			atlas->rawDataFloat = std::make_unique<float[]>(atlasWidth * atlasHeight * sizeof(float) * channelCount);

			memset(atlas->rawData.get(), atlasWidth * atlasHeight * sizeof(uint8) * channelCount, 0);
			memset(atlas->rawDataFloat.get(), atlasWidth * atlasHeight * sizeof(float) * channelCount, 0);

			m_atlasList.push_back(std::move(atlas));

			m_atlasChanged = True;

			m_currentAtlasIndex = (int32)(m_atlasList.size()) - 1;
		}

		return packedRect;
	}

	Core::int32 AtlasManager::GetCurrentLightmapIndex() const
	{
		return m_currentAtlasIndex;
	}

	Core::Bool AtlasManager::AtlasChanged() const
	{
		return m_atlasChanged;
	}

	Core::Bool AtlasManager::IsFirstAtlas() const
	{
		return m_atlasList.size() == 1;
	}

	//void AtlasManager::AppendLastAtlasRawData(uint8 * pRawData)
	//{
	//	int32 atlasWidth = LightmappingSetting::Instance()->lightmapSize;
	//	int32 atlasHeight = LightmappingSetting::Instance()->lightmapSize;
	//
	//	for (int32 i = 0; i < m_atlasList[m_lastAtlasIndex]->rects.size(); ++i)
	//	{
	//		if (m_atlasList[m_lastAtlasIndex]->rectAppendeds[i])
	//			continue;
	//
	//		m_atlasList[m_lastAtlasIndex]->rectAppendeds[i] = True;
	//
	//		int32 x = m_atlasList[m_lastAtlasIndex]->rects[i].x;
	//		int32 y = m_atlasList[m_lastAtlasIndex]->rects[i].y;
	//		int32 width = m_atlasList[m_lastAtlasIndex]->rects[i].width;
	//		int32 height = m_atlasList[m_lastAtlasIndex]->rects[i].height;
	//
	//		for (int32 col = y; col < height + y; ++col)
	//		{
	//			for (int32 row = x * channelCount; row < (x + width) * channelCount; ++row)
	//				m_atlasList[m_lastAtlasIndex]->rawData[
	//					(col * atlasWidth) * channelCount + row] |= pRawData[
	//						(col * atlasWidth) * channelCount + row];
	//		}
	//	}
	//	//int32 atlasWidth = LightmappingSetting::Instance()->lightmapSize;
	//	//int32 atlasHeight = LightmappingSetting::Instance()->lightmapSize;
	//	//
	//	//for (int32 i = 0; i < atlasWidth * atlasHeight * channelCount; ++i)
	//	//	m_atlasList[m_lastAtlasIndex]->rawData[i] |= pRawData[i];
	//}

	void AtlasManager::AppendLastAtlasRawDataFloat(float *pRawDataFloat)
	{
		int32 atlasWidth = LightmappingSetting::Instance()->lightmapSize;
		int32 atlasHeight = LightmappingSetting::Instance()->lightmapSize;

		for (int32 i = 0; i < m_atlasList[m_lastAtlasIndex]->rects.size(); ++i)
		{
			if (m_atlasList[m_lastAtlasIndex]->rectAppendeds[i])
				continue;

			m_atlasList[m_lastAtlasIndex]->rectAppendeds[i] = True;

			int32 x = m_atlasList[m_lastAtlasIndex]->rects[i].x;
			int32 y = m_atlasList[m_lastAtlasIndex]->rects[i].y;
			int32 width = m_atlasList[m_lastAtlasIndex]->rects[i].width;
			int32 height = m_atlasList[m_lastAtlasIndex]->rects[i].height;

			for (int32 col = y; col < height + y; ++col)
			{
				for (int32 row = x * channelCount; row < (x + width) * channelCount; ++row)
					m_atlasList[m_lastAtlasIndex]->rawDataFloat[
						(col * atlasWidth) * channelCount + row] += pRawDataFloat[
							(col * atlasWidth) * channelCount + row];

						//memcpy(
						//	m_atlasList[m_lastAtlasIndex]->rawDataFloat.get()
						//	+ (col * atlasWidth + x) * channelCount,
						//	pRawDataFloat
						//	+ (col * atlasWidth + x) * channelCount,
						//	width * sizeof(float) * channelCount);
			}
		}
	}

	void AtlasManager::AppendAtlasRawDataFloat(int32 atlasIndex, int32 rectCount, float *pRawDataFloat)
	{
		int32 atlasWidth = LightmappingSetting::Instance()->lightmapSize;
		int32 atlasHeight = LightmappingSetting::Instance()->lightmapSize;

		for (int32 i = 0; i < rectCount; ++i)
		{
			if (m_atlasList[atlasIndex]->rectAppendeds[i])
				continue;

			m_atlasList[atlasIndex]->rectAppendeds[i] = True;

			int32 x = m_atlasList[atlasIndex]->rects[i].x;
			int32 y = m_atlasList[atlasIndex]->rects[i].y;
			int32 width = m_atlasList[atlasIndex]->rects[i].width;
			int32 height = m_atlasList[atlasIndex]->rects[i].height;

			for (int32 col = y; col < height + y; ++col)
			{
				for (int32 row = x * channelCount; row < (x + width) * channelCount; ++row)
					m_atlasList[atlasIndex]->rawDataFloat[
						(col * atlasWidth) * channelCount + row] += pRawDataFloat[
							(col * atlasWidth) * channelCount + row];

				//memcpy(
				//	m_atlasList[atlasIndex]->rawDataFloat.get()
				//	+ (col * atlasWidth + x) * channelCount,
				//	pRawDataFloat
				//	+ (col * atlasWidth + x) * channelCount,
				//	width * sizeof(float) * channelCount);
			}
		}
	}

	void AtlasManager::SetRawData(int32 index, uint8 * pRawData)
	{
		int32 atlasWidth = LightmappingSetting::Instance()->lightmapSize;
		int32 atlasHeight = LightmappingSetting::Instance()->lightmapSize;

		memcpy(m_atlasList[index]->rawData.get(), pRawData, atlasWidth * atlasHeight * channelCount * sizeof(uint8));
	}

	Core::uint8 * AtlasManager::GetCurrentAtlasRawData() const
	{
		return m_atlasList[m_currentAtlasIndex]->rawData.get();
	}

	int32 AtlasManager::GetAtlasCount() const
	{
		return static_cast<int32>(m_atlasList.size());
	}

	Core::Atlas * AtlasManager::GetAtlas(int32 index) const
	{
		return m_atlasList[index].get();
	}

	Core::int32 AtlasManager::GetCurrentAtlasIndex() const
	{
		return m_currentAtlasIndex;
	}

	Core::int32 AtlasManager::GetCurrentAtlasRectCount() const
	{
		return static_cast<int32>(m_atlasList[m_currentAtlasIndex]->rects.size());
	}

	void AtlasManager::LateUpdate()
	{
		m_lastAtlasIndex = m_currentAtlasIndex;
	}

	void AtlasManager::SaveLightmap()
	{
		int32 atlasWidth = LightmappingSetting::Instance()->lightmapSize;
		int32 atlasHeight = LightmappingSetting::Instance()->lightmapSize;

		for (int32 i = 0; i < m_atlasList.size(); ++i)
		{
			ctd::string lightmapName = LightmappingSetting::Instance()->GetLightmapName(i);

			m_assetManager.lock()->SaveLightmap(
				lightmapName.c_str(),
				channelCount == 4 ? TextureFormat_RGBA32 : TextureFormat_RGB24,
				m_atlasList[i]->rawData.get(),
				atlasWidth,
				atlasHeight);
		}
	}

	void AtlasManager::SaveMaskMap()
	{
		int32 atlasWidth = LightmappingSetting::Instance()->lightmapSize;
		int32 atlasHeight = LightmappingSetting::Instance()->lightmapSize;

		for (int32 i = 0; i < m_atlasList.size(); ++i)
		{
			ctd::string maskMapName = LightmappingSetting::Instance()->GetMaskMapName(i);

			m_assetManager.lock()->SaveMaskMap(
				maskMapName.c_str(),
				channelCount == 4 ? TextureFormat_RGBA32 : TextureFormat_RGB24,
				m_atlasList[i]->rawData.get(),
				atlasWidth,
				atlasHeight);
		}
	}

	void AtlasManager::CleanUpRawData()
	{
		int32 atlasWidth = LightmappingSetting::Instance()->lightmapSize;
		int32 atlasHeight = LightmappingSetting::Instance()->lightmapSize;

		for (int32 i = 0; i < m_atlasList.size(); ++i)
		{
			memset(m_atlasList[i].get()->rawData.get(), 0, atlasWidth * atlasHeight * sizeof(uint8) * channelCount);
			memset(m_atlasList[i].get()->rawDataFloat.get(), 0, atlasWidth * atlasHeight * sizeof(float) * channelCount);
		}
	}

	void AtlasManager::CleanUpAtlasList()
	{
		m_atlasList.clear();
	}

	void AtlasManager::ResetRectState()
	{
		for (int32 i = 0; i < m_atlasList.size(); ++i)
		{
			for (int32 j = 0; j < m_atlasList[i].get()->rectAppendeds.size(); ++j)
				m_atlasList[i].get()->rectAppendeds[j] = False;
		}
	}

	AtlasManager::~AtlasManager()
	{
		m_atlasList.clear();
	}
}