#pragma once
#include <BaseInclude.h>
#include "../Asset/StaticMesh.h"
#include "../Asset/Material.h"

namespace Core
{
	class GLRenderableUnit
	{
	public:
		std::weak_ptr<StaticMesh> staticMesh;
		std::weak_ptr<Material> material;
		std::weak_ptr<Material> DrawIDMaterial;
		std::weak_ptr<Material> ComputeFormFactorMaterial;
		std::weak_ptr<Material> ViewCubeMapMaterial;

		GLRenderableUnit();
		void BeginUse();
		void Activate();
		void Inactivate();
		void ActivateDrawingID();
		void InactivateDrawingID();
		void ActiveComputingFormFactor();
		void InactiveComputingFormFactor();
		void ActiveViewCubeMap();
		void InactiveCubeMap();
		~GLRenderableUnit();
	};
}