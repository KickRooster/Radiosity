#include "GLRenderableUnit.h"

namespace Core
{
	GLRenderableUnit::GLRenderableUnit()
	{
	}

	void GLRenderableUnit::BeginUse()
	{
		if (!staticMesh.expired())
			staticMesh.lock()->BeginUse();

		if (!material.expired())
			material.lock()->BeginUse();

		if (!DrawGBufferMaterial.expired())
			DrawGBufferMaterial.lock()->BeginUse();

		if (!DrawIDMaterial.expired())
			DrawIDMaterial.lock()->BeginUse();

		if (!ComputeFormFactorMaterial.expired())
			ComputeFormFactorMaterial.lock()->BeginUse();

		if (!ViewCubeMapMaterial.expired())
		{
			ViewCubeMapMaterial.lock()->BeginUse();	
		}

		if (!PickShooterMaterial.expired())
		{
			PickShooterMaterial.lock()->BeginUse();
		}
	}

	void GLRenderableUnit::Activate()
	{
		if (!staticMesh.expired())
			staticMesh.lock()->Activate();

		if (!material.expired())
			material.lock()->Activate();
	}

	void GLRenderableUnit::Inactivate()
	{
		if (!staticMesh.expired())
			staticMesh.lock()->Inactivate();

		if (!material.expired())
			material.lock()->Inactivate();
	}

	void GLRenderableUnit::ActiveDrawGBuffer()
	{
		if (!staticMesh.expired())
			staticMesh.lock()->Activate();

		if (!DrawGBufferMaterial.expired())
			DrawGBufferMaterial.lock()->Activate();
	}

	void GLRenderableUnit::InactiveDrawGBuffer()
	{
		if (!staticMesh.expired())
			staticMesh.lock()->Inactivate();

		if (!DrawGBufferMaterial.expired())
			DrawGBufferMaterial.lock()->Inactivate();
	}
	
	void GLRenderableUnit::ActivateDrawingID()
	{
		if (!staticMesh.expired())
			staticMesh.lock()->Activate();

		if (!DrawIDMaterial.expired())
			DrawIDMaterial.lock()->Activate();
	}

	void GLRenderableUnit::InactivateDrawingID()
	{
		if (!staticMesh.expired())
			staticMesh.lock()->Inactivate();

		if (!DrawIDMaterial.expired())
			DrawIDMaterial.lock()->Inactivate();
	}
	
	void GLRenderableUnit::ActiveComputingFormFactor()
	{
		if (!staticMesh.expired())
			staticMesh.lock()->Activate();

		if (!ComputeFormFactorMaterial.expired())
			ComputeFormFactorMaterial.lock()->Activate();
	}

	void GLRenderableUnit::InactiveComputingFormFactor()
	{
		if (!staticMesh.expired())
			staticMesh.lock()->Inactivate();

		if (!ComputeFormFactorMaterial.expired())
			ComputeFormFactorMaterial.lock()->Inactivate();
	}

	void GLRenderableUnit::ActiveViewCubeMap()
	{
		if (!staticMesh.expired())
			staticMesh.lock()->Activate();

		if (!ViewCubeMapMaterial.expired())
			ViewCubeMapMaterial.lock()->Activate();
	}

	void GLRenderableUnit::InactiveCubeMap()
	{
		if (!staticMesh.expired())
			staticMesh.lock()->Inactivate();

		if (!ViewCubeMapMaterial.expired())
			ViewCubeMapMaterial.lock()->Inactivate();
	}

	void GLRenderableUnit::ActivePickShooter()
	{
		if (!staticMesh.expired())
			staticMesh.lock()->Activate();

		if (!PickShooterMaterial.expired())
			PickShooterMaterial.lock()->Activate();
	}

	void GLRenderableUnit::InactivePickShooter()
	{
		if (!staticMesh.expired())
			staticMesh.lock()->Inactivate();

		if (!PickShooterMaterial.expired())
			PickShooterMaterial.lock()->Inactivate();
	}
	
	GLRenderableUnit::~GLRenderableUnit()
	{

	}
}