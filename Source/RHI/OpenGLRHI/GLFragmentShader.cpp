#include "GLFragmentShader.h"

namespace Core
{
	ANSICHAR * GLFragmentShader::defaultShaderText =
		"#version 330 core\n"
		"in vec3 normal;\n"
		"in vec3 tangent;\n"
		"in vec3 binormal;\n"
		"in vec3 color;\n"
		"in vec2 uv0;\n"
		"in vec2 uv1;\n"
		"in vec2 uv2;\n"
		"in vec2 uv3;\n"
		"in vec2 uv4;\n"
		"in vec2 uv5;\n"
		"in vec2 uv6;\n"
		"in vec2 uv7;\n"
		"uniform sampler2D albedoSampler;\n"
		"layout (std140, binding = 0) uniform CameraMatrices\n"
		"{\n"
		"	mat4 viewProjectionMatrix;\n"
		"};\n"
		"layout (std140, binding = 1) uniform ObjectMatrices\n"
		"{\n"
		"	mat4 object2World;\n"
		"};\n"
		"void main()\n"
		"{\n"
		"	 vec3 albedo = texture(albedoSampler, vec2(uv0.x, uv0.y)).xyz;\n"
		"	 gl_FragColor.xyz = albedo; \n"
		"	 gl_FragColor.w = 1.0;\n"
		"}\n";

	GLFragmentShader::GLFragmentShader()
	{
		m_id = glCreateShader(GL_FRAGMENT_SHADER);
		glCheckError();
	}

	void GLFragmentShader::Compile() const
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

	uint32 GLFragmentShader::GetID() const
	{
		return m_id;
	}

	GLFragmentShader::~GLFragmentShader()
	{
		glDeleteShader(m_id);
	}
}