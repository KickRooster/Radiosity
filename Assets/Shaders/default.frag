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
		layout (std140, binding = 0) uniform CameraUniformData
		{
			mat4 viewMatrix;
			mat4 perspectiveProjectionMatrix;
			mat4 orthoProjectionMatrix;
			vec4 position;
			vec4 NearFar;
		};
		layout (std140, binding = 1) uniform ShooterInfo
		{
			vec4 ShooterPosition;
			vec4 ShooterNormal;
			vec4 ShooterEnergy;
			vec4 ShooterSurfaceArea;
		};
		layout (std140, binding = 2) uniform ObjectMatrices
		{
			mat4 object2World;
		};
		layout (std140, binding = 3) uniform ObjectMatricesIT
		{
			mat4 object2WorldIT;
		};

		out vec4 out_Color;

		void main()
		{
			 vec3 albedo = texture(albedoSampler, vec2(uv0.x, uv0.y)).xyz;
			 out_Color.xyz = albedo;
			 out_Color.w = 1.0;
		};