#pragma once
#include <BaseInclude.h>
#include "IAsset.h"
#include "TextureInfo.h"
#include "..\RHI\OpenGLRHI\GLTexture.h"
#include "..\DesignPattern\ObserverPattern\Subject.h"

namespace Core
{
	class Texture : public IAsset, public Subject
	{
	private:
		std::unique_ptr<GLTexture> m_glTexture;
		
		void load();

	public:
		std::shared_ptr<TextureInfo> info;
		int32 width;
		int32 height;
		void * pImage;

		Texture();

		virtual void BeginUse() override;
		virtual void Reload() override;
		virtual void BeforeSave() override;
		virtual void AfterLoad() override;
		void Activate();
		void Inactivate();
		void SetGLWrapMode(GLTextureWrapMode wrapMode);
		GLTextureWrapMode GetGLWrapMode() const;
		void SetGLFilterMode(GLTextureFilterMode filterMode);
		GLTextureFilterMode GetGLFilterMode() const;
		void UploadToGL();
		virtual ~Texture();
	};
}