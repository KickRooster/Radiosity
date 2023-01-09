#pragma once
#include <BaseInclude.h>
#include "..\Object\Object.h"
#include "..\Object\Camera.h"
#include "Prefab.h"
#include "../3rdParty/cereal/types/vector.hpp"

#include VECTOR_INCLUDE_PATH

namespace Core
{
	class Scene : public IAsset
	{
	private:
		friend class cereal::access;

		std::unique_ptr<Camera> m_camera;

		std::vector<std::weak_ptr<Object>> m_LightRefs;
		std::weak_ptr<Object> m_beingBakingObjectRef;

	public:
		ctd::vector<std::shared_ptr<Object>> objects;
		ctd::vector<std::shared_ptr<Object>> serializedObjects;

		Scene();
		Scene(Scene &) {}
		virtual void BeginUse() override;
		virtual void Reload() override;
		virtual void BeforeSave() override;
		virtual void AfterLoad() override;
		void Initialize(OpenGLDevice * pDevice, int32 width, int32 height);
		void Tick(float deltaTime, OpenGLDevice * pDevice, const InputState & inputState);
		void Render(OpenGLDevice * pDevice, int32 width, int32 height, Bool LightmapEncodingInRGBM);
		Camera * GetCamera();
		void AddObject(std::shared_ptr<Object> object, Bool needSerialization = True);
		void AddLight(std::shared_ptr<Object> object, Bool needSerialization = True);
		int32 GetLightCount() const ;
		Object* GetAreaLight(int32 Index) const;
		Object* GetBeingBakingObject() const;

		template <class Archive>
		void serialize(Archive & ar)
		{
			ar(serializedObjects);
		}

		~Scene();
	};
}