#pragma once
#include "IWidget.h"
#include "../Asset/StaticMesh.h"
#include "../Component/GLRenderableUnit.h"


namespace Core
{
	class Button : public IWidget
	{
	private:
		std::unique_ptr<StaticMesh> m_pStaticMesh;
		std::unique_ptr<GLRenderableUnit> m_pRenderableUnit;

	public:
		Button();
		virtual void SetParent(IWidget * const pParent);
		virtual void Update();
		virtual void Draw();
		virtual ~Button();
	};
}