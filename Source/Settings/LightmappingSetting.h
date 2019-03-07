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
		static const float uvScale;
		static const int32 paddingSize ;	//	包裹在atlas外的padding size.
		static const int32 progressivePassCount ;

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