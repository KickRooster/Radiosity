#include "Material.h"
#include "..\Helper\FileOperator.h"

namespace Core
{
	Material::Material()
		:IsBeingBaking(False)
	{

	}

	Material::Material(Material & mat)
	{
		this->albedoTextureName = mat.albedoTextureName;
		this->normalMapName = mat.normalMapName;
		this->metallicTextureName = mat.metallicTextureName;
		this->roughnessTextureName = mat.roughnessTextureName;
		this->fileNameWithExt = mat.fileNameWithExt;
		this->fullPathName = mat.fullPathName;
		this->IsOccluder = mat.IsOccluder;
		this->lightmapName = mat.lightmapName;
		this->type = mat.type;
	}

	void Material::BeginUse()
	{
		if (!m_beingUsed)
		{
			m_glProgram = std::make_unique<GLProgram>();

			if (!glVertexShader.expired())
			{
				glVertexShader.lock()->BeginUse();
				m_glProgram->AttachShader(glVertexShader.lock()->GetGLShader());
			}

			if (!glFragmentShader.expired())
			{
				glFragmentShader.lock()->BeginUse();
				m_glProgram->AttachShader(glFragmentShader.lock()->GetGLShader());
			}

			if (!glVertexShader.expired() && !glFragmentShader.expired())
			{
				m_glProgram->Link();
			}
			
			if (!rlVertexShader.expired())
				rlVertexShader.lock()->BeginUse();

			if (!rlRayShader.expired())
				rlRayShader.lock()->BeginUse();

			if (!albedoTexture.expired())
				albedoTexture.lock()->BeginUse();

			if (!normalMap.expired())
				normalMap.lock()->BeginUse();

			if (!metallicTexture.expired())
				metallicTexture.lock()->BeginUse();

			if (!roughnessTexture.expired())
				roughnessTexture.lock()->BeginUse();

			if (!lightmapTexture.expired())
				lightmapTexture.lock()->BeginUse();

			m_beingUsed = True;
		}
	}

	void Material::Reload()
	{
		FileOperator::DeserializeXML(fullPathName.c_str(), this);
	}

	void Material::BeforeSave()
	{
	}

	void Material::AfterLoad()
	{
	}

	void Material::Activate()
	{
		m_glProgram->Activate();

		int32 SamplerSlotIndex = 0;

		if (!albedoTexture.expired())
		{
			m_glProgram->ActivateTextureSlot(SamplerSlotIndex, albedoSamplerName.c_str());
			++SamplerSlotIndex;
			albedoTexture.lock()->Activate();
		}

		if (!IDCumeMap.expired())
		{
			m_glProgram->ActivateTextureSlot(SamplerSlotIndex, IDCubeMapSamplerName.c_str());
			++SamplerSlotIndex;
			IDCumeMap.lock()->Activate();
		}

		if (!VisibilityTexture.expired())
		{
			m_glProgram->ActivateTextureSlot(SamplerSlotIndex, VisibilitySamplerName.c_str());
			++SamplerSlotIndex;
			VisibilityTexture.lock()->Activate();
		}

		if (!RadiosityImage0.expired())
		{
			m_glProgram->ActivateImageSlot(RadiosityImage0.lock()->GetUnit(), RadiosityImage0Name.c_str());
		}

		if (!RadiosityImage1.expired())
		{
			m_glProgram->ActivateImageSlot(RadiosityImage1.lock()->GetUnit(), RadiosityImage1Name.c_str());
		}

		if (!ResidualImage0.expired())
		{
			m_glProgram->ActivateImageSlot(ResidualImage0.lock()->GetUnit(), ResidualImage0Name.c_str());
		}

		if (!ResidualImage1.expired())
		{
			m_glProgram->ActivateImageSlot(ResidualImage1.lock()->GetUnit(), ResidualImage1Name.c_str());
		}
		
		if (!normalMap.expired())
		{
			m_glProgram->ActivateTextureSlot(SamplerSlotIndex, normalSamplerName.c_str());
			++SamplerSlotIndex;
			normalMap.lock()->Activate();
		}

		if (!metallicTexture.expired())
		{
			m_glProgram->ActivateTextureSlot(SamplerSlotIndex, metallicSamplerName.c_str());
			++SamplerSlotIndex;
			metallicTexture.lock()->Activate();
		}

		if (!roughnessTexture.expired())
		{
			m_glProgram->ActivateTextureSlot(SamplerSlotIndex, roughnessSamplerName.c_str());
			++SamplerSlotIndex;
			roughnessTexture.lock()->Activate();
		}

		if (IsBeingBaking && !lightmapImageTexture.expired())
		{
			m_glProgram->ActivateTextureSlot(SamplerSlotIndex, lightmapSamplerName.c_str());
			++SamplerSlotIndex;
			lightmapImageTexture.lock()->Activate();
		}
		else if (!lightmapTexture.expired())
		{
			m_glProgram->ActivateTextureSlot(SamplerSlotIndex, lightmapSamplerName.c_str());
			++SamplerSlotIndex;
			lightmapTexture.lock()->Activate();
		}
	}

	void Material::Inactivate()
	{
		m_glProgram->Inactivate();

		if (!albedoTexture.expired())
			albedoTexture.lock()->Inactivate();

		if (!IDCumeMap.expired())
		{
			IDCumeMap.lock()->Inactivate();
		}

		if (!VisibilityTexture.expired())
		{
			VisibilityTexture.lock()->Inactivate();
		}

		if (!normalMap.expired())
			normalMap.lock()->Inactivate();

		if (!metallicTexture.expired())
			metallicTexture.lock()->Inactivate();

		if (!roughnessTexture.expired())
			roughnessTexture.lock()->Inactivate();

		if (!lightmapImageTexture.expired())
		{
			lightmapImageTexture.lock()->Inactivate();
		}
		else if (!lightmapTexture.expired())
		{
			lightmapTexture.lock()->Inactivate();
		}
	}

	void Material::SetSampler(int32 slotIndex, ctd::string samplerName, GLTexture * pTexture)
	{
		m_glProgram->Activate();
		m_glProgram->ActivateTextureSlot(slotIndex, samplerName.c_str());
		m_glProgram->Inactivate();
		pTexture->Activate();
	}

	void Material::SetAlbedoColor()
	{
		if (m_glProgram.get() != Null)
		{
			m_glProgram->Set4f(albedoColorName.c_str(), albedoColor);
		}
	}

	void Material::UpdateAhead()
	{
		if (m_glProgram.get() != Null)
		{
			m_glProgram->DetachShader(glVertexShader.lock()->GetGLShader());
			m_glProgram->DetachShader(glFragmentShader.lock()->GetGLShader());
		}
	}

	void Material::UpdateBehind()
	{
		if (m_glProgram.get() != Null)
		{
			m_glProgram->AttachShader(glVertexShader.lock()->GetGLShader());
			m_glProgram->AttachShader(glFragmentShader.lock()->GetGLShader());
			m_glProgram->Link();
		}
	}

	Material::~Material()
	{
	}
}