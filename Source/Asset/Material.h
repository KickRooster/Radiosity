#pragma once
#include <BaseInclude.h>
#include <../3rdParty/cereal/archives/xml.hpp>
#include "../RHI/OpenGLRHI/GLProgram.h"
#include "IAsset.h"
#include "GLSL.h"
#include "RLSL.h"
#include "Texture.h"
#include "../DesignPattern/ObserverPattern/Obeserver.h"
#include "../Math/Vector4.h"
#include "../RHI/OpenGLRHI/ImageTexture.h"

#include <string>

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

		std::weak_ptr<RLSL> rlVertexShader;
		std::weak_ptr<RLSL> rlRayShader;

		std::weak_ptr<Texture> albedoTexture;
		std::weak_ptr<GLTexture> VisibilityTexture;
		std::weak_ptr<GLImageTexture> RadiosityImage0;
		std::weak_ptr<GLImageTexture> RadiosityImage1;
		std::weak_ptr<GLImageTexture> ResidualImage0;
		std::weak_ptr<GLImageTexture> ResidualImage1;
		std::weak_ptr<Texture> normalMap;
		std::weak_ptr<Texture> metallicTexture;
		std::weak_ptr<Texture> roughnessTexture;
		std::weak_ptr<Texture> lightmapTexture;
		std::weak_ptr<GLImageTexture> lightmapImageTexture;

		Bool IsOccluder;
		Bool IsBeingBaking;

		//	TODO:	�������ܻ���id
		std::string glVertexShaderName;
		std::string glFragmentShaderName;
		std::string rlVertexShaderName;
		std::string rlRayShaderName;
		
		std::string albedoTextureName;
		std::string IDTextureName;
		std::string normalMapName;
		std::string metallicTextureName;
		std::string roughnessTextureName;
		std::string lightmapName;
		std::string maskMapName;

		float albedoColor[4] = { 1.0f, 1.0f,1.0f,1.0f }; 

		const std::string albedoSamplerName = "albedoSampler";
		const std::string IDCubeMapSamplerName = "IDCubeMapSampler";
		const std::string VisibilitySamplerName = "VisibilitySampler";
		const std::string RadiosityImage0Name = "RadiosityOutput0";
		const std::string RadiosityImage1Name = "RadiosityOutput1";
		const std::string ResidualImage0Name = "ResidualOutput0";
		const std::string ResidualImage1Name = "ResidualOutput1";
		const std::string normalSamplerName = "normalSampler";
		const std::string metallicSamplerName = "metallicSampler";
		const std::string roughnessSamplerName = "roughnessSampler";
		const std::string lightmapSamplerName = "lightmapSampler";
		const std::string idName = "id";
		const std::string albedoColorName = "albedoColor";

		Material();
		Material(const Material & mat);

		void BeginUse() override;
		void Reload() override;
		void BeforeSave() override;
		void AfterLoad() override;
		void Activate();
		void Inactivate();
		void SetSampler(int32 slotIndex, const std::string& samplerName, const GLTexture * pTexture);
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
		
		virtual void UpdateAhead();
		virtual void UpdateBehind();
		virtual ~Material();
	};
}