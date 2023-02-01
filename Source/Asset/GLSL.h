#pragma once
#include <BaseInclude.h>
#include "IAsset.h"
#include "..\RHI\OpenGLRHI\GLShader.h"
#include "..\DesignPattern\ObserverPattern\Subject.h"

namespace Core
{
	class GLSL : public IAsset, public Subject
	{
		std::unique_ptr<GLShader> m_shader;
		void load();
	public:
		GLSL();
		void BeginUse() override;
		void Reload() override;
		void BeforeSave() override;
		void AfterLoad() override;
		GLShader * GetGLShader() const;
		virtual ~GLSL();
	};
} 