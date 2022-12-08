#pragma once
#include <BaseInclude.h>
#include "..\DesignPattern\SingletonPattern\Singleton.h"

#include STRING_INCLUDE_PATH

namespace Core
{
	class LightmappingSetting : public Singleton<LightmappingSetting>
	{
	private:
		ctd::string lightmapNamePrefix = "lightmap_";
		ctd::string maskMapNamePrefix = "maskMap_";

	public:
		static const int32 lightmapSize;
		static const uint32 maxChartNumber;
		static const float maxStrechSize ;
		static const float gutter ;
		static const int32 paddingSize ;	//	������atlas���padding size.
		static const int32 progressivePassCount ;
		//	approximate amount of lightmap pixels per world unit. Affects the amount and resolution of generated lightmaps.
		static float TexelsPerUnit;

		const ctd::string GetLightmapName(int32 lightmapIndex)
		{
			return lightmapNamePrefix + ctd::to_string(lightmapIndex);;
		}

		const ctd::string GetMaskMapName(int32 lightmapIndex)
		{
			return maskMapNamePrefix + ctd::to_string(lightmapIndex);;
		}
	};
}