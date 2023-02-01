#pragma once
#include <BaseInclude.h>
#include <../3rdParty/cereal/archives/xml.hpp>
#include "IAsset.h"

#include <string>

namespace Core
{
	class Prefab : public IAsset
	{
	public:
		std::string staticMeshName;
		std::string materialName;

		virtual void BeginUse() override;
		virtual void Reload() override;
		virtual void BeforeSave() override;
		virtual void AfterLoad() override;

		template <class Archive>
		void serialize(Archive & ar)
		{
			ar(
				staticMeshName,
				materialName
			);
		}
	};
}