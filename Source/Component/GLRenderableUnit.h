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
		std::weak_ptr<Material> DrawGBufferMaterial;
		std::weak_ptr<Material> DrawIDMaterial;
		std::weak_ptr<Material> ComputeFormFactorMaterial;
		std::weak_ptr<Material> PickShooterMaterial;

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
		void ActivePickShooter();
		void InactivePickShooter();
		~GLRenderableUnit();
	};
}