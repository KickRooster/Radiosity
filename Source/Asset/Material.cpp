#include "Material.h"
#include "..\Helper\FileOperator.h"

namespace Core
{
	Material::Material()
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

			glVertexShader.lock()->BeginUse();
			m_glProgram->AttachShader(glVertexShader.lock()->GetGLShader());

			glFragmentShader.lock()->BeginUse();
			m_glProgram->AttachShader(glFragmentShader.lock()->GetGLShader());

			m_glProgram->Link();

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

		int32 slotIndex = 0;

		if (!albedoTexture.expired())
		{
			m_glProgram->ActivateTextureSlot(slotIndex, albedoSamplerName.c_str());
			++slotIndex;
			albedoTexture.lock()->Activate();
		}

		if (!IDTexture.expired())
		{
			m_glProgram->ActivateTextureSlot(slotIndex, IDSamplerName.c_str());
			++slotIndex;
			IDTexture.lock()->Activate();
		}

		if (!normalMap.expired())
		{
			m_glProgram->ActivateTextureSlot(slotIndex, normalSamplerName.c_str());
			++slotIndex;
			normalMap.lock()->Activate();
		}

		if (!metallicTexture.expired())
		{
			m_glProgram->ActivateTextureSlot(slotIndex, metallicSamplerName.c_str());
			++slotIndex;
			metallicTexture.lock()->Activate();
		}

		if (!roughnessTexture.expired())
		{
			m_glProgram->ActivateTextureSlot(slotIndex, roughnessSamplerName.c_str());
			++slotIndex;
			roughnessTexture.lock()->Activate();
		}

		if (!lightmapTexture.expired())
		{
			m_glProgram->ActivateTextureSlot(slotIndex, lightmapSamplerName.c_str());
			++slotIndex;
			lightmapTexture.lock()->Activate();
		}
	}

	void Material::Inactivate()
	{
		m_glProgram->Inactivate();

		if (!albedoTexture.expired())
			albedoTexture.lock()->Inactivate();

		if (!IDTexture.expired())
		{
			IDTexture.lock()->Inactivate();
		}
		
		if (!normalMap.expired())
			normalMap.lock()->Inactivate();

		if (!metallicTexture.expired())
			metallicTexture.lock()->Inactivate();

		if (!roughnessTexture.expired())
			roughnessTexture.lock()->Inactivate();

		if (!lightmapTexture.expired())
			lightmapTexture.lock()->Inactivate();
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