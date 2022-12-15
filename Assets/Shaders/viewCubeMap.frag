		#version 450 core
	
		in vec4 pos;
		in vec3 normal;
		in vec3 tangent;
		in vec3 binormal;
		in vec3 color;
		flat in vec4 customData;
		in vec2 uv0;
		in vec2 uv1;
		in vec2 uv2;
		in vec2 uv3;
		in vec2 uv4;
		in vec2 uv5;
		in vec2 uv6;
		in vec2 uv7;
		uniform samplerCube IDCubeMapSampler;
		layout (std140, binding = 0) uniform CameraUniformData
		{
			mat4 viewMatrix;
			mat4 perspectiveProjectionMatrix;
			mat4 orthoProjectionMatrix;
			vec4 position;
			vec4 NearFar;
		};
		layout (std140, binding = 1) uniform HemicubeMatrices
		{
			mat4 ViewProjection_N_Z;
			mat4 ViewProjection_N_X;
			mat4 ViewProjection_P_X;
			mat4 ViewProjection_N_Y;
			mat4 ViewProjection_P_Y;
		};
		layout (std140, binding = 2) uniform ShooterInfo
		{
			vec4 ShooterPosition;
			vec4 ShooterNormal;
			vec4 ShooterEnergy;
			vec4 ShooterSurfaceArea;
		};
		layout (std140, binding = 3) uniform ObjectMatrices
		{
			mat4 object2World;
		};
		layout (std140, binding = 4) uniform ObjectMatricesIT
		{
			mat4 object2WorldIT;
		};

		out vec4 out_Color;

		void main()
		{
			vec4 WorldPos = object2World * pos;
			vec3 SampleDir = WorldPos.xyz - position.xyz;
			vec3 CubeMap = texture(IDCubeMapSampler, normalize(SampleDir)).xyz;
			out_Color.xyz = CubeMap;
			out_Color.w = 1.0;
		};