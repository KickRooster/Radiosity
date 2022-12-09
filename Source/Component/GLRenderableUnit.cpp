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

		if (!DrawIDMaterial.expired())
			DrawIDMaterial.lock()->BeginUse();
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
	
	GLRenderableUnit::~GLRenderableUnit()
	{

	}
}