#pragma once
#include "BaseRL.h"
#include "RLProgram.h"
#include "RLShader.h"

namespace Core
{
	class RLPrimitive
	{
	private:
		static ANSICHAR * defaultVertexShaderSource;
		static ANSICHAR * defaultRayShaderSource;

		static ANSICHAR * defaultVertexShaderFullPathName;
		static ANSICHAR * defaultRayShaderFullPathName;

		static ANSICHAR * vertexShaderName;
		static ANSICHAR * rayShaderName;
		static ANSICHAR * programName;

		RLprimitive m_rawData;
		Bool m_commited;

		std::shared_ptr<RLShader> m_vertexShader;
		std::shared_ptr<RLShader> m_rayShader;
		std::shared_ptr<RLProgram> m_program;

	public:
		RLprimitive & GetRawData();
		ANSICHAR * vertexShaderFullPathName;
		ANSICHAR * rayShaderFullPathName;
		Bool IsOccluder;
		RLPrimitive(Bool isOccluder = True, ANSICHAR *  vertexShaderFullPathName = defaultVertexShaderFullPathName, ANSICHAR *  rayShaderFullPathName = defaultRayShaderFullPathName);
		Bool Commited() const;
		void SetCommited(Bool commited = True);
		void Activate();
		void Inactivate();
		RLProgram * GetProgram() const;
		void ReloadVertexShader();
		void ReloadRayShader();
		void SetLightPosition(const ANSICHAR * pParamName, const Vector3 * pValue);
		void SetLightPrimivite(const ANSICHAR * pParamName, RLprimitive & value);
		void CheckStatus() const;
		~RLPrimitive();
	};
}