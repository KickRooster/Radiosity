#include "GLVertexShader.h"

namespace Core
{
	ANSICHAR * GLVertexShader::defaultShaderText = 
		"#version 330 core\n"
		"layout(location = 0) in vec4 vPos;\n"
		"layout(location = 1) in vec3 vNormal;\n"
		"layout(location = 2) in vec3 vTangent;\n"
		"layout(location = 3) in vec3 vBinormal;\n"
		"layout(location = 4) in vec3 vColor;\n"
		"layout(location = 5) in vec2 texCoord0;\n"
		"layout(location = 6) in vec2 texCoord1;\n"
		"layout(location = 7) in vec2 texCoord2;\n"
		"layout(location = 8) in vec2 texCoord3;\n"
		"layout(location = 9) in vec2 texCoord4;\n"
		"layout(location = 10) in vec2 texCoord5;\n"
		"layout(location = 11) in vec2 texCoord6;\n"
		"layout(location = 12) in vec2 texCoord7;\n"
		"layout (std140, binding = 0) uniform CameraMatrices\n"
		"{\n"
		"	mat4 viewProjectionMatrix;\n"
		"};\n"
		"layout (std140, binding = 1) uniform ObjectMatrices\n"
		"{\n"
		"	mat4 object2World;\n"
		"};\n"
		"out vec3 normal;\n"
		"out vec3 tangent;\n"
		"out vec3 binormal;\n"
		"out vec3 color;\n"
		"out vec2 uv0;\n"
		"out vec2 uv1;\n"
		"out vec2 uv2;\n"
		"out vec2 uv3;\n"
		"out vec2 uv4;\n"
		"out vec2 uv5;\n"
		"out vec2 uv6;\n"
		"out vec2 uv7;\n"
		"void main()\n"
		"{\n"
		"    gl_Position = viewProjectionMatrix * object2World * vPos;\n"
		"    normal = vNormal;\n"
		"    tangent = vTangent;\n"
		"    binormal = vBinormal;\n"
		"    color = vColor;\n"
		"    uv0 = texCoord0;\n"
		"    uv1 = texCoord1;\n"
		"    uv2 = texCoord2;\n"
		"    uv3 = texCoord3;\n"
		"    uv4 = texCoord4;\n"
		"    uv5 = texCoord5;\n"
		"    uv6 = texCoord6;\n"
		"    uv7 = texCoord7;\n"
		"}\n";

	GLVertexShader::GLVertexShader()
	{
		m_id = glCreateShader(GL_VERTEX_SHADER);
		glCheckError();
	}

	void GLVertexShader::Compile() const
	{
		glShaderSource(m_id, 1, &defaultShaderText, nullptr);
		glCheckError();

		glCompileShader(m_id);
		glCheckError();

		GLchar * info = new GLchar[1000];
		GLsizei acturalLength;

		glGetShaderInfoLog(m_id, 10000, &acturalLength, info);

		if (acturalLength != 0)
		{
			return;
		}
	}

	uint32 GLVertexShader::GetID() const
	{
		return m_id;
	}

	GLVertexShader::~GLVertexShader()
	{
		glDeleteShader(m_id);
	}
}