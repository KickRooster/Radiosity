#pragma once
#include "BaseRL.h"
#include "RLShader.h"
#include "..\..\Math\Vector3.h"

namespace Core
{
	class RLProgram
	{
	private:
		RLprogram m_rawData;

	public:
		RLProgram();
		RLprogram GetRawData();
		void AttachShader(RLShader * pShaderAttach) const;
		void DetachShader(RLShader * pShaderDetach) const;
		void Link();
		void Activate() const;
		void Inactivate() const;
		void SetUniform1f(const ANSICHAR * pParamName, float value) const;
		void SetUniform3f(const ANSICHAR * pParamName, const Vector3 * pValue) const;
		void SetUniformMatrix4f(const ANSICHAR * pParamName, const float * pValue) const;
		void SetUniformPrimitive(const ANSICHAR * pParamName, RLprimitive value) const;
		void SetTexture(const ANSICHAR * pParamName, RLtexture texture) const;
		~RLProgram();
	};
}