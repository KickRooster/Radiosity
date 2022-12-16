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
		uniform sampler2D albedoSampler;
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
		layout(location = 0) out vec4 attch0;
		layout(location = 1) out vec4 attch1;

		void main()
		{
			vec4 RecvPos = object2World * pos;
			vec3 r = ShooterPosition.xyz - RecvPos.xyz;
			float distance2 = dot(r, r);
			vec4 Normal4 = vec4(normal.x, normal.y, normal.z, 1.0f);
			vec4 WorldNormal4 = object2WorldIT * Normal4;
			vec3 WorldNormal = normalize(WorldNormal4.xyz);
            float cosi = dot(WorldNormal, normalize(r));    
            float cosj = -dot(ShooterNormal.xyz, normalize(r)); 
			const float pi = 3.1415926535;
			float Fij = max(cosi * cosj, 0) / (pi * distance2 + ShooterSurfaceArea.x);

			vec4 WorldPos = object2World * pos;
			vec3 SampleDir = WorldPos.xyz - position.xyz;

			vec3 CubeMap = texture(IDCubeMapSampler, normalize(SampleDir)).xyz;
			
			float Visable;
			if (CubeMap.x == customData.x)
			{
				Visable = 1.0f;
			}
			else
			{
				Visable = 0;
			}

			Fij *= Visable;
			Fij *= 50;

			vec3 albedo = texture(albedoSampler, uv0).xyz;

			vec3 Delta = ShooterEnergy.xyz * albedo * ShooterSurfaceArea.x * Fij;   

			attch0.xyz = Delta;
			attch0.w = 1.0;

			attch1.xyz = Delta;
			
			attch1.w = 1.0;
		};