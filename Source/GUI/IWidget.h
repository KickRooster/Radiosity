#pragma once
#include <BaseInclude.h>
#include <Vector2.h>

namespace Core
{
	class IWidget
	{
	protected:
		std::shared_ptr<IWidget> m_pParent;
		Vector2 m_position;

	public:
		virtual void SetParent(IWidget * const pParent) = 0;
		virtual void Update() = 0;
		virtual void Draw() = 0;
	};
}