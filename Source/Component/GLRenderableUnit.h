#pragma once
#include <BaseInclude.h>
#include "../Asset/StaticMesh.h"
#include "../Asset/Material.h"

namespace Core
{
	class GLRenderableUnit
	{
	public:
		std::shared_ptr<StaticMesh> staticMesh;
		std::weak_ptr<Material> material;
		std::weak_ptr<Material> DrawGBufferMaterial;
		std::weak_ptr<Material> DrawIDMaterial;
		std::weak_ptr<Material> ComputeFormFactorMaterial;

		GLRenderableUnit();
		void BeginUse();
		void Activate();
		void Inactivate();
		void ActiveDrawGBuffer();
		void InactiveDrawGBuffer();
		void ActivateDrawingID();
		void InactivateDrawingID();
		void ActiveComputingFormFactor();
		void InactiveComputingFormFactor();
		~GLRenderableUnit();
	};
}