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

	private:
		friend class cereal::access;

		std::unique_ptr<Camera> m_camera;

		std::weak_ptr<Object> m_dirLightRef;

		int32 m_objectCursor;

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
		void Render(OpenGLDevice * pDevice, int32 width, int32 height);
		void BeginBaking(OpenGLDevice * pGLDevice, int32 width, int32 height);
		Bool BakingFinished()const;
		Object * PoolBakingObject() const;
		void SkipOneObject();
		Bool BakeOneObject(OpenGLDevice * pDevice);
		void EndBaking(OpenGLDevice * pDevice);
		Camera * GetCamera();
		void AddObject(std::shared_ptr<Object> object, Bool needSerialization = True);
		void AddLight(std::shared_ptr<Object> object, Bool needSerialization = True);

		template <class Archive>
		void serialize(Archive & ar)
		{
			ar(serializedObjects);
		}

		~Scene();
	};
}