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

#include <string>

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

		Bool m_isLight;
		float m_color_r;
		float m_color_g;
		float m_color_b;
		float m_intensity;
		float m_energy_r;
		float m_energy_g;
		float m_energy_b;
		int32 m_lightmap_resolution;
		int32 m_light_precision;
		int32 m_x_length;
		int32 m_y_length;
		int32 m_max_x_length;
		int32 m_max_y_length;

	private:
		Matrix4x4 m_object2WorldMatrix;
		Matrix4x4 m_world2ObjectMatrix;
		Matrix4x4 m_object2WorldITMatrix;

	public:
		static int32 idSeed;
		int32 id;
		
		std::string name;
		std::string staticMeshName;
		std::string materialName;

		Vector3 position;
		Vector3 eulerAngle;
		Vector3 scale;

		//	Light property
		Bool IsLight;
		float Color[3];
		float Intensity;
		float Energy[3];
		//	For baking object only for editor.
		LightmapResolution LightmapResolution;
		const ANSICHAR* ResolutionString;
		LightPrecision LightPrecision;
		const ANSICHAR* PrecisionString;
		int32 XLength;
		int32 YLength;
		int32 MaxXLength;
		int32 MaxYLength;
		
		std::unique_ptr<GLRenderableUnit> glRenderableUnit;
		std::unique_ptr<RLRenderableUnit> rlRenderableUnit;
		
		Object();
		void Initialize(OpenGLDevice * pDevice, Bool SerializedRTSAsLocal2World);
		void UpdateTransformMatrix();
		void Tick(float deltaTime, OpenGLDevice * pDevice);
		void Render(OpenGLDevice * pDevice);
		void RenderAsPostprocess(OpenGLDevice * pDevice, Vector4 & postprocessParam);
		void BeforeBaking();
		void UpdateRLMatrix(RLDevice * pDevice);
		void DrawGBuffer(OpenGLDevice * pDevice);
		void ComputeFormFactor(OpenGLDevice * pDevice);
		void BeforeComputeFormFactor(OpenGLDevice* pDevice);
		void DrawPrimitive(OpenGLDevice * pDevice, int32 StartPrimitive, int32 PrimitiveCount);
		void AfterComputeFormFactor();
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

			m_isLight = IsLight;
			m_color_r = Color[0];
			m_color_g = Color[1];
			m_color_b = Color[2];
			m_intensity = Intensity;
			m_energy_r = Energy[0];
			m_energy_g = Energy[1];
			m_energy_b = Energy[2];
			m_lightmap_resolution = LightmapResolution;
			m_light_precision = LightPrecision;
			m_x_length = XLength;
			m_y_length = YLength;
			m_max_x_length = MaxXLength;
			m_max_y_length = MaxYLength;
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

			IsLight = m_isLight;
			Color[0] = m_color_r;
			Color[1] = m_color_g;
			Color[2] = m_color_b;
			Intensity = m_intensity;
			Energy[0] = m_energy_r;
			Energy[1] = m_energy_g;
			Energy[2] = m_energy_b;
			LightmapResolution = static_cast<Core::LightmapResolution>(m_lightmap_resolution);
			LightPrecision = static_cast<Core::LightPrecision>(m_light_precision);
			XLength = m_x_length;
			YLength = m_y_length;
			MaxXLength = m_max_x_length;
			MaxYLength = m_max_y_length;
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
				m_scale_z,
				m_isLight,
				m_color_r,
				m_color_g,
				m_color_b,
				m_intensity,
				m_energy_r,
				m_energy_g,
				m_energy_b,
				m_lightmap_resolution,
				m_light_precision,
				m_x_length,
				m_y_length,
				m_max_x_length,
				m_max_y_length
			);
		}

		~Object();
	};
}