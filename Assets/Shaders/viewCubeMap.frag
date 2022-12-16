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

		out vec4 out_Color;

		void main()
		{
			vec4 WorldPos = object2World * pos;
			vec3 SampleDir = normalize(WorldPos.xyz - position.xyz);

			//	Reconstruct sampling vector from 6 perspective matrices
			/*
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
				vec4 ProjectedPos = ViewProjection_Positive_X * WorldPos;
				ProjectedPos.xyz /= ProjectedPos.w;

				SampleDir.x = 1.0;
				SampleDir.y = -ProjectedPos.y;
				SampleDir.z = -ProjectedPos.x;
			}
			else if (MaxDot == DotNegativeX)
			{
				vec4 ProjectedPos = ViewProjection_Negative_X * WorldPos;
				ProjectedPos.xyz /= ProjectedPos.w;

				SampleDir.x = -1.0;
				SampleDir.y = -ProjectedPos.y;
				SampleDir.z = ProjectedPos.x;
			}
			else if (MaxDot == DotPositiveY)
			{
				vec4 ProjectedPos = ViewProjection_Positive_Y * WorldPos;
				ProjectedPos.xyz /= ProjectedPos.w;
				
				SampleDir.x = ProjectedPos.x;
				SampleDir.y = 1.0;
				SampleDir.z = ProjectedPos.y;
			}
			else if (MaxDot == DotNegativeY)
			{
				vec4 ProjectedPos = ViewProjection_Negative_Y * WorldPos;
				ProjectedPos.xyz /= ProjectedPos.w;

				SampleDir.x = ProjectedPos.x;
				SampleDir.y = -1.0;
				SampleDir.z = -ProjectedPos.y;
			}
			else if (MaxDot == DotPositiveZ)
			{
				vec4 ProjectedPos = ViewProjection_Positive_Z * WorldPos;
				ProjectedPos.xyz /= ProjectedPos.w;

				SampleDir.x = ProjectedPos.x;
				SampleDir.y = -ProjectedPos.y;
				SampleDir.z = 1.0;
			}
			else
			{
				vec4 ProjectedPos = ViewProjection_Negative_Z * WorldPos;
				ProjectedPos.xyz /= ProjectedPos.w;

				SampleDir.x = -ProjectedPos.x;
				SampleDir.y = -ProjectedPos.y;
				SampleDir.z = -1.0;
			}
			*/

			vec3 CubeMap = texture(IDCubeMapSampler, SampleDir).xyz;
			out_Color.xyz = CubeMap;
			out_Color.w = 1.0;
		};