#pragma once
#include <BaseInclude.h>
#include <../3rdParty/cereal/archives/xml.hpp>
#include "../RHI/OpenGLRHI/GLProgram.h"
#include "IAsset.h"
#include "GLSL.h"
#include "Texture.h"
#include "../DesignPattern/ObserverPattern/Obeserver.h"
#include "../Math/Vector4.h"
#include "../RHI/OpenGLRHI/ImageTexture.h"

#include STRING_INCLUDE_PATH

namespace Core
{
	//	����һ�����Ա����л����Զ����������.
	struct CustomSerializableClass
	{
		int x;

		// Enabled for text archives (e.g. XML, JSON)
		template <class Archive,
			cereal::traits::EnableIf<cereal::traits::is_text_archive<Archive>::value>
			= cereal::traits::sfinae>
			std::string save_minimal(Archive &) const
		{
			return std::to_string(x) + "hello";
		}

		// Enabled for text archives (e.g. XML, JSON)
		template <class Archive,
			cereal::traits::EnableIf<cereal::traits::is_text_archive<Archive>::value>
			= cereal::traits::sfinae>
			void load_minimal(Archive const &, std::string const & str)
		{
			x = std::stoi(str.substr(0, 1));
		}

		// Enabled for binary archives (e.g. binary, portable binary)
		template <class Archive,
			cereal::traits::DisableIf<cereal::traits::is_text_archive<Archive>::value>
			= cereal::traits::sfinae>
			int save_minimal(Archive &) const
		{
			return x;
		}

		// Enabled for binary archives (e.g. binary, portable binary)
		template <class Archive,
			cereal::traits::DisableIf<cereal::traits::is_text_archive<Archive>::value>
			= cereal::traits::sfinae>
			void load_minimal(Archive const &, int const & xx)
		{
			x = xx;
		}
	};

	class Material : public IAsset, public Obeserver
	{
	private:
		std::unique_ptr<GLProgram> m_glProgram;

		friend class cereal::access;

	public:
		std::weak_ptr<GLSL> glVertexShader;
		std::weak_ptr<GLSL> glFragmentShader;

		std::weak_ptr<Texture> albedoTexture;
		std::weak_ptr<GLTexture> IDCumeMap;
		std::weak_ptr<GLImageTexture> AccumulatedImage0;
		std::weak_ptr<GLImageTexture> AccumulatedImage1;
		std::weak_ptr<GLImageTexture> ResidualImage0;
		std::weak_ptr<GLImageTexture> ResidualImage1;
		std::weak_ptr<Texture> normalMap;
		std::weak_ptr<Texture> metallicTexture;
		std::weak_ptr<Texture> roughnessTexture;
		std::weak_ptr<Texture> lightmapTexture;
		std::weak_ptr<GLImageTexture> lightmapImageTexture;

		Bool IsOccluder;

		//	TODO:	�������ܻ���id
		ctd::string glVertexShaderName;
		ctd::string glFragmentShaderName;

		ctd::string albedoTextureName;
		ctd::string IDTextureName;
		ctd::string normalMapName;
		ctd::string metallicTextureName;
		ctd::string roughnessTextureName;
		ctd::string lightmapName;
		ctd::string maskMapName;

		float albedoColor[4] = { 1.0f, 1.0f,1.0f,1.0f }; 

		const ctd::string albedoSamplerName = "albedoSampler";
		const ctd::string IDCubeMapSamplerName = "IDCubeMapSampler";
		const ctd::string AccumulatedImage0Name = "AccumulatedOutput0";
		const ctd::string AccumulatedImage1Name = "AccumulatedOutput1";
		const ctd::string ResidualImage0Name = "ResidualOutput0";
		const ctd::string ResidualImage1Name = "ResidualOutput1";
		const ctd::string normalSamplerName = "normalSampler";
		const ctd::string metallicSamplerName = "metallicSampler";
		const ctd::string roughnessSamplerName = "roughnessSampler";
		const ctd::string lightmapSamplerName = "lightmapSampler";
		const ctd::string idName = "id";
		const ctd::string albedoColorName = "albedoColor";
		//const ctd::string bounceProbablilityName = "bounceProbablility";

		Material();
		Material(Material & mat);

		virtual void BeginUse() override;
		virtual void Reload() override;
		virtual void BeforeSave() override;
		virtual void AfterLoad() override;
		void Activate();
		void Inactivate();
		void SetSampler(int32 slotIndex, ctd::string samplerName, GLTexture * pTexture);
		void SetAlbedoColor();

		template <class Archive>
		void serialize(Archive & ar)
		{
			ar(IsOccluder,
				glVertexShaderName,
				glFragmentShaderName,
				albedoTextureName,
				normalMapName,
				metallicTextureName,
				roughnessTextureName,
				lightmapName,
				albedoColor
			);
		}

		//template <class Archive>
		//void save(Archive & ar) const
		//{
		//	//m_lightmapUVParam_x = lightmapUVParam.x;
		//	//m_lightmapUVParam_y = lightmapUVParam.y;
		//	//m_lightmapUVParam_z = lightmapUVParam.z;
		//	//m_lightmapUVParam_w = lightmapUVParam.w;
		//
		//	ar(
		//		m_lightmapUVParam_x,
		//		m_lightmapUVParam_y,
		//		m_lightmapUVParam_z,
		//		m_lightmapUVParam_w,
		//		IsOccluder,
		//		lightmapIndex,
		//		glVertexShaderName,
		//		glFragmentShaderName,
		//		rlVertexShaderName,
		//		rlRayShaderName,
		//		albedoTextureName,
		//		lightmapName
		//	);
		//}
		//
		//template <class Archive>
		//void load(Archive & ar)
		//{
		//	ar(
		//		m_lightmapUVParam_x,
		//		m_lightmapUVParam_y,
		//		m_lightmapUVParam_z,
		//		m_lightmapUVParam_w,
		//		IsOccluder,
		//		lightmapIndex,
		//		glVertexShaderName,
		//		glFragmentShaderName,
		//		rlVertexShaderName,
		//		rlRayShaderName,
		//		albedoTextureName,
		//		lightmapName
		//	);
		//
		//	lightmapUVParam.x = m_lightmapUVParam_x;
		//	lightmapUVParam.y = m_lightmapUVParam_y;
		//	lightmapUVParam.z = m_lightmapUVParam_z;
		//	lightmapUVParam.w = m_lightmapUVParam_w;
		//}

		virtual void UpdateAhead();
		virtual void UpdateBehind();
		virtual ~Material();
	};
}