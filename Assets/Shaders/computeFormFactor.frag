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
		layout (std140, binding = 1) uniform CubeMatrices
		{
			mat4 ViewProjection_Positive_X;
			mat4 ViewProjection_Negative_X;
			mat4 ViewProjection_Positive_Y;
			mat4 ViewProjection_Negative_Y;
			mat4 ViewProjection_Positive_Z;
			mat4 ViewProjection_Negative_Z;
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

			vec3 SampleDir = normalize(RecvPos.xyz - ShooterPosition.xyz);

			//	Construct sampling vector from 6 perspective matrices.
			float DotPositiveX = dot(SampleDir, vec3(1.0, 0, 0));
			float DotNegativeX = dot(SampleDir, vec3(-1.0, 0, 0));
			float DotPositiveY = dot(SampleDir, vec3(0, 1.0, 0));
			float DotNegativeY = dot(SampleDir, vec3(0, -1.0, 0));
			float DotPositiveZ = dot(SampleDir, vec3(0, 0, 1.0));
			float DotNegativeZ = dot(SampleDir, vec3(0, 0, -1.0));

			float MaxDot = max(DotPositiveX, DotNegativeX);
			MaxDot = max(MaxDot, DotPositiveY);
			MaxDot = max(MaxDot, DotNegativeY);
			MaxDot = max(MaxDot, DotPositiveZ);
			MaxDot = max(MaxDot, DotNegativeZ);

			if (MaxDot == DotPositiveX)
			{
				vec4 ProjectedPos = ViewProjection_Positive_X * RecvPos;
				ProjectedPos.xyz /= ProjectedPos.w;

				SampleDir.x = 1.0;
				SampleDir.y = -ProjectedPos.y;
				SampleDir.z = -ProjectedPos.x;
			}
			else if (MaxDot == DotNegativeX)
			{
				vec4 ProjectedPos = ViewProjection_Negative_X * RecvPos;
				ProjectedPos.xyz /= ProjectedPos.w;

				SampleDir.x = -1.0;
				SampleDir.y = -ProjectedPos.y;
				SampleDir.z = ProjectedPos.x;
			}
			else if (MaxDot == DotPositiveY)
			{
				vec4 ProjectedPos = ViewProjection_Positive_Y * RecvPos;
				ProjectedPos.xyz /= ProjectedPos.w;
				
				SampleDir.x = ProjectedPos.x;
				SampleDir.y = 1.0;
				SampleDir.z = ProjectedPos.y;
			}
			else if (MaxDot == DotNegativeY)
			{
				vec4 ProjectedPos = ViewProjection_Negative_Y * RecvPos;
				ProjectedPos.xyz /= ProjectedPos.w;

				SampleDir.x = ProjectedPos.x;
				SampleDir.y = -1.0;
				SampleDir.z = -ProjectedPos.y;
			}
			else if (MaxDot == DotPositiveZ)
			{
				vec4 ProjectedPos = ViewProjection_Positive_Z * RecvPos;
				ProjectedPos.xyz /= ProjectedPos.w;

				SampleDir.x = ProjectedPos.x;
				SampleDir.y = -ProjectedPos.y;
				SampleDir.z = 1.0;
			}
			else
			{
				vec4 ProjectedPos = ViewProjection_Negative_Z * RecvPos;
				ProjectedPos.xyz /= ProjectedPos.w;

				SampleDir.x = -ProjectedPos.x;
				SampleDir.y = -ProjectedPos.y;
				SampleDir.z = -1.0;
			}

			vec3 CubeMap = texture(IDCubeMapSampler, SampleDir).xyz;
			
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

			attch0.xyz = albedo;
			attch0.w = 1.0;

			attch1.xyz = Delta;
			
			attch1.w = 1.0;
		};