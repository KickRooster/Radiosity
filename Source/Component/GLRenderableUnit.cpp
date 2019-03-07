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

		if (!bakingMaterial.expired())
			bakingMaterial.lock()->BeginUse();
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

	void GLRenderableUnit::ActivateBaking()
	{
		if (!staticMesh.expired())
			staticMesh.lock()->Activate();

		if (!bakingMaterial.expired())
			bakingMaterial.lock()->Activate();
	}

	void GLRenderableUnit::InactivateBaking()
	{
		if (!staticMesh.expired())
			staticMesh.lock()->Inactivate();

		if (!bakingMaterial.expired())
			bakingMaterial.lock()->Inactivate();
	}

	GLRenderableUnit::~GLRenderableUnit()
	{

	}
}