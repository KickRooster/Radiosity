#pragma once
#include <BaseInclude.h>
#include "..\Asset\StaticMesh.h"
#include "..\Asset\Material.h"
#include <OpenRL\rl.h>
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
		RLprogram m_bakingProgram;
		
	public:
		std::weak_ptr<StaticMesh> staticMesh;
		std::weak_ptr<Material> material;
		std::weak_ptr<RLBuffer> lightingSettingsUniformBuffer;
		std::weak_ptr<RLBuffer> giUniformBuffer;

		RLRenderableUnit();
		RLprimitive GetRawPrimitive() const;
		void Commit(int32 objectID);
		void Activate();
		void SetAlbdeoTexture();
		void SetAlbdeoNormalMap();
		void SetAlbedoColor();
		void Inactivate();
		virtual void UpdateAhead() override;
		virtual void UpdateBehind() override;
		~RLRenderableUnit();
	};
}