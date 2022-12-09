		#version 450 core
		
		in vec3 normal;
		in vec3 tangent;
		in vec3 binormal;
		in vec3 color;
		in vec4 customData;
		in vec2 uv0;
		in vec2 uv1;
		in vec2 uv2;
		in vec2 uv3;
		in vec2 uv4;
		in vec2 uv5;
		in vec2 uv6;
		in vec2 uv7;
		uniform sampler2D albedoSampler;
		layout (std140, binding = 0) uniform CameraMatrices_0
		{
			mat4 viewProjectionMatrix;
		};
		layout (std140, binding = 1) uniform ObjectMatrices
		{
			mat4 object2World;
		};

		out vec4 out_Color;

		void main()
		{
			 out_Color.xyz = vec3(customData.x, customData.x, customData.x);
			 out_Color.w = 1.0;
		};