		#version 450 core
		in vec3 normal;
		in vec3 tangent;
		in vec3 binormal;
		in vec3 color;
		in vec2 uv0;
		in vec2 uv1;
		in vec2 uv2;
		in vec2 uv3;
		in vec2 uv4;
		in vec2 uv5;
		in vec2 uv6;
		in vec2 uv7;

		out vec4 out_Color;

		uniform sampler2D albedoSampler;
		layout (std140, binding = 0) uniform CameraUniformData
		{
			mat4 viewMatrix;
			mat4 viewProjectionMatrix;
			vec4 position;
			vec4 NearFar;
		};
		layout (std140, binding = 1) uniform ObjectMatrices
		{
			mat4 object2World;
		};
		void main()
		{
			 vec3 albedo = texture(albedoSampler, vec2(uv0.x, uv0.y)).xyz;
			 out_Color.xyz = albedo; 
			 out_Color.w = 1.0;
		};