#include "RLPrimitive.h"
#include "..\..\Helper\FileOperator.h"
#include "..\..\Helper\Helper.h"

namespace Core
{
	ANSICHAR * RLPrimitive::defaultVertexShaderSource = 
		"attribute vec4 vPos;\n"
		"attribute vec3 vNormal;\n"
		"attribute vec3 vTangent;\n"
		"attribute vec3 vBinormal;\n"
		"attribute vec3 vColor;\n"
		"attribute vec2 texCoord0;\n"
		"attribute vec2 texCoord1;\n"
		"attribute vec2 texCoord2;\n"
		"attribute vec2 texCoord3;\n"
		"attribute vec2 texCoord4;\n"
		"attribute vec2 texCoord5;\n"
		"attribute vec2 texCoord6;\n"
		"attribute vec2 texCoord7;\n"

		"void main()\n"
		"{\n"
		"	rl_Position = vPos ;\n"
		"}\n";

	ANSICHAR * RLPrimitive::defaultRayShaderSource =
		"void main()\n"
		"{\n"
		"	accumulate(vec3(1.0, 0, 1.0));\n"
		"}\n";

	ANSICHAR * RLPrimitive::defaultVertexShaderFullPathName = "E:\\LightEnvironment\\Shader\\RLSL\\default.vertex";
	ANSICHAR * RLPrimitive::defaultRayShaderFullPathName = "E:\\LightEnvironment\\Shader\\RLSL\\default.ray";

	ANSICHAR * RLPrimitive::vertexShaderName = "Vertex Shader";
	ANSICHAR * RLPrimitive::rayShaderName = "Ray Shader";
	ANSICHAR * RLPrimitive::programName = "Default Program";

	RLprimitive & RLPrimitive::GetRawData()
	{
		return m_rawData;
	}

	RLPrimitive::RLPrimitive(
		Bool isOccluder,
		ANSICHAR *  vertexShaderFullPathName,
		ANSICHAR *  rayShaderFullPathName) :
		m_commited(False),
		vertexShaderFullPathName(vertexShaderFullPathName),
		rayShaderFullPathName(rayShaderFullPathName),
		IsOccluder(False)
	{
		rlGenPrimitives(1, &m_rawData);
		rlCheckError();

		rlBindPrimitive(RL_PRIMITIVE, m_rawData);
		rlCheckError();

		//rlPrimitiveParameterString(RL_PRIMITIVE, RL_PRIMITIVE_NAME, "triangle");
		//rlCheckError();

		rlBindPrimitive(RL_PRIMITIVE, RL_NULL_PRIMITIVE);
		rlCheckError();

		//m_vertexShader = std::make_unique<RLShader>(RLShaderType_VertexShader);
		//Helper::LoadAndCompileShader(m_vertexShader.get(), vertexShaderFullPathName, vertexShaderName, defaultVertexShaderSource);

		//m_rayShader = std::make_unique<RLShader>(RLShaderType_RayShader);
		//Helper::LoadAndCompileShader(m_rayShader.get(), rayShaderFullPathName, rayShaderName, defaultRayShaderSource);
		
		//m_program = std::make_unique<RLProgram>();
		//m_program->AttachShader(m_vertexShader.get());
		//m_program->AttachShader(m_rayShader.get());
		//m_program->Link(programName);		
	}

	Core::Bool RLPrimitive::Commited() const
	{
		return m_commited;
	}

	void RLPrimitive::SetCommited(Bool commited /*= True*/)
	{
		m_commited = commited;
	}

	void RLPrimitive::Activate()
	{
		rlBindPrimitive(RL_PRIMITIVE, m_rawData);
		rlCheckError();
	}

	void RLPrimitive::Inactivate()
	{
		rlBindPrimitive(RL_PRIMITIVE, RL_NULL_PRIMITIVE);
		rlCheckError();
	}

	Core::RLProgram * RLPrimitive::GetProgram() const
	{
		return m_program.get();
	}

	void RLPrimitive::ReloadVertexShader()
	{
		m_program->DetachShader(m_vertexShader.get());

		//Helper::LoadAndCompileShader(m_vertexShader.get(), vertexShaderFullPathName, vertexShaderName, defaultVertexShaderSource);

		m_program->AttachShader(m_vertexShader.get());
		//m_program->Link(programName);
	}

	void RLPrimitive::ReloadRayShader()
	{
		m_program->DetachShader(m_rayShader.get());

		//Helper::LoadAndCompileShader(m_rayShader.get(), rayShaderFullPathName, rayShaderName, defaultRayShaderSource);

		m_program->AttachShader(m_rayShader.get());
		//m_program->Link(programName);
	}

	void RLPrimitive::SetLightPosition(const ANSICHAR * pParamName, const Vector3 * pValue)
	{
		m_program->SetUniform3f(pParamName, pValue);
	}

	void RLPrimitive::SetLightPrimivite(const ANSICHAR * pParamName, RLprimitive & value)
	{
		m_program->SetUniformPrimitive(pParamName, value);
	}

	void RLPrimitive::CheckStatus() const
	{
		RLenum value = rlCheckPrimitiveStatus(RL_PRIMITIVE);
		rlCheckError();

		if (value != RL_PRIMITIVE_COMPLETE)
		{
			if (value == RL_PRIMITIVE_INCOMPLETE_PROGRAM)
				return;
			else if (value == RL_PRIMITIVE_INCOMPLETE_MISSING_PROGRAM)
				return;
		}
	}

	RLPrimitive::~RLPrimitive()
	{
		rlDeletePrimitives(1, &m_rawData);
	}
}