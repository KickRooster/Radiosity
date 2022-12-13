		#version 450 core
	
		in vec4 pos;
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
		layout (std140, binding = 1) uniform ObjectMatrices
		{
			mat4 object2World;
		};
		layout (std140, binding = 2) uniform ObjectMatricesIT
		{
			mat4 object2WorldIT;
		};
		layout (std140, binding = 3) uniform ShooterInfo
		{
			vec3 ShooterPosition;
			vec3 ShooterNormal;
			vec3 ShooterEnergy;
			vec3 ShooterSurfaceArea;
		};

		layout(location = 0) out vec4 attch0;
		layout(location = 1) out vec4 attch1;

		void main()
		{
			//vec4 RecvPos = object2World * pos;

			vec3 albedo = texture(albedoSampler, vec2(uv0.x, uv0.y)).xyz;

			attch0.xyz = albedo.xyz;
			attch0.w = 1.0;

			attch1.xyz = albedo.xyz;
			attch1.w = 1.0;
		};