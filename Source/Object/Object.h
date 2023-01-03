#pragma once
#include <BaseInclude.h>
#include "..\Math\Matrix4x4.h"
#include "..\Asset\StaticMesh.h"
#include "..\RHI\OpenGLRHI\OpenGLDevice.h"
#include "..\RHI\OpenRLRHI\RLDevice.h"
#include "..\Component\GLRenderableUnit.h"
#include "..\Component\RLRenderableUnit.h"
#include "../3rdParty/cereal/archives/xml.hpp"
#include "..\3rdParty\RectangleBinPack\Rect.h"

#include STRING_INCLUDE_PATH

namespace Core
{
	class Object
	{
	private:
		float m_position_x;
		float m_position_y;
		float m_position_z;

		float m_eulerAngle_x;
		float m_eulerAngle_y;
		float m_eulerAngle_z;

		float m_scale_x;
		float m_scale_y;
		float m_scale_z;

	private:
		Matrix4x4 m_object2WorldMatrix;
		Matrix4x4 m_world2ObjectMatrix;
		Matrix4x4 m_object2WorldITMatrix;

	public:
		static int32 idSeed;
		int32 id;

		ctd::string name;
		ctd::string staticMeshName;
		ctd::string materialName;

		Vector3 position;
		Vector3 eulerAngle;
		Vector3 scale;
		float Energy[3];

		std::unique_ptr<GLRenderableUnit> glRenderableUnit;
		std::shared_ptr<RLRenderableUnit> rlRenderableUnit;
		
		Object();
		void Initialize(OpenGLDevice * pDevice, Bool SerializedRTSAsLocal2World);
		void Tick(float deltaTime, OpenGLDevice * pDevice);
		void Render(OpenGLDevice * pDevice);
		void RenderAsPostprocess(OpenGLDevice * pDevice, Vector4 & postprocessParam);
		void BeforeBaking();
		void UpdateRLMatrix(RLDevice * pDevice);
		void DrawGBuffer(OpenGLDevice * pDevice);
		void DrawID(OpenGLDevice * pDevice);
		void ComputeFormFactor(OpenGLDevice * pDevice);
		void BeforeComputeFormFactor(OpenGLDevice* pDevice);
		void DrawPrimitive(OpenGLDevice * pDevice, int32 StartPrimitive, int32 PrimitiveCount);
		void AfterComputeFormFactor();
		void BeforePickShooter(OpenGLDevice * pDevice);
		void AfterPickShooter();
		void ViewCubeMap(OpenGLDevice * pDevice);
		void SetShootingPrimitive(float PrimitiveID);
		Matrix4x4 * GetObject2WorldMatrix() ;
		Vector3 GetZDirection();
		Vector3 GetYDirection();
		float GetXScale() const;
		float GetYScale() const;
		float GetZScale() const;

		void BeforeSave()
		{
			m_position_x = position.x;
			m_position_y = position.y;
			m_position_z = position.z;

			m_eulerAngle_x = eulerAngle.x;
			m_eulerAngle_y = eulerAngle.y;
			m_eulerAngle_z = eulerAngle.z;

			m_scale_x = scale.x;
			m_scale_y = scale.y;
			m_scale_z = scale.z;
		}

		void AfterLoad()
		{
			position.x = m_position_x;
			position.y = m_position_y;
			position.z = m_position_z;

			eulerAngle.x = m_eulerAngle_x;
			eulerAngle.y = m_eulerAngle_y;
			eulerAngle.z = m_eulerAngle_z;

			scale.x = m_scale_x;
			scale.y = m_scale_y;
			scale.z = m_scale_z;
		}

		template <class Archive>
		void serialize(Archive & ar)
		{
			ar(
				name,
				staticMeshName,
				materialName,
				m_position_x,
				m_position_y,
				m_position_z,
				m_eulerAngle_x,
				m_eulerAngle_y,
				m_eulerAngle_z,
				m_scale_x,
				m_scale_y,
				m_scale_z
			);
		}

		~Object();
	};
}