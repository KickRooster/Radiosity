#include "GLRenderableUnit.h"

namespace Core
{
	GLRenderableUnit::GLRenderableUnit()
	{
	}

	void GLRenderableUnit::BeginUse()
	{
		if (staticMesh)
			staticMesh->BeginUse();

		if (!material.expired())
			material.lock()->BeginUse();

		if (!DrawGBufferMaterial.expired())
			DrawGBufferMaterial.lock()->BeginUse();
		
		if (!ComputeFormFactorMaterial.expired())
			ComputeFormFactorMaterial.lock()->BeginUse();
	}

	void GLRenderableUnit::Activate()
	{
		if (staticMesh)
			staticMesh->Activate();

		if (!material.expired())
			material.lock()->Activate();
	}

	void GLRenderableUnit::Inactivate()
	{
		if (staticMesh)
			staticMesh->Inactivate();

		if (!material.expired())
			material.lock()->Inactivate();
	}

	void GLRenderableUnit::ActiveDrawGBuffer()
	{
		if (staticMesh)
			staticMesh->Activate();

		if (!DrawGBufferMaterial.expired())
			DrawGBufferMaterial.lock()->Activate();
	}

	void GLRenderableUnit::InactiveDrawGBuffer()
	{
		if (staticMesh)
			staticMesh->Inactivate();

		if (!DrawGBufferMaterial.expired())
			DrawGBufferMaterial.lock()->Inactivate();
	}
	
	void GLRenderableUnit::ActiveComputingFormFactor()
	{
		if (staticMesh)
			staticMesh->Activate();

		if (!ComputeFormFactorMaterial.expired())
			ComputeFormFactorMaterial.lock()->Activate();
	}

	void GLRenderableUnit::InactiveComputingFormFactor()
	{
		if (staticMesh)
			staticMesh->Inactivate();

		if (!ComputeFormFactorMaterial.expired())
			ComputeFormFactorMaterial.lock()->Inactivate();
	}

	GLRenderableUnit::~GLRenderableUnit()
	{

	}
}