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
		friend class cereal::access;

		std::unique_ptr<Camera> m_camera;

		std::vector<std::weak_ptr<Object>> m_LightRefs;
		std::weak_ptr<Object> m_beingBakingObjectRef;

		float m_camera_z_near;
		float m_camera_z_far;
		float m_camera_fov;
		float m_camera_position_x;
		float m_camera_position_y;
		float m_camera_position_z;
		float m_camera_look_at_x;
		float m_camera_look_at_y;
		float m_camera_look_at_z;
		float m_camera_eulerAngle_x;
		float m_camera_eulerAngle_y;
		float m_camera_eulerAngle_z;
	
	public:
		ctd::vector<std::shared_ptr<Object>> objects;
		ctd::vector<std::shared_ptr<Object>> serializedObjects;
		
		Scene();
		Scene(Scene &) {}
		virtual void BeginUse() override;
		virtual void Reload() override;
		virtual void BeforeSave() override;
		virtual void AfterLoad() override;
		void Tick(float deltaTime, OpenGLDevice * pDevice, const InputState & inputState);
		void Render(OpenGLDevice * pDevice, int32 width, int32 height, Bool LightmapEncodingInRGBM);
		Camera * GetCamera();
		void AddObject(std::shared_ptr<Object> object, Bool needSerialization = True);
		void AddLight(std::shared_ptr<Object> object, Bool needSerialization = True);
		void RemoveLight(Object* Light);
		void Clear();
		int32 GetLightCount() const ;
		Object* GetAreaLight(int32 Index) const;
		Object* GetBeingBakingObject() const;

		template <class Archive>
		void serialize(Archive & ar)
		{
			ar(m_camera_z_near);
			ar(m_camera_z_far);
			ar(m_camera_fov);
			ar(m_camera_position_x);
			ar(m_camera_position_y);
			ar(m_camera_position_z);
			ar(m_camera_look_at_x);
			ar(m_camera_look_at_y);
			ar(m_camera_look_at_z);
			ar(m_camera_eulerAngle_x);
			ar(m_camera_eulerAngle_y);
			ar(m_camera_eulerAngle_z);
			ar(serializedObjects);
		}

		~Scene();
	};
}