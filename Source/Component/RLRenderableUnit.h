#pragma once
#include <BaseInclude.h>
#include "..\Asset\StaticMesh.h"
#include "..\Asset\Material.h"
#include <OpenRL\rl.h>
#include "..\RHI/OpenRLRHI/RLBuffer.h"
#include "..\DesignPattern\ObserverPattern\Obeserver.h"

namespace Core
{
	class RLRenderableUnit : public Obeserver
	{
	private:
		RLprimitive m_primitive;
		RLbuffer m_indexBuffer;
		RLbuffer m_vertexBuffer;
		RLprogram m_program;
		
	public:
		std::weak_ptr<StaticMesh> staticMesh;
		std::weak_ptr<Material> material;

		RLRenderableUnit();
		RLprimitive GetRawPrimitive() const;
		RLprogram GetProgram() const;
		void Commit();
		void Activate();
		void Inactivate();
		virtual void UpdateAhead() override;
		virtual void UpdateBehind() override;
		~RLRenderableUnit();
	};
}